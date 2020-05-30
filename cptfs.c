#include "fs.h"


int cptfs(char* filename)
{
    FILE* fp;
    FILE* src;
    SuperBlock sb;
    iNodeTakenBMP nBMP;
    dataTakenBMP dBMP;
    iNode nodes[INODE_COUNT];
    iNode temp;
    int index;
    int inner_index;
    int file_max_size = BLOCK_SIZE*DATA_BLOCK_COUNT;
    int data_blocks_needed;
    int starting_block;
    int src_size;
    char taken;
    bool has_space;
    char src_buf[BLOCK_SIZE*DATA_BLOCK_COUNT];
    time_t now = time(0);

    /* try opening VSFS */
    if ( !(fp = fopen("verysmallfilesystem", "r+b")) ){

        printf("Couldn't open filesystem! \nPerhaps it doesn't exist?");
        return VSFSDISCARDED;
    }

    /* read Superblock */
    fread(&sb, sizeof(SuperBlock), 1, fp);

    /* move to iNodes bitmap and read */
    fseek(fp, sb.iNodeTakenBMPOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nBMP, sizeof(char), sb.iNodesNum, fp);

    /* move to data bitmap and read */
    fseek(fp, sb.dataTakenBMPOffset*sb.dataBlockSize, SEEK_SET);
    fread(&dBMP, sizeof(char), sb.dataBlocksNum, fp);

    /* move to nodes and read */
    fseek(fp, sb.iNodesOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nodes, sb.iNodeSize, sb.iNodesNum, fp);

    /* check if filename already exists in vsfs */
    for(index = 0; index < sb.iNodesNum; index++)
    {
        if( nBMP.taken[index] != '\0' && strcmp(nodes[index].name, filename) == 0 ){
            printf("File already exists!\n");
            return VSFSSRCERROR;
        }
    }


    /* try opening src file in binary mode */
    if( !(src = fopen(filename, "rb")) ) {

        printf("Source file cannot be opened!\n");
        return VSFSSRCERROR;
    }

    /* read up to max data possible from src file */
    fread(src_buf, file_max_size, 1, src);

    /* close src file as its not needed anymore */
    fclose(src);

    /* calculate blocks of data needed to store file from minix into VSFS */
    src_size = strlen(src_buf);
    data_blocks_needed = 1 + src_size / sb.dataBlockSize;

    /* find first available set of blocks for data */
    for(index = 0; index < sb.dataBlocksNum; index++) {

        has_space = false;

        /* look for untaken data block */
        if (dBMP.taken[index] == '\0') {

            starting_block = index;

            /* check next n blocks to see if src fits into vsfs */
            for(inner_index = 0; inner_index < data_blocks_needed; inner_index++) {

                if(dBMP.taken[index + inner_index] == '\0') {
                    has_space = true;
                }
                else
                {
                    /* break the inner loop on first failure */
                    has_space = false;
                    break;
                }

            }
            
        }
        if ( has_space )
                break;
    }

    /* if set of blocks was found, insert data */
    if ( has_space ) {

        /* mark inode pointing to starting block as taken */
        /* copy data to correct node */
        for(index = 0; index < sb.iNodesNum; index++)
        {
            if (nBMP.taken[index] == '\0') {

                taken = '1';
                
                /* insert data to temporary node */
                strcpy(temp.name,filename);
                temp.size = src_size;
                temp.startingBlock = starting_block;
                temp.lastAccessed = now;
                temp.lastModified = now;


                /* move to iNode bitmap, mark correct bit as taken */
                fseek(fp, sb.iNodeTakenBMPOffset*sb.dataBlockSize + index*sizeof(char), SEEK_SET);                
                fwrite(&taken, sizeof(char), 1, fp);


                /* move to data bitmap, mark correct block bits as taken */
                for(inner_index = 0; inner_index < data_blocks_needed; inner_index++) {
                    fseek(fp, sb.dataTakenBMPOffset*sb.dataBlockSize + (starting_block+inner_index)*sizeof(char), SEEK_SET);
                    fwrite(&taken, sizeof(char), 1, fp);
                }

                /* move to nodes and set a node with correct data */
                fseek(fp, sb.iNodesOffset*sb.dataBlockSize + index*sb.iNodeSize, SEEK_SET);
                fwrite(&temp, sb.iNodeSize, 1, fp);

                break;
            }
        }
        

        /* move in file to correct data blocks */
        fseek(fp, (sb.dataBlockOffset + starting_block)*sb.dataBlockSize, SEEK_SET);

        /* write data to data blocks */
        fwrite(src_buf, src_size, 1, fp);
        
        /* flush file stream to maintain real-time response */
        fflush(fp);
        fclose(fp);

        printf("File %s copied to VSFS @ Node %d, Starting data block %d\n", filename, index, sb.dataBlockOffset + starting_block);
        return 0;
    }
    else
    {
        printf("Could not find space for src file!\n");
        return VSFSSRCTOOLARGE;
    }


}