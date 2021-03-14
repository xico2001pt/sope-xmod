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
 * @param answer "1"->Para o programa ou "2"->Continua o programa
 * @return int -1 se deu erro ou se a resposta é inválida ou 0 se correu bem
 */
int signSIGINTAnwserHandler(char* answer);



#endif //SIGNAL_H