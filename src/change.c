#include "change.h"
#include "regfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int changePermission(XmodInfo * xmodInfo){
    struct stat st;

    // Verify if the file exists
    if (stat(xmodInfo->filename, &st) != 0)
        return 1;
    
    // Changing permissions
    if (chmod(xmodInfo->filename, xmodInfo->mode) != 0)
        return 1;
    
    // Register event
    eventFileModf(xmodInfo->filename, st.st_mode, xmodInfo->mode);

    // Writing in the console the changes that took effect
    char oldMode[10], newMode[10];
    octalModeToString(st.st_mode, oldMode);
    octalModeToString(xmodInfo->mode, newMode);
    if (xmodInfo->flags.changes || xmodInfo->flags.verbose)
        printf("mode of '%s' changed from %#o (%s) to %#o (%s)\n", xmodInfo->filename, st.st_mode, oldMode, xmodInfo->mode, newMode);
    
    return 0;
}

int octalModeToString(mode_t mode, char *buf){
    // Check if space was allocated
    if (buf == NULL) return 1;

    strcpy(buf, "rwxrwxrwx");

    int temp, i = 8;
    while (i != -1){
        temp = mode & 1;
        mode >>= 1;
        if (!temp) buf[i] = '-';
        i--;
    }

    return 0;
}
