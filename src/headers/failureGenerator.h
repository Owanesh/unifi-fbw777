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

#define SIGSTOP_PROBABILITY 1e2
#define SIGINT_PROBABILITY 1e4
#define SIGCONT_PROBABILITY 1e1
#define SIGUSR1_PROBABILITY 1e2

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  FailureGen struct                                 :*/
/*::    log_file : referece to FILE, used for logging   :*/
/*::    selfPid : a copy of pid of process who allocate :*/
/*::                memory for this struct              :*/
/*::    PFC_list : references to three PFC, useful to   :*/
/*::                implement an easier workflow.       :*/
/*::                Sending signals or reading          :*/
/*::                metadata of PFC                     :*/
/*::    signals : list of signals.                      :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
typedef struct {
    FILE *log_file;
    pid_t selfpid;
    PFC **PFC_list[3];
    int signals[4];
} FailureGen;

void FailureGen__init(FailureGen *self,  PFC *PFC_list[3]);
FailureGen * FailureGen__create();
#endif