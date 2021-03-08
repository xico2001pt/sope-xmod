#include "regfile.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>

/**
 * @brief LOG_FILENAME number if it exists, -1 if it doesnt
 * 
 */
extern int logFile;

extern clock_t startClock;


int initLogFile(){
    
    char* logFilename = getenv("LOG_FILENAME");
    
    if(logFilename==NULL){
        //Não existe path para o logFilename
        return 1;
    }
    else{
        //Flags -> Escrever apenas, cria ficheiro se não existir e volta a 0 se já existir
        //Modo de permissão -> Ler para todos os utilizadores
        logFile = open(logFilename, O_WRONLY|O_CREAT|O_TRUNC, S_IRGRP|S_IRUSR|S_IROTH|S_IWGRP|S_IWOTH|S_IWUSR);
        if(logFile==-1){
            //Erro em abrir o ficheiro
            return -1;
        }
        //Nenhum erro em abrir o ficheiro
        return 0;
    }

}

int registerEvent(clock_t registration_instant, pid_t pid);

int eventProcCreat(int argc, char * argv[]){
    //Apos inst; pid; fica inst; pid; event ;
    write(logFile, "PROC_CREAT",10);
    write(logFile, " ; ", 3);

    //Apos inst ; pid ; event ; fica inst ; pid ; event ; arg[1] arg[2] arg[3]\n 
    for(int i = 0; i<argc; i++){
        write(logFile, argv[i], strlen(argv[i]));
        write(logFile, " ", 1);
    }
    
    write(logFile, "\n", 1);

    return 0;
}

int eventProcExit(int exitStatus);

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode){
    //Escrita do evento
    write(logFile, "FILE_MODF", 9);
    write(logFile, " ; ", 3);

    //Escrita do ficheiro
    write(logFile, filename, strlen(filename));
    write(logFile, " : ", 3);

    //Escrita do oldMode
    char oldmode[10];
    sprintf(oldmode, "%#o : ", oldMode);
    write(logFile, oldmode, 7);

    char newmode[10];
    sprintf(newmode, "%#o\n", newMode);
    write(logFile, newmode, 5);

    return 0;

}