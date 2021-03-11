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
    initLogFile();  // Initialize log file and related information
    eventProcCreat(argc, argv);  // Register process creation event

    // Check if number of console arguments is valid
    if (argc < MIN_NUM_ARGS || argc > MAX_NUM_ARGS) {
        printf("usage: xmod [options] mode file/dir\n");
        EXIT(1);
    }

    // Fill xmod information
    XmodInfo xmodInfo;
    if (fillXmodInfo(&xmodInfo, argc, argv) != 0) EXIT(1);

    if (changePermission(&xmodInfo) != 0) EXIT(2);

    EXIT(0);
}