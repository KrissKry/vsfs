#include "fs.h"


int rm(char* filename)
{
    FILE* fp;
    int index;
    int bmpindex;
    int blockNum;
    char null;
    // int name_index;
    // int filename_index;
    // char buf[2048];
    SuperBlock sb;
    iNodeTakenBMP nodebmp;
    dataTakenBMP databmp;
    iNode nodes[INODE_COUNT];


    if ( !(fp = fopen("verysmallfilesystem", "r+b")) ) {
        printf("Unable to open VSFS!");
        return VSFSDISCARDED;
    }

    fread(&sb, sizeof(SuperBlock), 1, fp);

    /* Move to iNode bitmap and read */
    fseek(fp, sb.iNodeTakenBMPOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nodebmp, sizeof(char), sb.iNodesNum, fp);

    /* Move to data bitmap and read */
    fseek(fp, sb.dataTakenBMPOffset*sb.dataBlockSize, SEEK_SET);
    fread(&databmp, sizeof(char), sb.dataBlocksNum, fp);

    /* Move to nodes and read */
    fseek(fp, sb.iNodesOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nodes, sb.iNodeSize, sb.iNodesNum, fp);

    /* iterate over all inodes */
    for(index = 0; index < sb.iNodesNum; index++)
    {
        /* if bitmap shows node as taken */
        if( nodebmp.taken[index] != '\0' ) {
            
            /* if name under that node is equal to given filename */
            if ( strcmp(nodes[index].name, filename) == 0 ) {
                
                /* assign null value to char variable */
                null = '\0';

                /* get number of data blocks taken by file                        */
                /* size 300B / 512B rounded to int = 0, which is why we add 1     */
                blockNum = 1 + (nodes[index].size / sb.dataBlockSize);

                /* move to proper node in file */
                fseek(fp, sb.iNodeTakenBMPOffset*sb.dataBlockSize + index, SEEK_SET);

                /* set inode.size = \0 */
                fwrite(&null, sizeof(char), 1, fp);

                /* move to data bitmap */
                fseek(fp, sb.dataTakenBMPOffset*sb.dataBlockSize + index, SEEK_SET);

                /* set data blocks as available for write */
                for(bmpindex = 0; bmpindex < blockNum; bmpindex++)
                {
                    fwrite(&null, sizeof(char), 1, fp);
                    fseek(fp, 1, SEEK_CUR);
                }

                fclose(fp);
                printf("Removed file %s\n", filename);
                return 0;
            }

        }
    }
    printf("File not found!\n");
    return VSFSFILENOTEXISTS;


    /*moves forward in file by Superblock, inode_bitmap and data_bitmap*/
    //fseek(fp, 1536, SEEK_SET);
    /* reads  inodes */
    //fread(buf, 2048, 1, fp);

    // for(index = 0; index < sizeof(buf); index+64)
    // {
    //     filename_index = 0;
    //     for(name_index = index + 16; name_index < index + 64; name_index++)
    //     {
    //         if ( filename[filename_index] == '\0')
    //         if(buf[name_index] = '\0')
    //             break;

            
    //     }
    // }    
}