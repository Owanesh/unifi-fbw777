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
    pid_t selfpid;
    PFC **PFC_list[3];
    bool needsShift[3];
    int signals[4];
} FailureGen;

static pid_t shifter;

_Noreturn void setShiftFor(pid_t destinationprocess);
void FailureGen__init(FailureGen *self,  PFC *PFC_list[3]);
FailureGen * FailureGen__create();
#endif