#include "xmod_info.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

int fillXmodInfo(XmodInfo * xi, int argc, char * argv[]) {
    // Fill flags/options
    if (fillXmodFlags(&(xi->flags), argc, argv) != 0) {
        printf("xmod: invalid options\n");
        return 1;
    }

    // Verify if the file exists
    if (checkFileStatus(argv[argc-1], xi) != 0) {
        printf("xmod: cannot access '%s' : No such file or directory\n", argv[argc-1]);
        if (xi->flags.verbose) printf("failed to change mode of '%s' from 0000 (---------) to 0000 (---------)\n", argv[argc-1]);
        return 1;
    }

    // Fill mode
    mode_t mode = convertToOctal(argv[argc-2], argv[argc-1], xi->oldMode);
    if (mode == -1) {
        printf("xmod: invalid mode: '%s'\n", argv[argc-2]);
        return 1;
    }
    xi->mode = mode;

    return 0;
}

int copyXmodInfo(XmodInfo * dest, XmodInfo * src) {
    if (dest == NULL || src == NULL) return 1;

    dest->flags = src->flags;
    dest->mode = src->mode;
    dest->oldMode = src->oldMode;
    dest->filename = src->filename;

    return 0;
}

int fillXmodFlags(XmodFlags * xf, int argc, char * argv[]) {
    // Initialize struct
    memset(xf, 0, sizeof(*xf));

    // Iterate every flag
    for (int i = 1; i < argc - 2; ++i) {
        if (strcmp(argv[i], "-R") == 0) {
            if (xf->recursive)
                return 1;
            xf->recursive = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            if (xf->verbose)
                return 1;
            xf->verbose = true;
        } else if (strcmp(argv[i], "-c") == 0) {
            if (xf->changes)
                return 1;
            xf->changes = true;
        }
        else return 1;
    }
    return 0;
}

int checkFileStatus(char* filename, XmodInfo * xf) {
    struct stat st;

    // Verify if the file exists
    if (stat(filename, &st) != 0)
        return 1;

    // Fill filename struct information
    xf->filename = filename;

    // Fill oldMode struct information
    xf->oldMode = st.st_mode;

    return 0;
}

int checkOctal(char * mode) {
    // If the size of the string is different than 4, it is not octal
    if (strlen(mode) != 4) return 1;      

    // The first character must be a 0
    if (mode[0] != '0') return 1;

    // Verify if any character is greater than 7
    for (int i = 0; i < strlen(mode) ; i++) {
        if(mode[i] > '7') return 1;
    }
    return 0;
}

int checkRegularMode(char* mode) {
    // If the size of the string is smaller than 3 or greater than 5, it is not regular
    if (strlen(mode) < 3 || strlen(mode) > 5) return 1;

    // Check if the first character is 'u', 'g', 'o' or 'a'
    if (mode[0] == 'u' || mode[0] == 'g' || mode[0] == 'o' || mode[0] == 'a') {
        // Check if the second character is '+', '-' or '='
        if (mode[1] == '+' || mode[1] == '-' || mode[1] == '=') {
            switch (strlen(mode)-2){
                case 1:  // Must be one of the characters
                if (mode[2] != 'r' && mode[2] != 'w' && mode[2] != 'x') return 1;
                break;

                case 2:  // Contains two characters
                // Starts with 'r'
                if (mode[2] == 'r') {
                    if (mode[3] != 'w' && mode[3] != 'x') return 1;
                }

                //Starts with 'w'
                else if (mode[2] == 'w' && mode[3] != 'x') return 1;

                // Starts with neither of these two
                else return 1;
                break;

                case 3:  // Contains all characters
                if (mode[2] != 'r' || mode[3] != 'w' || mode[4] != 'x') return 1;
                break;
            }
            return 0;
        }
        else return 1;
    }
    else return 1;
}


mode_t convertToOctal(char * mode, char * filename, mode_t oldMode) {
    if (checkOctal(mode) == 0) {  // Check if string is in octal mode
        return strtol(mode, NULL, 8);
    } else if (checkRegularMode(mode) == 0) {  // Check if string is in regular mode
        // Initialize variable
        mode_t octalMode = 0;

        // Assemble given mode
        for (int i = 2; i <strlen(mode);i++) {
            if (mode[i] == 'r') {
                if (mode[0] == 'u' || mode[0] == 'a')
                    octalMode |= S_IRUSR;   // Read permission for user
                if (mode[0] == 'g' || mode[0] == 'a')
                    octalMode |= S_IRGRP;   // Read permission for user group
                if (mode[0] == 'o' || mode[0] == 'a')
                    octalMode |= S_IROTH;   // Read permission for other users
            } else if (mode[i] == 'w') {
                if (mode[0] == 'u' || mode[0] == 'a')
                    octalMode |= S_IWUSR;   // Write permission for user
                if (mode[0] == 'g' || mode[0] == 'a')
                    octalMode |= S_IWGRP;   // Write permission for user group
                if (mode[0] == 'o' || mode[0] == 'a')
                    octalMode |= S_IWOTH;   // Write permission for other users
            } else if (mode[i] == 'x') {
                if (mode[0] == 'u' || mode[0] == 'a')
                    octalMode |= S_IXUSR;   // Execution permission for user
                if (mode[0] == 'g' || mode[0] == 'a')
                    octalMode |= S_IXGRP;   // Execution permission for group
                if (mode[0] == 'o' || mode[0] == 'a')
                    octalMode |= S_IXOTH;   // Execution permission for other users
            }
        }

        // If substitution symbol, then return octal mode
        if (mode[1] == '=') return octalMode;

        // Merge given permission with file permission
        if (mode[1] == '+') {
            octalMode |= oldMode;
        } else if (mode[1] == '-') {
            octalMode = ~octalMode & oldMode;
        }
        return octalMode;
    }
    else return -1;
}
