#include "signal.h"
#include "xmod.h"
#include <wait.h>
#include "regfile.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>



int signSIGINTAnwserHandler(char* answer){
    //Se for igual a 1, vai mandar todos os procesos abortarem
    if(strcmp(answer, "1")){
        for(int i = 0; i<numberOfChildren; i++){
            kill(childProcesses[i], SIGKILL);
        }

        endLogFile();
        write(STDOUT_FILENO, "The program has been cancelled!\n", 33);
        raise(SIGKILL);
        return 0;

    }

    else if(strcmp(answer, "2")){
        for(int i = 0; i<numberOfChildren; i++){
            kill(childProcesses[i], SIGCONT);
        }

        write(STDOUT_FILENO, "The program has been continued!\n", 33);

        return 0;

    }

    else{return -1;}
}



static void signSIGINTHandler(int signo){
    //Escrever a mensagem
    char buffer[50];

    sprintf(buffer, "%d; %s; %d; %d\n", getpid(), filename, nftot, nfmod);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    //Loop pelos fihos do processo para enviar o sinal
    for (int i = 0; i < numberOfChildren; i++) {
        kill(childProcesses[i], SIGINT);
    }

    //Se for pai:
    if (isFirstParent) {
        //Maneira de esperar que os filhos escrevam tudo
        sleep(5);


        write(STDOUT_FILENO,"Do you want to stop the program(1) or do you prefer to continue(2)?\t", 69);
    mauInput:
        read(STDIN_FILENO, buffer, 1);
        if(signSIGINTAnwserHandler(buffer)==-1){
            write(STDOUT_FILENO,"Please insert '1' to stop or '2' to continue!\t", 47);
            goto mauInput;
        }
        
    }
    else
    {
         //senão entra em pausa
        raise(SIGSTOP);
    }   
}

