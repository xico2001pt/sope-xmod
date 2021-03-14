#ifndef XMOD_H
#define XMOD_H

#include <sys/types.h>

#define MAX_BUFFER 50

/**
 * @brief Nº de ficheiros encontrados até ao momento
 * 
 */
extern int nftot;

/**
 * @brief Nº de ficheiros modificados até ao momento
 * 
 */
extern int nfmod;

/**
 * @brief Diretório do ficheiro ou da pasta
 * 
 */
extern char* filename;

/**
 * @brief Array com o pid de cada filho do processo
 * 
 */
extern pid_t childProcesses[MAX_BUFFER];

/**
 * @brief Tamanho do array childProcesses
 * 
 */
extern int numberOfChildren;

/**
 * @brief Variável que indica se é ou não o processo pai
 * 
 */
extern int isParent;





#endif