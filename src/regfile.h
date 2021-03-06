#ifndef REGFILE_H
#define REGFILE_H

#include <sys/types.h>
#include <sys/times.h>

#define MAX_ENV_VAR_SIZE 128

static char logFilename[MAX_ENV_VAR_SIZE];

int updateStart(clock_t startTime);

int registerEvent();

int eventProcCreat(int argc, char * argv[]);

int eventProcExit(int exitStatus);

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode);

#endif // REGFILE_H
