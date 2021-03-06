#include "regfile.h"

static clock_t start;

int updateStart(clock_t startTime) {
    start = startTime;
}

int setEvent();

int eventProcCreat(int argc, char * argv[]);

int eventProcExit(int exitStatus);

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode);