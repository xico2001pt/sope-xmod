#ifndef SIGNAL_H
#define SIGNAL_H

#include <signal.h>

/**
 * @brief Handler so sinal CTRL+C de interrupção. 
 * Este terá de mandar a mensagem e depois enviar o sinal aos seus filhos.
 * 
 * @param signo SIGINT
 */
static void signSIGINTHandler(int signo);

static void signSIGCONTHandler(int signo);

static void signSIGSTOPHandler(int signo);

static void signSIGABRTHandler(int signo);

/**
 * @brief Função que determina como o pai vai responder ao utilizadr
 * 
 * @param answer "Yes" ->Continua ou "No" ->
 * @return int 
 */
int signSIGINTAnwserHandler(char* answer);



#endif //SIGNAL_H