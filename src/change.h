#ifndef CHANGE_H
#define CHANGE_H

#include "xmod_info.h"
/**
 * @brief Changes the permission of the file/dir based on the new permissions
 * 
 * @param xmodInfo Struct containing all the info about permission changes
 * @return int Returns 1 if not success and 0 if success
 */
int changePermission(XmodInfo * xmodInfo);

#endif // CHANGE_H
