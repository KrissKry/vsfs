#include "fs.h"


int fsinfo()
{
    FILE* fp;
    int index;
    int realSize = 0;
    int blocksOccupied = 0;
    int nodesOccupied = 0;
    double realDiskUsage = 0;
    double nodeUsage;
    SuperBlock sb;
    iNodeTakenBMP nodebmp;
    iNode nodes[INODE_COUNT];

    /* try opening filesystem */
    if ( !(fp = fopen("verysmallfilesystem", "rb")) ){

        printf("Couldn't open filesystem! \nPerhaps it doesn't exist?");
        return VSFSDISCARDED;
    }

    /* read Superblock */
    fread(&sb, sizeof(SuperBlock), 1, fp);

    /* move to iNodes bitmap and read */
    fseek(fp, sb.iNodeTakenBMPOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nodebmp, sizeof(char), sb.iNodesNum, fp);

    /* move to nodes and read */
    fseek(fp, sb.iNodesOffset*sb.dataBlockSize, SEEK_SET);
    fread(&nodes, sb.iNodeSize, sb.iNodesNum, fp);

    /* print block type */
    printf("SB - Superblock, n - iNodeBitmap, d - dataBitmap, I - iNode block, D - data block\n");
    for ( index = 0; index < sb.blocksNum; index++ )
    {
        if (index < sb.iNodeTakenBMPOffset )
            printf("[S|");
        if (index >= sb.iNodeTakenBMPOffset && index < sb.dataTakenBMPOffset)
            printf("n|");
        if (index >= sb.dataTakenBMPOffset && index < sb.iNodesOffset)
            printf("d|");
        if (index >= sb.iNodesOffset && index < sb.dataBlockOffset)
            printf("I|");
        if (index >= sb.dataBlockOffset)
            printf("D|");
        if(index % 15 == 0 && index > 0)
            printf("\n ");
    }

    /* print each node info and collect visual data*/
    for ( index = 0; index < sb.iNodesNum; index++)
    {
        if( nodebmp.taken[index] != '\0' ) {
            printf("Node %d - bBlock %d - filename %s - filesize %d \n", index, nodes[index].startingBlock, nodes[index].name, nodes[index].size);
            blocksOccupied += 1 + (nodes[index].size / sb.dataBlockSize);
            realSize += nodes[index].size;
            nodesOccupied++;
        }
        else
            printf("Node %d unused\n", index);
    }

    /* print visually disk usage */
    printf("Disk usage:\n");
    for (index = 0; index < sb.dataBlocksNum; index++)
    {
        if(blocksOccupied > 0) {
            printf("#");
            blocksOccupied--;
        }
        else 
            printf("_");

        //if (index > 0 && index % 5 == 0)
            //printf("\n");
    }

    /* print visually real disk usage */
    realDiskUsage = 100 * (realSize / (sb.dataBlocksNum*sb.dataBlockSize) );
    printf("\nReal disk usage:\n");
    for (index = 0; index < 100; index++)
    {
        if (index < realDiskUsage)
            printf("#");
        else
            printf("_");

        //if (index % 9 == 0 && index > 0)
            //printf("\n");
        
    }

    /* print number of nodes in use and percentage of total */
    nodeUsage = (double)nodesOccupied / sb.iNodesNum * 100;
    printf("\nNodes currently in use: %d out of %d which is %.2f%\n", nodesOccupied, sb.iNodesNum, nodeUsage);

    printf("sizeofnode %zu\n", sizeof(iNode));
    return 0;
}