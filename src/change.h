#ifndef CHANGE_H
#define CHANGE_H

#include "xmod_info.h"
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief Array com o pid de cada filho do processo
 * 
 */
extern pid_t childProcesses[100];

/**
 * @brief Tamanho do array childProcesses
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

// TODO: alterar
/**
 * @brief Changes the permission of the file/dir based on the new permissions
 * 
 * @param xmodInfo Struct containing all the info about permission changes
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

#endif // CHANGE_H
