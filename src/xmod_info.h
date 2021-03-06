#ifndef XMOD_INFO_H
#define XMOD_INFO_H

#include <sys/stat.h>
#include <stdbool.h>
/**
 * @brief Struct with all the info about the xmod(configs, mode and file)
 * 
 */
typedef struct {
    XmodFlags flags;
    mode_t mode;
    char * filename;
} XmodInfo;

/**
 * @brief Struct with all info about the configs(-v,-c,-R)
 * 
 */
typedef struct {
    bool recursive; // -R
    bool verbose;   // -v
    bool changes;   // -c
} XmodFlags;

/**
 * *Description*
 * Returns 0 on success and -1 in case of error
 */
int fillXmodInfo(XmodInfo * xi, int argc, char * argv[]);

/**
 * *Description*
 * Returns 0 on success and -1 in case of error
 */
int fillXmodFlags(XmodFlags * xf, int argc, char * argv[]);

/**
 * Checks if the given mode is represented in octal
 * Returns true if the given mode is represented in octal or false otherwise.
 */
bool isOctal(char * mode);

/**
 * Converts the given mode to octal mode and returns it through octalMode
 * Returns 0 on success and -1 in case of error
 */
int convertToOctal(char * mode, mode_t * octalMode);

/**
 * Checks if the given modes are the same
 * Returns true if the given modes are the same or false otherwise.
 */
bool compareModes(mode_t mode1, mode_t mode2);

#endif // XMOD_INFO_H
