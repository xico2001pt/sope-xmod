#ifndef REGFILE_H
#define REGFILE_H

#include <sys/types.h>

#define MAX_ENV_VAR_SIZE 128

static char logFilename[MAX_ENV_VAR_SIZE];

typedef enum {PROC_CREAT, PROC_EXIT, SIGNAL_RECV, SIGNAL_SENT, FILE_MODF} event;

typedef union {
    struct {
        int argc;
        char * argv[];
    };
    int exitStatus;
    struct {
        int signo;
        pid_t targetPID;
    };
    struct {
        char * filename;
        mode_t oldMode;
        mode_t newMode;
    };
} info;

int setEvent(event ev, info * in);

int eventProcCreat(int argc, char * argv[]);

int eventProcExit(int exitStatus);

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode);

#endif // REGFILE_H
