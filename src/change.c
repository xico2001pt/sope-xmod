#include "change.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include "signal.h"
#include "regfile.h"

pid_t childProcesses[100];
int numberOfChildren = 0;
int nftot = 0;
int nfmod = 0;
char* filename;

int changePermission(XmodInfo * xmodInfo) {
    // Convert octal permissions to string
    char oldMode[BUF_SIZE], newMode[BUF_SIZE];
    octalModeToString(xmodInfo->oldMode, oldMode);
    octalModeToString(xmodInfo->mode, newMode);

    // Changing permissions
    if (chmod(xmodInfo->filename, xmodInfo->mode) != 0) {
        printf("failed to change mode of '%s' from %#o (%s) to %#o (%s)\n", xmodInfo->filename, xmodInfo->oldMode & 0777, oldMode,  xmodInfo->mode& 0777, newMode);
        return 1;
    }
    

    // Register event
    eventFileModf(xmodInfo->filename, xmodInfo->oldMode, xmodInfo->mode);

    // Writing in the console the changes that took effect
    if ((xmodInfo->flags.verbose || xmodInfo->flags.changes) && ((xmodInfo->oldMode & 0777) != xmodInfo->mode))
        printf("mode of '%s' changed from %#o (%s) to %#o (%s)\n", xmodInfo->filename, xmodInfo->oldMode & 0777, oldMode, xmodInfo->mode &0777, newMode);
    else if (xmodInfo->flags.verbose && ((xmodInfo->oldMode & 0777) == xmodInfo->mode))
        printf("mode of '%s' retained as %#o (%s)\n", xmodInfo->filename, xmodInfo->oldMode & 0777, oldMode);
    return 0;
}

int changePermissionRecursive(XmodInfo * xmodInfo, int argc, char * argv[]) {
    filename = argv[argc-1];
    
    nftot++;
    // Change file/dir permission
    if (changePermission(xmodInfo) != 0) return 1;
    nfmod++;

    // Verify if it is a directory
    if (!S_ISDIR(xmodInfo->oldMode)) return 0;

    DIR *dp = opendir(xmodInfo->filename);
    struct dirent *files;
    char path[400];
    struct stat buf;
    XmodInfo subFile;
    int status;


    // Change permissions for each file/dir inside
    while ((files = readdir(dp)) != NULL) {
        //sleep(1);
        if (strcmp(files->d_name, ".") != 0 && strcmp(files->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", xmodInfo->filename, files->d_name);
            if (lstat(path, &buf) == -1) {
                perror("stat()");
                return 1;
            }
            nftot++;
            if (S_ISDIR(buf.st_mode)) {
                pid_t pid;
                
                if ((pid = fork()) < 0) {
                    perror("fork()");
                    return 1;
                } else if (pid == 0) {  // Child
                    //char * filecopy = argv[argc - 1];
                    argv[argc - 1] = path;
                    execv(argv[0], argv);
                    //argv[argc - 1] = filecopy;
                    return 0;
                } else {  // Parent
                    childProcesses[numberOfChildren] = pid;
                    numberOfChildren++;
                    nfmod++;
                }
            } else if (S_ISLNK(buf.st_mode)) {
                if (xmodInfo->flags.verbose){ printf("neither symbolic link '%s' nor referent has been changed\n", path);}
            } else {
                copyXmodInfo(&subFile, xmodInfo);
                subFile.filename = path;
                subFile.oldMode = buf.st_mode;
                changePermission(&subFile);
                nfmod++;
            }
        }
    }

    // Wait for children
    for (int i = 0; i < numberOfChildren; ++i) waitpid(childProcesses[i], &status, 0);

    return 0;
}

int initProcess() {
    char *buf, value[MAX_CHARS];

    if ((buf = getenv("XMOD_PARENT")) == NULL) {
        snprintf(value, MAX_CHARS, "%ld", getClock());
        setenv("XMOD_PARENT", value, 0);
        return 1;
    } else {
        clock_t clock;
        sscanf(buf, "%ld", &clock);
        setClock(clock);
        return 0;
    }
}

int octalModeToString(mode_t mode, char * buf) {
    // Check if space was allocated
    if (buf == NULL) return 1;

    snprintf(buf, BUF_SIZE, "rwxrwxrwx");

    int temp, i = 8;
    while (i != -1){
        temp = mode & 1;
        mode >>= 1;
        if (!temp) buf[i] = '-';
        i--;
    }

    return 0;
}
