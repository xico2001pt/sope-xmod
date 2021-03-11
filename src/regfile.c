#include "regfile.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

/**
 * @brief LOG_FILENAME number if it exists, -1 if it doesnt
 * 
 */
static int logFile;

static clock_t startClock;

int initLogFile() {
    struct tms t;
    startClock = times(&t);
    
    char* logFilename = getenv("LOG_FILENAME");
    
    if (logFilename == NULL) {  // The path to the filename doesn't exist
        logFile = -1;
        return 1;
    }
    else {
        // Flags -> Write only, create file if doesn't exist and truncate if it does
        // Permissions -> Read and write for all users
        logFile = open(logFilename, O_WRONLY|O_CREAT|O_TRUNC, S_IRGRP|S_IRUSR|S_IROTH|S_IWGRP|S_IWOTH|S_IWUSR);
        if(logFile == -1) return -1;  // Error opening the file
    }
    return 0;
}

int registerEvent() {
    // Verify if the file exists
    if (logFile == -1)
        return 1;
    
    // Register instant
    struct tms t;
    char instStr[10];
    int inst = (times(&t) - startClock) * 1000 / sysconf(_SC_CLK_TCK);
    sprintf(instStr, "%d ; ", inst);
    write(logFile, instStr, strlen(instStr));

    // Register PID
    char pidStr[10];
    pid_t pid = getpid();
    sprintf(pidStr, "%d; ", pid);
    write(logFile, pidStr, strlen(pidStr));

    return 0;
}

int eventProcCreat(int argc, char * argv[]) {
    // Verify if the file exists
    if (registerEvent() == 1)
        return 1;
    
    // After inst; pid; write event ;
    write(logFile, "PROC_CREAT",10);
    write(logFile, " ; ", 3);

    // After inst ; pid ; event ; write arg[1] arg[2] arg[3]
    for (int i = 0; i < argc; i++) {
        write(logFile, argv[i], strlen(argv[i]));
        write(logFile, " ", 1);
    }
    write(logFile, "\n", 1);

    return 0;
}

int eventProcExit(int exitStatus) {
    // Verify if the file exists
    if (registerEvent() == 1)
        return 1;
    
    // After inst; pid; write event ;
    write(logFile, "PROC_EXIT", 9);
    write(logFile, " ; ", 3);

    // Write exit status
    char exitStr[10];
    sprintf(exitStr, "%d\n", exitStatus);
    write(logFile, exitStr, strlen(exitStr));

    return 0;
}

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode) {
    // Verify if the file exists
    if (registerEvent() == 1)
        return 1;
    
    // Write event
    write(logFile, "FILE_MODF", 9);
    write(logFile, " ; ", 3);

    // Write filename
    write(logFile, filename, strlen(filename));
    write(logFile, " : ", 3);

    // Write oldMode
    char oldmode[10];
    sprintf(oldmode, "%#o : ", oldMode);
    write(logFile, oldmode, strlen(oldmode));

    // Write newMode
    char newmode[10];
    sprintf(newmode, "%#o\n", newMode);
    write(logFile, newmode, strlen(newmode));

    return 0;
}


int endLogFile() {
    // Verify if the file exists
    if(logFile == -1)
        return 1;

    close(logFile);
    return 0;
}
