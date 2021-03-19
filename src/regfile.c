#include "regfile.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "./signal.h"

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
    
    
    if (logFilename == NULL) {  // The path to the filename doesn't exist
        logFile = -1;
        return 1;
    } else {
        // Flags -> Write only, create file if doesn't exist and truncate if it does
        // Permissions -> Read and write for all users
        if (firstParent) logFile = open(logFilename, O_WRONLY|O_CREAT|O_TRUNC|O_SYNC|O_APPEND, S_IRGRP|S_IRUSR|S_IROTH|S_IWGRP|S_IWOTH|S_IWUSR);
        else logFile = open(logFilename, O_WRONLY|O_APPEND|O_SYNC);

        if (logFile == -1) return -1;  // Error opening the file
    }
    
    return 0;
}

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
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event + strlen(event), MAX_CHARS, "PROC_CREAT ; "); // After inst; pid; write event ;

    // After inst ; pid ; event ; write arg[1] arg[2] arg[3] ...
    for (int i = 0; i < argc; i++)
        snprintf(event + strlen(event), MAX_CHARS, "%s ", argv[i]);

    snprintf(event + strlen(event), MAX_CHARS, "\n");
    
    write(logFile, event, strlen(event));

    return 0;
}

int eventProcExit(int exitStatus) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event + strlen(event), MAX_CHARS, "PROC_EXIT ; %d\n", exitStatus);  // After inst; pid; write event ; Write exit status

    write(logFile, event, strlen(event));

    return 0;
}

int eventSignalRecv(int signo) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event + strlen(event), MAX_CHARS, "SIGNAL_REC ; ");

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
    
    snprintf(event + strlen(event), MAX_CHARS, "%s", sign);

    write(logFile, event, strlen(event));
   
    return 0;
}

int eventSignalSent(int signo, pid_t targetPID) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;
    snprintf(event + strlen(event), MAX_CHARS, "SIGNAL_ENV ; ");

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

    snprintf(event + strlen(event), MAX_CHARS, "%s", sign);

    write(logFile, event, strlen(event));

    return 0;
}

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;

    // Write event ; filename : oldMode : newMode
    snprintf(event + strlen(event), MAX_CHARS, "FILE_MODF ; %s : %#o : %#o\n", filename, oldMode & 0777, newMode & 0777);

    //lseek(logFile, 0, SEEK_END);
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
