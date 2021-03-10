#include "change.h"
#include "regfile.h"
#include "xmod_info.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern clock_t startClock;

int main(int argc, char * argv[]) {
    //Start the clock:
    struct tms t;
    startClock = times(&t);
    initLogFile();
    eventProcCreat(argc, argv);
    XmodInfo xmodInfo;
    fillXmodInfo(&xmodInfo, argc, argv);
    changePermission(&xmodInfo);
    eventProcExit(0);
    return 0;
}