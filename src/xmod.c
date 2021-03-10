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

extern clock_t startClock;

int main(int argc, char * argv[]) {
    //Start the clock:
    struct tms t;
    startClock = times(&t);
    if (argc < 3 || argc > 6)
        printf("usage: xmod [options] mode file/dir");
    if (open(argv[argc - 1], O_RDONLY)  < 0)
        printf("xmod: cannot access '%s' : No such file or directory/nfailed to change mode of '%s' from 0000 (---------) to 0000 (---------)", argv[argc-1], argv[argc-1]);
    initLogFile();
    eventProcCreat(argc, argv);
    XmodInfo xmodInfo;
    fillXmodInfo(&xmodInfo, argc, argv);
    changePermission(&xmodInfo);
    eventProcExit(0);
    return 0;
}