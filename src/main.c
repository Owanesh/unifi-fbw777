#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "headers/pfc.h"

int main() {
    PFC* pfc  = PFC__create("filedebllo.txt");
    PFC_verbose(pfc);
    return 0;
}
