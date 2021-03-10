#include "change.h"
#include "regfile.h"
#include <sys/types.h>
#include <sys/stat.h>

int changePermission(XmodInfo * xmodInfo){
    struct stat st;
    if (stat(xmodInfo->filename, &st) != 0)
        return 1;
    if (chmod(xmodInfo->filename, xmodInfo->mode) != 0)
        return 1;
    eventFileModf(xmodInfo->filename, st.st_mode, xmodInfo->mode);
    char oldMode[10];
    char newMode[10];
    octalModeToString(st.st_mode, oldMode);
    octalModeToString(xmodInfo->mode, newMode);
    if (xmodInfo->flags.changes || xmodInfo->flags.verbose){
        printf("mode of '%s' changed from %#o (%s) to %#o (%s)/n", xmodInfo->filename, st.st_mode, oldMode, xmodInfo->mode, newMode);
    }
    return 0;
}