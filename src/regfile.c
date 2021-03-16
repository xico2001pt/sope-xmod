#include "regfile.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

/**
 * @brief LOG_FILENAME file pointer if it exists, NULL if it doesnt
 * 
 */
static FILE * logFile;

static clock_t startClock;

#define MAX_CHARS 300

void initClock() {
    struct tms t;
    startClock = times(&t);
}

int initLogFile(int firstParent) {
    char* logFilename = getenv("LOG_FILENAME");
    
    if (logFilename == NULL) {  // The path to the filename doesn't exist
        logFile = NULL;
        return 1;
    }
    else {
        // Flags -> Write only, create file if doesn't exist and truncate if it does
        // Permissions -> Read and write for all users
        if (firstParent) logFile = fopen(logFilename, "w");
        else logFile = fopen(logFilename, "a");

        if (logFile == NULL) return -1;  // Error opening the file
    }
    
    return 0;
}

void setClock(clock_t clock) {
    startClock = clock;
}

clock_t getClock() {return startClock;}

int registerEvent(char * str) {
    // Verify if the file  or the string exist
    if (logFile == NULL || str == NULL)
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

    fseek(logFile, 0, SEEK_END);
    fwrite(event, sizeof(char), strlen(event), logFile);
    fflush(logFile);  // Flush output buffer

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

    fseek(logFile, 0, SEEK_END);
    fwrite(event, sizeof(char), strlen(event), logFile);
    fflush(logFile);  // Flush output buffer

    return 0;
}

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode) {
    char event[MAX_CHARS];

    if (registerEvent(event) == 1) return 1;

    // Write event ; filename : oldMode : newMode
    char str[MAX_CHARS];
    sprintf(str, "FILE_MODF ; %s : %#o : %#o\n", filename, oldMode, newMode);

    strcat(event, str);

    fseek(logFile, 0, SEEK_END);
    fwrite(event, sizeof(char), strlen(event), logFile);
    fflush(logFile);  // Flush output buffer

    return 0;
}


int endLogFile() {
    // Verify if the file exists
    if(logFile == NULL)
        return 1;

    fclose(logFile);
    return 0;
}
