#ifndef SRC_SIGNAL_H_
#define SRC_SIGNAL_H_

#include <signal.h>

/**
 * @brief Handler of the CTRL+C interruption signal 
 * This will have to send the message after sending the signal to the children
 * 
 * @param signo SIGINT
 */
void signSIGINTHandler(int signo);

/**
 * @brief Handler of the CONT signal, that allows us to send CONT to the children
 * 
 * @param signo SIGCONT
 */
void sighandlerSIGCONT(int signo);

/**
 * @brief Handler of the SIGUSR1 that allows us to send KILL to the children
 * 
 * @param signo SIGUSR1
 */
void sigHandlerSIGUSR1(int signo);

/**
 * @brief Handler of the SIGCHILD allowing us to register the signal
 * 
 * @param signo SIGCHLD
 */
void sigHandlerSIGCHILD(int signo);

/**
 * @brief Function that determines how the first process will respond to the user
 * 
 * @param answer "1" -> Stops the program or "2" -> Continues the program
 * @return int -1 if error or invalid answer, 0 if everything's ok
 */
int signSIGINTAnwserHandler(char* answer);



#endif // SRC_SIGNAL_H_