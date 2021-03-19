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

int signSIGINTAnwserHandler(char *answer) {
    if (answer == NULL) return -1;

    int status;

    switch (answer[0]) {
    case '1':
        for(int i = 0; i < numberOfChildren; i++) {
            eventSignalSent(SIGUSR1, childProcesses[i]);
            kill(childProcesses[i], SIGUSR1);
        }

        // Waits for the children to terminate before it terminates itself
        for(int i = 0; i<numberOfChildren; i++)
            waitpid(childProcesses[i], &status, 1);

        write(STDOUT_FILENO, "The program has been cancelled!\n", 33);
        
        // Raises kill and registers it
        eventSignalSent(SIGKILL, getpid());
        eventSignalRecv(SIGKILL);
        eventProcExit(0);
        raise(SIGKILL);
        
    case '2':
        for(int i = 0; i < numberOfChildren; i++) {
            eventSignalSent(SIGCONT, childProcesses[i]);
            kill(childProcesses[i], SIGCONT);
        }
        
        write(STDOUT_FILENO, "The program has been continued!\n", 33);

        return 0;
    }
    return -1;
}

void signSIGINTHandler(int signo) {
    if (filename == NULL) {
        pause();
        return;
    }

    // Registers the signal
    eventSignalRecv(SIGINT);

    // Writing the pid; directory_name; number_of_files_found; number_of_files_modified
    char buffer[50];
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
    }
    else {
        // Children pause and registers the event
        eventSignalSent(SIGSTOP, getpid());
        eventSignalRecv(SIGSTOP);
        pause();
    }   
}


void sighandlerSIGCONT(int signo) {
    if (filename == NULL) return;

    // Registers the signal
    eventSignalRecv(SIGCONT);

    // Sends the signals to the children and registers it
    for (int i = 0; i < numberOfChildren; i++) {
        eventSignalSent(SIGCONT, childProcesses[i]);
        kill(childProcesses[i], SIGCONT);
    }
}

void sigHandlerSIGUSR1(int signo) {
    if (filename == NULL) raise(SIGKILL);
    
    // Registers the signal
    eventSignalRecv(SIGUSR1);

    // Sends the signals to the children and registers it
    for (int i = 0; i < numberOfChildren; i++) {
        eventSignalSent(SIGUSR1, childProcesses[i]);
        kill(childProcesses[i], SIGUSR1);
    }

    eventSignalSent(SIGKILL, getpid());
    eventSignalRecv(SIGKILL);
    eventSignalSent(SIGCHLD, getppid());
    eventProcExit(0);
    raise(SIGKILL);
}

void sigHandlerSIGCHILD(int signo) {eventSignalRecv(SIGCHLD);}
