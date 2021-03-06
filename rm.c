#include "fs.h"


int rm(char* filename)
{
    FILE* fp;
    int index;
    int bit_index;
    int blockNum;
    char null;
    SuperBlock sb;
    iNodeTakenBMP nBMP;
    dataTakenBMP dBMP;
    iNode nodes[INODE_COUNT];


    if ( !(fp = fopen("verysmallfilesystem", "r+b")) ) {
        printf("Unable to open VSFS!");
        return VSFSDISCARDED;
    }

    fread(&sb, sizeof(SuperBlock), 1, fp);

    /* Move to iNode bitmap and read */
    fseek(fp, sb.iNodeTakenBMPOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nBMP, sizeof(char), sb.iNodesNum, fp);

    /* Move to data bitmap and read */
    fseek(fp, sb.dataTakenBMPOffset*sb.dataBlockSize, SEEK_SET);
    fread(&dBMP, sizeof(char), sb.dataBlocksNum, fp);

    /* Move to nodes and read */
    fseek(fp, sb.iNodesOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nodes, sb.iNodeSize, sb.iNodesNum, fp);

    /* iterate over all inodes */
    for(index = 0; index < sb.iNodesNum; index++)
    {
        /* if bitmap shows node as taken */
        if( nBMP.taken[index] != '\0' ) {
            /*printf("node %d taken, filename %s\n", index, nodes[index].name); needed before fflush in cptfs.c */
            /* if name under that node is equal to given filename */
            if ( strcmp(nodes[index].name, filename) == 0 ) {
                
                /* assign null value to char variable */
                null = '\0';

                /* get number of data blocks taken by file                        */
                /* size 300B / 512B rounded to int = 0, which is why we add 1     */
                blockNum = 1 + (nodes[index].size / sb.dataBlockSize);

                /* move to proper node in file */
                fseek(fp, sb.iNodeTakenBMPOffset*sb.dataBlockSize + index*sizeof(char), SEEK_SET);

                /* set inode.size = \0 */
                fwrite(&null, sizeof(char), 1, fp);

                /* move to data bitmap, to correct starting block */
                fseek(fp, sb.dataTakenBMPOffset*sb.dataBlockSize + nodes[index].startingBlock*sizeof(char), SEEK_SET);

                /* set data blocks as available for write */
                for(bit_index = 0; bit_index < blockNum; bit_index++)
                {
                    fwrite(&null, sizeof(char), 1, fp);
                    /*fseek(fp, 1, SEEK_CUR);*/
                }

                fclose(fp);
                printf("Removed file %s\n", filename);
                return 0;
            }

        }
    }
    printf("File not found!\n");
    return VSFSFILENOTEXISTS;  
}