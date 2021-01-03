#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "headers/pfc.h"
#include "headers/failureGenerator.h"
#include "util/headers/utilities.h"
#include <signal.h>


int main(int argc, char *argv[]) {
    int alpha_pipe[2];
    pipe(alpha_pipe);
    int bravo_pipe[2];
    pipe(bravo_pipe);
    int charlie_pipe[2];
    pipe(charlie_pipe);

    char *filename = checkFileIntoMainArgs(argc, argv);
    printf("Data location : %s\n", filename);
    pid_t PFC_pid_list[3];
    welcomeMessage();

    PFC *PFC_list[3];
    PFC_list[0] = PFC__create(filename, "Alpha");
    PFC_list[1] = PFC__create(filename, "Bravo");
    PFC_list[2] = PFC__create(filename, "Charlie");

    FailureGen *fgen = FailureGen__create();
    if (!(PFC_pid_list[0] = fork())) {
        PFC_list[0]->selfpid = getpid();
        close(alpha_pipe[0]);
        write(alpha_pipe[1],PFC_list[0], sizeof(PFC *));
        close(alpha_pipe[1]);

        usleep(2);
        PFC_read(PFC_list[0]);
        PFC__destroy(PFC_list[0]);
        exit(0);

    } else if (!(PFC_pid_list[1] = fork())) {
        PFC_list[1]->selfpid = getpid();
        close(bravo_pipe[0]);
        write(bravo_pipe[1],PFC_list[1], sizeof(PFC *));
        close(bravo_pipe[1]);

        sleep(2);
        PFC_read(PFC_list[1]);
        PFC__destroy(PFC_list[1]);
        exit(0);

    } else if (!(PFC_pid_list[2] = fork())) {
        PFC_list[2]->selfpid = getpid();
        close(charlie_pipe[0]);
        write(charlie_pipe[1], PFC_list[2], sizeof(PFC *));
        close(charlie_pipe[1]);

        sleep(2);
        PFC_read(PFC_list[2]);
        PFC__destroy(PFC_list[2]);
        exit(0);

    } else {
        close(alpha_pipe[1]);
        read(alpha_pipe[0], PFC_list[0], sizeof(PFC *));
        close(alpha_pipe[0]);
        close(bravo_pipe[1]);
        read(bravo_pipe[0], PFC_list[1], sizeof(PFC *));
        close(bravo_pipe[0]);
        close(charlie_pipe[1]);
        read(charlie_pipe[0], PFC_list[2], sizeof(PFC *));
        close(charlie_pipe[0]);
        FailureGen__init(fgen, PFC_list);

        wait(&PFC_pid_list[0]);
        wait(&PFC_pid_list[1]);
        wait(&PFC_pid_list[2]);
    }
    return 0;
}