#ifndef SRC_REGFILE_H_
#define SRC_REGFILE_H_

#include <sys/types.h>
#include <sys/times.h>

#define EXIT(exitStatus) ({eventProcExit(exitStatus); endLogFile(); return exitStatus;})  // Register process termination and return

#define MAX_CHARS 300 // Max number of file chars

/**
 * @brief Initializes the clock
 */
void initClock();

/**
 * @brief Initializes the log filename variable
 * 
 * @return int Returns -1 if it has an error on opening, 1 if it doesnt exist and 0 if it had no problems
 */
int initLogFile();

/**
 * @brief Initializes the global variable
 */
void setClock(clock_t clock);

/**
 * @brief Get clock
 * 
 * @return clock_t Returns the clock
 */
clock_t getClock();

/**
 * @brief Function that register the starting info of any event(instant and pid)
 * 
 * @param str String that will receive the event text
 * @return int Returns 1 if it ends in error and 0 if it goes all right
 */
int registerEvent(char * str);

/**
 * @brief Function that registers the Proccess creation event on the LOG_FILENAME
 * 
 * @param argc Number of arguments
 * @param argv Arguments passed by the command
 * @return int Returns 1 if it ends in error and 0 if it goes all right
 */
int eventProcCreat(int argc, char * argv[]);

/**
 * @brief Function that registers the end of a proccess on the LOG_FILENAME
 * 
 * @param exitStatus Registers if the proccess ended correctly or not
 * @return int Returns 1 if it ends in error and 0 if it goes all right
 */
int eventProcExit(int exitStatus);

/**
 * @brief Function that registers the signal received by a proccess on the LOG_FILENAME
 * 
 * @param signo Sinal received
 * @return int Returns 1 if it ends in error and 0 if it goes all right
 */
int eventSignalRecv(int signo);

/**
 * @brief Function that registers the signal sent by a proccess to the targetProcess
 * 
 * @param signo Signal that was sent
 * @param targetPID The proccess target PID
 * @return int Returns 1 if it ends in error and 0 if it goes all right
 */
int eventSignalSent(int signo, pid_t targetPID);

/**
 * @brief Function that registers the modification of the permissions of a file/dir
 * 
 * @param filename file/dir that was changed
 * @param oldMode Old permissions in octal
 * @param newMode New permissions in octal
 * @return int Returns 1 if it ends in error and 0 if it goes all right
 */
int eventFileModf(char * filename, mode_t oldMode, mode_t newMode);

/**
 * @brief Closes the LogFile
 * 
 * @return int Returns 0 if it goes well
 */
int endLogFile();

#endif // SRC_REGFILE_H_
