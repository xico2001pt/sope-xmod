#include "change.h"
#include "regfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int changePermission(XmodInfo * xmodInfo) {
    // Convert octal permissions to string
    char oldMode[10], newMode[10];
    octalModeToString(xmodInfo->oldMode, oldMode);
    octalModeToString(xmodInfo->mode, newMode);

    // Changing permissions
    if (chmod(xmodInfo->filename, xmodInfo->mode) != 0) {
        printf("failed to change mode of '%s' from %#o (%s) to %#o (%s)\n", xmodInfo->filename, xmodInfo->oldMode, oldMode, xmodInfo->mode, newMode);
        return 1;
    }
    
    // Register event
    eventFileModf(xmodInfo->filename, xmodInfo->oldMode, xmodInfo->mode);

    // Writing in the console the changes that took effect
    if (xmodInfo->flags.changes || xmodInfo->flags.verbose)
        printf("mode of '%s' changed from %#o (%s) to %#o (%s)\n", xmodInfo->filename, xmodInfo->oldMode, oldMode, xmodInfo->mode, newMode);
    
    return 0;
}

int octalModeToString(mode_t mode, char *buf) {
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
