#include "memory.h"
#include "fs.h"



int mkfs() {

    FILE* fp;
    char null;
    char temp[FS_SIZE];
    int index;
    SuperBlock sb;
    iNode inode[INODE_COUNT];
    iNodeTakenBMP nodeBMP;
    dataTakenBMP dataBMP;

    if (fp = fopen("verysmallfilesystem", "r") ) {
        //file system already exists
        printf("VSFS already exists!");
        fclose(fp);
        return VSFSEXISTS;
    }

    /* Tries creating file with write (a) and read (+) permissions in binary mode (b)
    *  Stream positioned at the end of the file */
    if ( !(fp = fopen("verysmallfilesystem", "a+b") ) ) {
        //file system cannot be created
        printf("VSFS cannot be created!");
        return VSFSNOCREATION;
    } 

    /*nullify whole file */
    null = '\0';
    fwrite(&null, sizeof(char), FS_SIZE, fp);
    // for(index = 0; index < FS_SIZE; index++) {
    //     fwrite(&null, sizeof(char), 1, fp);
    //     //memset
    // }

    fclose(fp);

    /* Insert correct data */
    sb.blocksNum = FS_SIZE / BLOCK_SIZE;
    sb.dataBlocksNum = DATA_BLOCK_COUNT;
    sb.iNodeBlocksNum = INODE_COUNT / ( BLOCK_SIZE / INODE_SIZE );
    sb.iNodesNum = INODE_COUNT;

    sb.dataBlockSize = BLOCK_SIZE;
    sb.iNodeSize = INODE_SIZE;

    sb.iNodeTakenBMPOffset = 1;
    sb.dataTakenBMPOffset = 2;
    sb.iNodesOffset = 3;
    sb.dataBlockOffset = 7;

    /* Open file for input/output without discarding content (w+).
       The file must exist.                                         */
    if ( !(fp = fopen("verysmallfilesystem", "r+b")) ) {
        printf("VSFS cannot be opened again!");
        return VSFSDISCARDED;
    }

    for (index = 0; index < DATA_BLOCK_COUNT; index++) {
        dataBMP.taken[index] = '\0';
    }

    for (index = 0; index < INODE_COUNT; index++) {
        nodeBMP.taken[index] = '\0';
    }

    memcpy( temp, &sb, sizeof(SuperBlock) );
    memcpy( temp + (sb.dataBlockSize * sb.iNodeTakenBMPOffset), nodeBMP.taken, sizeof(nodeBMP) );
    memcpy( temp + (sb.dataBlockSize * sb.dataTakenBMPOffset), dataBMP.taken, sizeof(dataBMP) );

    fwrite(&sb, sizeof(temp),  1, fp);
    fclose(fp);

    printf("VSFS successfully created!\n");
    return 0;
}