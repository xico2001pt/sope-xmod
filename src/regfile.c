#include "regfile.h"

int setEvent(event ev, info * in);

int eventProcCreat(int argc, char * argv[]);

int eventProcExit(int exitStatus);

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode);