#include "fs.h"

int cpffs(char* filename)
{
    FILE* fp;
    FILE* dest;
    SuperBlock sb;
    iNodeTakenBMP nBMP;
    dataTakenBMP dBMP;
    iNode nodes[INODE_COUNT];
    int index;
    int dest_size;
    int dest_starting_block;
    char dest_buf[DATA_BLOCK_COUNT*BLOCK_SIZE];
    bool file_found = false;


    /* try opening VSFS */
    if ( !(fp = fopen("verysmallfilesystem", "r+b")) ){

        printf("Couldn't open filesystem! \nPerhaps it doesn't exist?");
        return VSFSDISCARDED;
    }
    /* read superblock */
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

    /* look for dest_filename in nodes */
    /* if found, get size and starting block */
    for (index = 0; index < sb.iNodesNum; index++)
    {
        if(nBMP.taken[index] != '\0' && strcmp(nodes[index].name, filename) == 0)
        {
            printf("File %s found in VSFS.\n", filename);
            dest_size = nodes[index].size;
            dest_starting_block = nodes[index].startingBlock;
            file_found = true;
            break;
        }
    }

    if( file_found ) 
    {
        /* move to correct data block, read dest_size number of bytes to dest_buf */
        fseek(fp, (sb.dataBlockOffset+dest_starting_block)*sb.dataBlockSize, SEEK_SET);
        fread(dest_buf, sizeof(char), dest_size, fp);
        fclose(fp);

        /* try creating / opening dest file */
        if( !(dest = fopen(filename, "a+b")) )
        {
            printf("Couldn't create destination file!\n");
            return VSFSNODEST;
        }
        /*jump to beginning of file */
        fseek(dest, 0, SEEK_SET);

        /* write dest_buf to destination file */
        fwrite(dest_buf, strlen(dest_buf), 1, dest);

        fclose(dest);

        printf("Successfully copied file %s from VSFS\n", filename);
        return 0;
    }
    else
    {
        printf("File not found!\n");
        return VSFSDESTNOTFOUND;
    }
    
}