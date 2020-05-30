#include "fs.h"


int rmfs() {
    
    char filename[] = "verysmallfilesystem";
    if ( remove(filename) < 0) {
        printf("Cannot remove file. \nPerhaps it's already deleted?\n");
        return VSFSRMFSERROR;
    }
    printf("File successfully removed!\n");
    return 0;
}