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
#include "xmod.h"
#include <unistd.h>
#include <wait.h>
#include "signal.h"

int isFirstParent=1;

int main(int argc, char * argv[]) {
    
    initClock();  // Initialize start clock
    isFirstParent = initProcess();  // Determinar se isParent

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

    if (xmodInfo.flags.recursive) {
        if (changePermissionRecursive(&xmodInfo, argc, argv) != 0) EXIT(2);
    }
    else {
        if (changePermission(&xmodInfo) != 0) EXIT(2);
    }
    

    //Se não for pai, envia o SIGCHLD ao pai
    if(!isFirstParent){
        
        eventSignalSent(SIGCHLD, getppid());
    }
    
    EXIT(0);
}