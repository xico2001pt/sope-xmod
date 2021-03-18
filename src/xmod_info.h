#ifndef SRC_XMOD_INFO_H_
#define SRC_XMOD_INFO_H_

#include <sys/stat.h>
#include <stdbool.h>

#define MIN_NUM_ARGS 3  // Minimum number of console arguments
#define MAX_NUM_ARGS 6  // Maximum number of console arguments

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
 * @brief Struct with all the info about the xmod(configs, mode and file)
 * 
 */
typedef struct {
    XmodFlags flags;
    mode_t oldMode;
    mode_t mode;
    char * filename;
} XmodInfo;

/**
 * @brief Fills the given XmodInfo struct according to the given arguments
 * 
 * @param argc Number of arguments
 * @param argv Arguments passed by the command
 * @return int Returns 0 on success and 1 in case of error
 */
int fillXmodInfo(XmodInfo * xi, int argc, char * argv[]);

/**
 * @brief Copies the src XmodInfo values to dest
 * 
 * @param dest XmodInfo struct to be copied to
 * @param src XmodInfo struct to copy from
 * @return int Returns 0 on success and 1 in case of error
 */
int copyXmodInfo(XmodInfo * dest, XmodInfo * src);

/**
 * @brief Fills the given XmodFlags struct according to the given arguments
 * 
 * @param argc Number of arguments
 * @param argv Arguments passed by the command
 * @return int Returns 0 on success and 1 in case of error
 */
int fillXmodFlags(XmodFlags * xf, int argc, char * argv[]);

/**
 * @brief Checks if the file exists and writes the old permissions mode in the XmodInfo struct and the filename
 * 
 * @param filename The filename of the file/dir we want to change permissions
 * @param xf XmodInfo struct
 * @return int Returns 0 if it ends with no problems and 1 if the file does not exist
 */
int checkFileStatus(char* filename, XmodInfo * xf);

/**
 * @brief Checks if the given mode is represented in octal
 * 
 * @param mode String with permissions
 * @return int Returns 0 if the given mode is represented in octal, 1 if it is not
 */
int checkOctal(char * mode);

/**
 * @brief Checks if the given mode is the regular mode
 * 
 * @param mode String with permissions
 * @return int If the string has the format "<u|g|o|a><+|-|=>[rmx]" returns 0 and if it does not have that format, returns 1
 */
int checkRegularMode(char* mode);

/**
 * @brief Converts the given string mode to octal mode
 * 
 * @param mode String with permissions
 * @param filename String with path to the file
 * @param oldMode Old permissions of the file in octal
 * @return mode_t Returs the octal form of the mode or -1 in case of error
 */
mode_t convertToOctal(char * mode, char * filename, mode_t oldMode);

/**
 * @brief Checks if the given modes are the same
 * 
 * @param mode1 Permissions in octal
 * @param mode2 Permissions in octal
 * @return bool Returns 0 if the given modes are the same or 1 otherwise.
 */
int compareModes(mode_t mode1, mode_t mode2);

#endif // SRC_XMOD_INFO_H_
