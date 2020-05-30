#include "fs.h"


int ls()
{
    FILE* fp;
    SuperBlock sb;
    iNodeTakenBMP nodebmp;
    iNode nodes[INODE_COUNT];
    int index;
    /* try opening filesystem */
    if ( !(fp = fopen("verysmallfilesystem", "rb")) ){

        printf("Couldn't open filesystem! \nPerhaps it doesn't exist?\n");
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

    /* print nodes that exist */
    for ( index = 0; index < sb.iNodesNum; index++)
    {
        if ( nodebmp.taken[index] != '\0' ) {
            
            printf("%s\n", nodes[index].name);
        }
    }
    fclose(fp);
    return 0;
}