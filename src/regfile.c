#include "regfile.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "signal.h"

/**
 * @brief LOG_FILENAME number if it exists, -1 if it doesnt
 * 
 */
static int logFile;

static clock_t startClock;

void initClock() {
    struct tms t;
    startClock = times(&t);
}

void setClock(clock_t clock) {startClock = clock;}

clock_t getClock() {return startClock;}

int initLogFile(int firstParent) {
    char* logFilename = getenv("LOG_FILENAME");
    setenv("LOCK", "1", 1);
    
    if (logFilename == NULL) {  // The path to the filename doesn't exist
        logFile = -1;
        return 1;
    } else {
        // Flags -> Write only, create file if doesn't exist and truncate if it does
        // Permissions -> Read and write for all users
        if (firstParent) {
            logFile = open(logFilename, O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, S_IRGRP|S_IRUSR|S_IROTH|S_IWGRP|S_IWOTH|S_IWUSR);
        } else {
            logFile = open(logFilename, O_WRONLY|O_APPEND|O_SYNC);
            //write(logFile, "\n\n", 2);
        }

        if (logFile == -1) return -1;  // Error opening the file
    }
    
    return 0;
}

void hasWritePermission() {
    int permission;
    while (sscanf(getenv("LOCK"), "%d", &permission) && !permission);
    setenv("LOCK", "0", 0);
}

void lockPermission() {setenv("LOCK", "1", 1);}

int registerEvent(char *str) {
    // Verify if the file  or the string exist
    if (logFile == -1 || str == NULL)
        return 1;
    
    // Register instant and PID
    struct tms t;
    int inst = (times(&t) - startClock) * 1000 / sysconf(_SC_CLK_TCK);
    pid_t pid = getpid();
    snprintf(str, MAX_CHARS, "%d ; %d ; ", inst, pid);

    return 0;
}

int eventProcCreat(int argc, char * argv[]) {
    hasWritePermission();

    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event, sizeof(event), "%sPROC_CREAT ; ", event); // After inst; pid; write event ;

    // After inst ; pid ; event ; write arg[1] arg[2] arg[3] ...
    for (int i = 0; i < argc; i++) {
        snprintf(event, sizeof(event), "%s%s ", event, argv[i]);
    }
    snprintf(event, sizeof(event), "%s\n",event);
    
    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    lockPermission();
    return 0;
}

int eventProcExit(int exitStatus) {
    hasWritePermission();

    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event, sizeof(event), "%sPROC_EXIT ; %d\n", event, exitStatus);  // After inst; pid; write event ; Write exit status

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    lockPermission();
    return 0;
}

int eventSignalRecv(int signo) {
    hasWritePermission();

    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event, sizeof(event), "%sSIGNAL_REC ; ", event);

    char sign[20];
    switch(signo){
        case SIGINT:
        snprintf(sign, sizeof(sign), "SIGINT\n");
        break;
        case SIGCHLD:
        snprintf(sign, sizeof(sign), "SIGCHLD\n");
        break;
        case SIGCONT:
        snprintf(sign, sizeof(sign), "SIGCONT\n");
        break;
        case SIGKILL:
        snprintf(sign, sizeof(sign), "SIGKILL\n");
        break;
        case SIGSTOP:
        snprintf(sign, sizeof(sign), "SIGSTOP\n");
        break;
        case SIGUSR1:
        snprintf(sign, sizeof(sign), "SIGUSR1\n");
        break;
    }
    
    snprintf(event, sizeof(event), "%s%s", event, sign);

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    lockPermission();
    return 0;
}

int eventSignalSent(int signo, pid_t targetPID) {
    hasWritePermission();

    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event, sizeof(event), "%sSIGNAL_ENV ; ", event);

    char sign[20];
    switch(signo){
        case SIGINT:
        snprintf(sign, sizeof(sign), "SIGINT ; %d\n", targetPID);
        break;
        case SIGCHLD:
        snprintf(sign, sizeof(sign), "SIGCHLD; %d\n", targetPID);
        break;
        case SIGCONT:
        snprintf(sign, sizeof(sign), "SIGCONT; %d\n", targetPID);
        break;
        case SIGKILL:
        snprintf(sign, sizeof(sign), "SIGKILL; %d\n", targetPID);
        break;
        case SIGSTOP:
        snprintf(sign, sizeof(sign), "SIGSTOP; %d\n", targetPID);
        break;
        case SIGUSR1:
        snprintf(sign, sizeof(sign), "SIGUSR1; %d\n", targetPID);
        break;
    }

    snprintf(event, sizeof(event), "%s%s",event, sign);

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    lockPermission();
    return 0;
}

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode) {
    hasWritePermission();

    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;

    // Write event ; filename : oldMode : newMode
    snprintf(event, sizeof(event), "%sFILE_MODF ; %s : %#o : %#o\n", event, filename, oldMode & 0777, newMode & 0777);

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    lockPermission();
    return 0;
}


int endLogFile() {
    // Verify if the file exists
    if(logFile == -1)
        return 1;

    close(logFile);
    return 0;
}
