#include <stdio.h>
#include "headers/signals.h"

sighandler_t handle_sigUSR1(int num) {
    printf("Hi i'm handle_sigUSR1 err : %d pid %d \n",num,getpid());
    shifter = getpid();
    return NULL;
}
