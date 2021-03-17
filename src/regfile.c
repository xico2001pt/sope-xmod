#include "regfile.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "signal.h"

/**
 * @brief LOG_FILENAME number if it exists, -1 if it doesnt
 * 
 */
static int logFile;

static clock_t startClock;

#define MAX_CHARS 300

void initClock() {
    struct tms t;
    startClock = times(&t);
}

void setClock(clock_t clock) {startClock = clock;}

clock_t getClock() {return startClock;}

int initLogFile(int firstParent) {
    char* logFilename = getenv("LOG_FILENAME");
    
    if (logFilename == NULL) {  // The path to the filename doesn't exist
        logFile = -1;
        return 1;
    }
    else {
        // Flags -> Write only, create file if doesn't exist and truncate if it does
        // Permissions -> Read and write for all users
        if (firstParent) logFile = open(logFilename, O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, S_IRGRP|S_IRUSR|S_IROTH|S_IWGRP|S_IWOTH|S_IWUSR);
        else {
            logFile = open(logFilename, O_WRONLY|O_APPEND|O_SYNC);
            //write(logFile, "\n\n", 2);
        }

        if (logFile == -1) return -1;  // Error opening the file
    }
    
    return 0;
}

int registerEvent(char * str) {
    // Verify if the file  or the string exist
    if (logFile == -1 || str == NULL)
        return 1;
    
    // Register instant and PID
    struct tms t;
    int inst = (times(&t) - startClock) * 1000 / sysconf(_SC_CLK_TCK);
    pid_t pid = getpid();
    sprintf(str, "%d ; %d ; ", inst, pid);

    return 0;
}

int eventProcCreat(int argc, char * argv[]) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;

    strcat(event, "PROC_CREAT ; ");  // After inst; pid; write event ;

    // After inst ; pid ; event ; write arg[1] arg[2] arg[3]
    for (int i = 0; i < argc; i++) {
        strcat(event, argv[i]);
        strcat(event, " ");
    }

    strcat(event, "\n");

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    return 0;
}

int eventProcExit(int exitStatus) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;

    strcat(event, "PROC_EXIT ; ");  // After inst; pid; write event ;

    // Write exit status
    char exitStr[20];
    sprintf(exitStr, "%d\n", exitStatus);
    strcat(event, exitStr);

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    return 0;
}

int eventSignalRecv(int signo) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    strcat(event, "SIGNAL_REC ; ");

    char sign[20];
    switch(signo){
        case SIGINT:
        sprintf(sign, "SIGINT\n");
        break;
        case SIGCHLD:
        sprintf(sign, "SIGCHLD\n");
        break;
        case SIGCONT:
        sprintf(sign, "SIGCONT\n");
        break;
        case SIGKILL:
        sprintf(sign, "SIGKILL\n");
        break;
        case SIGSTOP:
        sprintf(sign, "SIGSTOP\n");
        break;
        case SIGUSR1:
        sprintf(sign, "SIGUSR1\n");
        break;

    }

    strcat(event, sign);

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    return 0;
}

int eventSignalSent(int signo, pid_t targetPID) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    strcat(event, "SIGNAL_ENV ; ");

    char sign[20];
    switch(signo){
        case SIGINT:
        sprintf(sign, "SIGINT ; %d\n", targetPID);
        break;
        case SIGCHLD:
        sprintf(sign, "SIGCHLD; %d\n", targetPID);
        break;
        case SIGCONT:
        sprintf(sign, "SIGCONT; %d\n", targetPID);
        break;
        case SIGKILL:
        sprintf(sign, "SIGKILL; %d\n", targetPID);
        break;
        case SIGSTOP:
        sprintf(sign, "SIGSTOP; %d\n", targetPID);
        break;
        case SIGUSR1:
        sprintf(sign, "SIGUSR1; %d\n", targetPID);
        break;
    }

    strcat(event, sign);

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    return 0;
}

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;

    // Write event ; filename : oldMode : newMode
    char str[MAX_CHARS];
    sprintf(str, "FILE_MODF ; %s : %#o : %#o\n", filename, oldMode & 0777, newMode & 0777);

    strcat(event, str);

    lseek(logFile, 0, SEEK_END);
    write(logFile, event, strlen(event));

    return 0;
}


int endLogFile() {
    // Verify if the file exists
    if(logFile == -1)
        return 1;

    close(logFile);
    return 0;
}
