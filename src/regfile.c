#include "regfile.h"

/**
 * @brief LOG_FILENAME directory if it exists (NULL if it doesn't)
 * 
 */
static char logFilename[MAX_ENV_VAR_SIZE];

extern clock_t startClock;

int initLogFile();

int registerEvent(clock_t registration_instant, pid_t pid);

int eventProcCreat(int argc, char * argv[]);

int eventProcExit(int exitStatus);

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode);