#include "change.h"
#include "regfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t childProcesses[100];
int numberOfChildren = 0;

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

int changePermissionRecursive(XmodInfo * xmodInfo, int argc, char * argv[]) {

    // Para cada fich/pasta dentro da pasta:
        // Alterar xmodInfo
        // Se for pasta -> fazer fork() e execv("./xmod", argv);
        // Senão, -> alterar xmodinfo changePermission

    // Change file/dir permission
    if (changePermission(xmodInfo) != 0) return 1;

    // Verify if it is a directory
    if (!S_ISDIR(xmodInfo->oldMode)) return 0;

    DIR *dp = opendir(xmodInfo->filename);
    struct dirent *files;
    char path[300];
    struct stat buf;
    int found = 0;

    // Change permissions for each file/dir inside
    while ((files = readdir(dp)) != NULL)
    {
        if (found >= 2)
        {
            sprintf(path, "%s/%s", xmodInfo->filename, files->d_name);

            if (stat(path, &buf) == -1) {
                perror("stat()");
                return 1;
            }

            if (S_ISDIR(buf.st_mode))
            {
                pid_t pid;
                
                if ((pid = fork()) < 0) {
                    perror("fork()");
                    return 1;
                }
                else if (pid == 0) {  // Child
                    char * filecopy = argv[argc - 1];
                    argv[argc - 1] = path;
                    execv(argv[0], argv);
                    argv[argc - 1] = filecopy;

                }
                else {  // Father
                    childProcesses[numberOfChildren] = pid;
                    numberOfChildren++;
                }
            }
            else
            {
                XmodInfo subFile;
                copyXmodInfo(&subFile, xmodInfo);
                subFile.filename = path;
                subFile.oldMode = buf.st_mode;
                changePermission(&subFile);
            }
        }

        found++;
    }

    // Wait for children
    int status;
    for (int i = 0; i < numberOfChildren; ++i) waitpid(childProcesses[i], &status, 0);

    return 0;
}

int initProcess() {
    char *buf, value[50];

    if ((buf = getenv("XMOD_PARENT")) == NULL) {
        sprintf(value, "%ld", getClock());
        setenv("XMOD_PARENT", value, 0);
        return 1;
    }
    else {
        clock_t clock;
        sscanf(buf, "%ld", &clock);
        setClock(clock);
        return 0;
    }
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
