#include "xmod_info.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>


int fillXmodInfo(XmodInfo * xi, int argc, char * argv[]) {
    // Preencher flags/options
    fillXmodFlags(&(xi->flags), argc, argv);

    // Preencher modo
    mode_t mode = convertToOctal(argv[argc-2], argv[argc-1]);
    if (mode == -1) {
        printf("fillXmodInfo: Invalid Mode Format\n");
        return 1;
    }
    xi->mode = mode;

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
        }
        else if (strcmp(argv[i], "-v") == 0) {
            if (xf->verbose)
                return 1;
            xf->verbose = true;
        }
        else if (strcmp(argv[i], "-c") == 0) {
            if (xf->changes)
                return 1;
            xf->changes = true;
        }
        else return 1;
    }
    return 0;
}

int checkFileStatus(char* filename, XmodInfo * xf){
    struct stat st;

    //Verifica se o ficheiro existe
    if (stat(filename, &st) != 0)
        return -1;

    //Define na struct o nome do ficheiro
    xf->filename = filename;
    //Define na struct o old_mode do ficheiro
    xf->old_mode = st.st_mode;

    return 0;
}

int checkOctal(char * mode) {

     if(strlen(mode)!=4){
        //Se a string tiver mais de 4 carateres, não é octal
        return 1;      
     }
     
     //No octal, o primeiro carater é um 0
     if(mode[0] != '0')
     {
        return 1;
     }

     //ir a cada um dos carateres e verificar se algum deles é maior que 7;
     for(int i = 0; i<strlen(mode);i++){
        if(mode[i]>'7'){return 1;}
     }

     //Se passa todos os testes, é considerado octal
     return 0;
     
}

int checkRegularMode(char* mode){
    
    if(strlen(mode)<3 || strlen(mode)>5){
    // se o tamanho da string for menor que 3 e maior que 5, não é regular
        printf("Falhei no tamanho\n");
        return 1;
    }

    //Verifica se o primeiro carater é um u, g, o ou a
    if(mode[0]=='u'||mode[0]=='g'||mode[0]=='o'||mode[0] == 'a')
    {
        //Verifica se o segundo é
        if(mode[1]=='+'||mode[1]=='-'|| mode[1] == '=')
        {
            switch (strlen(mode)-2){
                case 1:
                //No caso de ter apenas 1, se não for um nenhum deles retorna 1
                if(mode[2] != 'r'&&mode[2] != 'w' && mode[2] != 'x'){return 1;}
                break;

                case 2:
                //No caso de ter dois
                //começar com r
                if(mode[2]=='r'){
                    if(mode[3]!='w'&&mode[3]!='x'){return 1;}
                }
                //começar com w
                else if (mode[2]=='w' && mode[3]!='x'){return 1;}
                //começar com nenhuim dos dois
                else{ return 1;}
                break;

                case 3:
                //No caso de ter os tres
                if(mode[2]!='r'||mode[3]!='w'||mode[4]!='x'){return 1;}
                break;
            }
            //Passou por todas as exceções logo é considerado regular
            return 0;

        }
        else{return 1;}
    }
    else{return 1;}
}


mode_t convertToOctal(char * mode, char * filename) {
    if (checkOctal(mode) == 0)  // String in octal mode
        return strtol(mode, NULL, 8);
    else if (checkRegularMode(mode) == 0) {  // String in regular mode
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
            }
            else if (mode[i] == 'w') {
                if (mode[0] == 'u' || mode[0] == 'a')
                    octalMode |= S_IWUSR;   // Write permission for user
                if (mode[0] == 'g' || mode[0] == 'a')
                    octalMode |= S_IWGRP;   // Write permission for user group
                if (mode[0] == 'o' || mode[0] == 'a')
                    octalMode |= S_IWOTH;   // Write permission for other users
            }
            else if (mode[i] == 'x') {
                if (mode[0] == 'u' || mode[0] == 'a')
                    octalMode |= S_IXUSR;   // Execution permission for user
                if (mode[0] == 'g' || mode[0] == 'a')
                    octalMode |= S_IXGRP;   // Execution permission for group
                if (mode[0] == 'o' || mode[0] == 'a')
                    octalMode |= S_IXOTH;   // Execution permission for other users
            }
        }

        // If update symbol, then return octal mode
        if (mode[1] == '=') return octalMode;

        // Merge given permission with file permission
        struct stat buf;
        if (stat(filename, &buf) != 0) return -1;

        if (mode[1] == '+') {
            octalMode |= buf.st_mode;
        }
        else if (mode[1] == '-') {
            octalMode = ~octalMode & buf.st_mode;
        }
        return octalMode;
    }
    else return -1;
}


