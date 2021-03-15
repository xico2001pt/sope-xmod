#ifndef XMOD_H
#define XMOD_H

#include <sys/types.h>

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

#endif
