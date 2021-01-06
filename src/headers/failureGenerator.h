#ifndef UNIFI_FBW777_FAILUREGENERATOR_H
#define UNIFI_FBW777_FAILUREGENERATOR_H
#include "../util/headers/utilities.h"
#include "../util/headers/constant.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pfc.h"

typedef struct {
    FILE *log_file;
    pid_t selfpid;
    PFC **PFC_list[3];
    int signals[4];
} FailureGen;


void FailureGen__init(FailureGen *self,  PFC *PFC_list[3]);
FailureGen * FailureGen__create();
#endif