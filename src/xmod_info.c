#include "xmod_info.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int fillXmodInfo(XmodInfo * xi, int argc, char * argv[]) {
    fillXmodFlags(&(xi->flags), argc, argv);  // Preencher options
    // Preencher modo
    // Preencher filename;
}

int fillXmodFlags(XmodFlags * xf, int argc, char * argv[]) {
    // iterar argv
}

int isOctal(char * mode) {

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

int isRegularMode(char* mode){
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
            char chosen[3];
            int j=0;
            for(int i = 2; i <strlen(mode);i++){
                //Vê se cada um é um r, w ou x
                if(mode[i] == 'r'||mode[i] == 'w'||mode[i] == 'x')
                {
                    for(int k = 0; k<2; k++){
                        //Verifica se já aquela letra já tinha sido utilizada (ex: impedir rrw)
                        if(chosen[k]==mode[i]){return 1;}                   
                    }
                    //Se não encontrou nada, adiciona ao chosen para verificar depois
                    chosen[j] = mode[i];
                    j++;
                }
                else{return 1;}
            }
            //Passou por todas as exceções logo é considerado regular
            return 0;

        }
        else{return 1;}
    }
    else{return 1;}
}

mode_t convertToOctal(char * mode) {
    return strtol(mode, NULL, 8);
}

int compareModes(mode_t mode1, mode_t mode2) {}
