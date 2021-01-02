#include "util/headers/utilities.h"
#include <signal.h>


bool isProbability(double elevation, short number){
    return random_number(0, elevation)==number?true:false;
}

void FailureGen__init(char *filename, char *name) {
    isProbability(1e4,SIGINT);
}
