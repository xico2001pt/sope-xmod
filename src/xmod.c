#include "change.h"
#include "regfile.h"
#include "xmod_info.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char * argv[]) {
    initLogFile();
    XmodInfo xmodInfo;
    if (argc < 3 || argc > 6) {
        printf("usage: xmod [options] mode file/dir\n");
        return 1;
    }

    //Chekcs if the file exists
    if(checkFileStatus(argv[argc-1],&xmodInfo)==-1){
        printf("xmod: cannot access '%s' : No such file or directory\nfailed to change mode of '%s' from 0000 (---------) to 0000 (---------)\n", argv[argc-1], argv[argc-1]);
        exit(1);
    }
    eventProcCreat(argc, argv);
    
    fillXmodInfo(&xmodInfo, argc, argv);
    changePermission(&xmodInfo);
    eventProcExit(0);
    endLogFile();

    return 0;
}