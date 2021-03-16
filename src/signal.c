#include "signal.h"
#include "change.h"
#include <wait.h>
#include "regfile.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * 1º-> funções de reigisto de sinais
 * 2ª-> aplicar os signhandlers no xmod.c
 * 3ª-> aplicar o sinal de fim de fil de processo filho
 * 
 */

int signSIGINTAnwserHandler(char* answer){
    //Se for igual a 1, vai mandar todos os procesos abortarem
    if(strcmp(answer, "1")){
        for(int i = 0; i<numberOfChildren; i++){
            kill(childProcesses[i], SIGUSR1);
            eventSignalSent(SIGUSR1, childProcesses[i]);
        }
        //Espera que os filhos todos morram antes de morrer
        for(int i = 0; i<numberOfChildren; i++){
            int status;
            waitpid(childProcesses[i], &status,1 );
        }

        endLogFile();
        write(STDOUT_FILENO, "The program has been cancelled!\n", 33);

        //Levanta o kill e regista tudo
        eventSignalSent(SIGKILL, getpid());   //Envio que vou me matar
        eventSignalRecv(SIGKILL);     //Recebo que me vou matar
        eventProcExit(0);    //Aviso que me matei
        raise(SIGKILL);     //Mato-me
        return 0; 

    }

    else if(strcmp(answer, "2")){
        for(int i = 0; i<numberOfChildren; i++){
            eventSignalSent(SIGCONT, childProcesses[i]);
            kill(childProcesses[i], SIGCONT);
        }

        write(STDOUT_FILENO, "The program has been continued!\n", 33);

        return 0;

    }

    else{return -1;}
}



static void signSIGINTHandler(int signo){
    eventSignalRecv(SIGINT);
    //Escrever a mensagem
    char buffer[50];

    sprintf(buffer, "%d; %s; %d; %d\n", getpid(), filename, nftot, nfmod);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    //Loop pelos fihos do processo para enviar o sinal
    for (int i = 0; i < numberOfChildren; i++) {
        kill(childProcesses[i], SIGINT);
        eventSignalSent(SIGINT, childProcesses[i]);
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
        eventSignalSent(SIGSTOP, getpid());
        eventSignalRecv(SIGSTOP);
        raise(SIGSTOP);
    }   
}


static void sighandlerSIGCONT(int signo){
    //Regist que recebeu o sinal
    eventSignalRecv(SIGCONT);

    for (int i = 0; i < numberOfChildren; i++) {
        //envia o sigcont aos filhos e regista
        kill(childProcesses[i], SIGCONT;
        eventSignalSent(SIGCONT, childProcesses[i]);
    }

}

static void sigHandlerSIGUSR1(int signo){
    eventSignalRecv(SIGUSR1);

    for(int i = 0; i<numberOfChildren; i++){
        kill(childProcesses[i], SIGUSR1);
        eventSignalSent(SIGUSR1, childProcesses[i]);
    }

    eventSignalSent(SIGKILL, getpid());
    eventSignalRecv(SIGKILL);
    eventSignalSent(SIGCHLD, getppid());    //Aviso o paizinho que me vou matar
    eventProcExit(0);
    raise(SIGKILL);
    
    
    
}

static void sigHandlerSIGCHLD(int signo){
    eventSignalRecv(SIGCHLD);
    return 0;
}

