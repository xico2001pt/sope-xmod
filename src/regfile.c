#include "regfile.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

/**
 * @brief LOG_FILENAME number if it exists, -1 if it doesnt
 * 
 */
static int logFile;

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

int registerEvent() {
    // Verify if the file exists
    if (logFile == -1)
        return 1;
    
    // Register instant
    struct tms t;
    char instStr[10];
    int inst = (times(&t) - startClock) * 1000 / sysconf(_SC_CLK_TCK);
    sprintf(instStr, "%d ; ", inst);
    write(STDOUT_FILENO, instStr, strlen(instStr));

    // Register PID
    char pidStr[10];
    pid_t pid = getpid();
    sprintf(pidStr, "%d; ", pid);
    write(STDOUT_FILENO, pidStr, strlen(pidStr));

    return 0;
}

int eventProcCreat(int argc, char * argv[]){
    // Verify if the file exists
    if (registerEvent() == 1)
        return 1;
    
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

int eventProcExit(int exitStatus){
    // Verify if the file exists
    if (registerEvent() == 1)
        return 1;
    
    //Apos inst; pid; fica inst; pid; event ;
    write(logFile, "PROC_EXIT", 9);
    write(logFile, " ; ", 3);
    write(logFile, exitStatus, sizeof(int));

    return 0;
}

int eventSignalRecv(int signo);

int eventSignalSent(int signo, pid_t targetPID);

int eventFileModf(char * filename, mode_t oldMode, mode_t newMode){
    // Verify if the file exists
    if (registerEvent() == 1)
        return 1;
    
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
