#include "headers/signals.h"

int shifter = -1;

void handle_sigUSR1(int num) {
    shifter = getpid();
    signal(SIGUSR1,handle_sigUSR1);
}
