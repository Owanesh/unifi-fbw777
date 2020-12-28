#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "headers/pfc.h"


int main() {
    char *filename = "../resources/G18.txt";

    pid_t child1, child2, child3;
    if (!(child1 = fork())) {
        PFC* alpha  = PFC__create(filename, "Alpha");
        sleep(3);

        PFC_read(alpha);
        exit(0);
    } else if (!(child2 = fork())) {
        PFC* bravo  = PFC__create(filename, "Bravo");
        sleep(3);
        PFC_read(bravo);
        exit(0);
    } else if (!(child3 = fork())) {
        PFC* charlie  = PFC__create(filename, "Charlie");
        sleep(3);
        PFC_read(charlie);
        exit(0);
    } else {
        wait(&child1);
        //printf("End of process Alpha\n");
        wait(&child2);
        //printf("End of process Bravo\n");
        wait(&child3);
        //printf("End of process Charlie\n");
    }

    return 0;
}
