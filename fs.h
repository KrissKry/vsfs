#ifndef FS_H
#define FS_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define FS_SIZE 16384
#define BLOCK_SIZE 512
#define INODE_SIZE  64      //64B
#define DATA_BLOCK_COUNT 25 //25*512B = 12.5kB
#define INODE_COUNT 32      //32*64B = 2kB

/* FS_SIZE = 16kB of which:
*  Superblock takes 0.5kB
*  BMP of Data and inode take 1kB
*  iNodes take 2kB
*  Data takes 12.5kB
*/

/* virtual file system */
typedef struct SuperBlock {
    int blocksNum;          /* blocks in fs */
    int dataBlocksNum;      /* data blocks in fs */
    int iNodeBlocksNum;     /* inodeblocks in fs */
    int iNodesNum;          /* inodes in fs */

    int dataBlockSize;      /*number of bytes per data block */
    int iNodeSize;          /* .. */

    int iNodesOffset;
    int dataBlockOffset;
    int iNodeTakenBMPOffset;
    int dataTakenBMPOffset;
} SuperBlock;

/* Bitmap for inodes currently in use */
typedef struct iNodeTakenBMP {
    char taken[INODE_COUNT];
} iNodeTakenBMP;

/* Bitmap for data blocks currently in use */
typedef struct dataTakenBMP {
    char taken[DATA_BLOCK_COUNT];
} dataTakenBMP;

// typedef struct DataBlock {
//     int valid;
//     int size;
//     char name[32];
//     char data*;        
// } DataBlock;

/* iNode for describing file with proper details */
typedef struct iNode{
    int startingBlock;      //4B - first block of data number 
    int size;               //4B - size in bytes of data
    time_t lastAccessed;    //4B
    time_t lastModified;    //4B 
    char name[48];          //48B

} iNode;                    //64B



int mkfs();     /* create virtual disk and filesystem on it */
int rmfs();     /* remove filesystem                        */
int rm(char* filename);       /* remove file from fs                      */
int ls();       /* view directory on fs                     */
int cptfs(char* filename);    /* copy to fs from minix                    */
int cpffs(char* filename);    /* copy from fs to minix                    */
int fsinfo();   /* view info on fs                          */



#define VSFSEXISTS -1
#define VSFSNOCREATION -2
#define VSFSDISCARDED -3
#define VSFSRMFSERROR -4
#define VSFSFILENOTEXISTS -5
#define VSFSSRCERROR -6
#define VSFSSRCTOOLARGE -7
#define VSFSNODEST -8
#define VSFSDESTNOTFOUND -9
#define ARGSMISSING -10
#endif