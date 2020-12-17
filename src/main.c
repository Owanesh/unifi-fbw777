#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "headers/pfc.h"


int main() {
    PFC* pfc  = PFC__create("../resources/G18.txt");
    PFC_verbose(pfc);
    PFC_read(pfc);
    return 0;
}
