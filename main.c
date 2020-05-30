#include "fs.h"


int main(int argc, char* argv[])
{
    if( (strcmp(argv[1], "mkfs") == 0) )
        return mkfs();

    if( (strcmp(argv[1], "rmfs") == 0) )
        return rmfs();

    if( (strcmp(argv[1], "rm") == 0) ) {
        
        if(argc < 3)
            return ARGSMISSING;

        return rm(argv[2]);
    }

    if( (strcmp(argv[1], "ls") == 0) )
        return ls();

    if( (strcmp(argv[1], "cptfs") == 0) ){

        if(argc < 3)
            return ARGSMISSING;
        
        return cptfs(argv[2]);
    }

    if( (strcmp(argv[1], "cpffs") == 0) ){

        if(argc < 3)
            return ARGSMISSING;
        
        return cpffs(argv[2]);
    }

    if( (strcmp(argv[1], "fsinfo") == 0) )
        return fsinfo();
        
    printf("Wrong arguments!\n");
    return ARGSMISSING;
}