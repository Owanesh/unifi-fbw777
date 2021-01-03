#include <stdio.h>
#include "headers/signals.h"


void handle_sigUSR1(int num) {
    printf("[LOG] Handle_sigUSR1() called >> on pid %d\n",getpid());
    fflush(stdout);
    signal(SIGUSR1,handle_sigUSR1);
}
