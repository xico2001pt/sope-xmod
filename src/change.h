#ifndef SRC_CHANGE_H_
#define SRC_CHANGE_H_

#include <sys/types.h>
#include <unistd.h>
#include "xmod_info.h"

/**
 * @brief Nº de ficheiros encontrados
 * 
 */
extern int nftot;

/**
 * @brief Number of files changed
 * 
 */
extern int nfmod;

/**
 * @brief Main folder address
 * 
 */
extern char* filename;

/**
 * @brief Array with the pid of the child processes
 * 
 */
extern pid_t childProcesses[100];

/**
 * @brief Size of the childProcesses array
 * 
 */
extern int numberOfChildren;

/**
 * @brief Changes the permission of the file/dir based on the new permissions
 * 
 * @param xmodInfo Struct containing all the info about permission changes
 * @return int Returns 1 if not success and 0 if success
 */
int changePermission(XmodInfo * xmodInfo);

/**
 * @brief Changes the permission of the folder and its files when its running in recursive mode
 * 
 * @param xmodInfo Struct containing all the info about permission changes
 * @param argc Number of arguments passed
 * @param argv Arguments passed when calling xmod
 * @return int Returns 1 if not success and 0 if success
 */
int changePermissionRecursive(XmodInfo * xmodInfo, int argc, char * argv[]);

/**
 * @brief Changes the permissions from octal to the regular mode
 * 
 * @param mode Permissions in octal
 * @param buf String that will be populated by the permissions in the regular mode
 * @return int Returns 1 if not success and 0 if success
 */
int octalModeToString(mode_t mode, char *buf);

/**
 * @brief Changes the permissions from octal to the regular mode
 * 
 * @param mode Permissions in octal
 * @param buf String that will be populated by the permissions in the regular mode
 * @return int Returns 1 if not success and 0 if success
 */
int initProcess();

#endif // SRC_CHANGE_H_
