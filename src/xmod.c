#include "xmod.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include "change.h"
#include "regfile.h"
#include "xmod_info.h"
#include "signal.h"

int isFirstParent;

int main(int argc, char * argv[]) {
    
    initClock();  // Initialize start clock
    isFirstParent = initProcess();  // Determine if isParent

    // Initialize signal handlers
    signal(SIGINT, signSIGINTHandler);
    signal(SIGCONT, sighandlerSIGCONT);
    signal(SIGCHLD, sigHandlerSIGCHILD);
    signal(SIGUSR1, sigHandlerSIGUSR1);

    initLogFile(isFirstParent);  // Initialize log file
    eventProcCreat(argc, argv);  // Register process creation event

    // Check if number of console arguments is valid
    if (argc < MIN_NUM_ARGS || argc > MAX_NUM_ARGS) {
        printf("usage: xmod [options] mode file/dir\n");
        EXIT(1);
    }

    // Fill xmod information
    XmodInfo xmodInfo;
    if (fillXmodInfo(&xmodInfo, argc, argv) != 0) EXIT(1);

    // Change FILE/DIR permissions
    if (xmodInfo.flags.recursive) {
        if (changePermissionRecursive(&xmodInfo, argc, argv) != 0) EXIT(2);
    } else {
        if (changePermission(&xmodInfo) != 0) EXIT(2);
    }

    // If not isParent, send SIGCHLD to parent
    if (!isFirstParent) eventSignalSent(SIGCHLD, getppid());
    
    EXIT(0);
}