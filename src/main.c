#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "headers/pfc.h"
#include "util/headers/utilities.h"
#include <signal.h>
#include "util/headers/signals.h"


int main(int argc, char *argv[]) {
    bool needshift = false;
    char *filename = checkFileIntoMainArgs(argc, argv);
    printf("Data location : %s\n", filename);
    welcomeMessage();
    pid_t PFC_list[3];
    if (!(PFC_list[0] = fork())) {
        signal(SIGUSR1, handle_sigUSR1);
        PFC *alpha = PFC__create(filename, "Alpha", getpid());
        sleep(2);
        PFC_read(alpha);
        PFC__destroy(alpha);
        exit(0);
    } else if (!(PFC_list[1] = fork())) {
        PFC *bravo = PFC__create(filename, "Bravo", getpid());
        sleep(2);
        PFC_read(bravo);
        PFC__destroy(bravo);
        exit(0);
    } else if (!(PFC_list[2] = fork())) {
        PFC *charlie = PFC__create(filename, "Charlie", getpid());
        sleep(2);
        PFC_read(charlie);
        PFC__destroy(charlie);
        exit(0);
    } else {
        kill(PFC_list[0], SIGUSR1);
        sleep(2);
        wait(&PFC_list[1]);
        wait(&PFC_list[2]);
        wait(&PFC_list[0]);

    }
    return 0;
}