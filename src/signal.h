#ifndef SIGNAL_H
#define SIGNAL_H

#include <signal.h>

/**
 * @brief Handler so sinal CTRL+C de interrupção. 
 * Este terá de mandar a mensagem e depois enviar o sinal aos seus filhos.
 * 
 * @param signo SIGINT
 */
void signSIGINTHandler(int signo);

/**
 * @brief Handler do sinal CONT que permite enviar o cont para os filhos
 * 
 * @param signo SIGCONT
 */
void sighandlerSIGCONT(int signo);

/**
 * @brief Handler do SIGUSR1 que nos permitirá enviar aos filhos o sinal de abort
 * 
 * @param signo SIGUSR1
 */
void sigHandlerSIGUSR1(int signo);

/**
 * @brief Handler do SIGCHILD que permite o registo do sinal recebido
 * 
 * @param signo SIGCHLD
 */
void sigHandlerSIGCHILD(int signo);

/**
 * @brief Função que determina como o pai vai responder ao utilizadr
 * 
 * @param answer "1"->Para o programa ou "2"->Continua o programa
 * @return int -1 se deu erro ou se a resposta é inválida ou 0 se correu bem
 */
int signSIGINTAnwserHandler(char* answer);



#endif //SIGNAL_H