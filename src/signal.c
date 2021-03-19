#include "signal.h"
#include <wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "./xmod.h"
#include "./change.h"
#include "./regfile.h"

/**
 * 1º-> funções de reigisto de sinais
 * 2ª-> aplicar os signhandlers no xmod.c
 * 3ª-> aplicar o sinal de fim de fil de processo filho
 * 
 */

int signSIGINTAnwserHandler(char answer[]) {
    //Se for igual a 1, vai mandar todos os procesos abortarem
    if(answer[0] =='1') {
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

    } else if(answer[0] == '2') {
        for(int i = 0; i<numberOfChildren; i++){
            eventSignalSent(SIGCONT, childProcesses[i]);
            kill(childProcesses[i], SIGCONT);
        }

        write(STDOUT_FILENO, "The program has been continued!\n", 33);

        return 0;

    } else { return -1; }
}



void signSIGINTHandler(int signo) {
    if (filename == NULL) {
        pause();
        return;
    }

    // Registers the signal
    eventSignalRecv(SIGINT);

    // Writing the pid; directory_name; number_of_files_found; number_of_files_modified
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "\n%d; %s; %d; %d", getpid(), filename, nftot, nfmod);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (isFirstParent) {
        sleep(1);  // Waiting for the children to write the messages

        char input[3];
        write(STDOUT_FILENO,"\nDo you want to stop the program(1) or do you prefer to continue(2)? ", 70);
        read(STDIN_FILENO, input, 2);

        while (signSIGINTAnwserHandler(input) == -1) {
            write(STDOUT_FILENO,"Please insert '1' to stop or '2' to continue! ", 47);
            read(STDIN_FILENO, input, 2);
        }
    } else {
        // Children pause and registers the event
        eventSignalSent(SIGSTOP, getpid());
        eventSignalRecv(SIGSTOP);
        pause();
    }
}


void sighandlerSIGCONT(int signo){
    //Regist que recebeu o sinal
    eventSignalRecv(SIGCONT);

    for (int i = 0; i < numberOfChildren; i++) {
        //envia o sigcont aos filhos e regista
        kill(childProcesses[i], SIGCONT);
        eventSignalSent(SIGCONT, childProcesses[i]);
    }

}

void sigHandlerSIGUSR1(int signo){
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

void sigHandlerSIGCHILD(int signo){
    eventSignalRecv(SIGCHLD);
}

