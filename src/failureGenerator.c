#include "util/headers/utilities.h"
#include "util/headers/constant.h"
#include "headers/failureGenerator.h"
#include "headers/pfc.h"
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


bool isProbability(int elevation, short number) {
    return random_number(0, elevation) == number ? true : false;
}

bool isSignalSendedTo(int signum, int probability, pid_t destinationProcess) {
    if (isProbability((int) probability, PROBABILITY_NUMBER)) {
        kill(destinationProcess, signum);
        return true;
    }
    return false;
}

void logEvent(FailureGen *self, PFC *pfc, int signum) {
    if (fileExists(FAILUREGEN_LOGFILE)) {
        self->log_file = fopen(FAILUREGEN_LOGFILE, "a");
    } else {
        self->log_file = fopen(FAILUREGEN_LOGFILE, "w");
    }
    fprintf(self->log_file, "%d\t%s\t%d\n", pfc->selfpid, pfc->name, signum);
    fclose(self->log_file);
}


void sendAndLog(FailureGen *self, int signum, PFC *destinationProcess) {
    int probability = 0;
    switch (signum) {
        case SIGSTOP:
            probability = (int) 1e2;
            break;
        case SIGINT:
            probability = (int) 1e4;
            break;
        case SIGCONT:
            probability = (int) 1e1;
            break;
        case SIGUSR1:
            probability = (int) 1e1;
            break;
    }
    if (isSignalSendedTo(signum, probability, destinationProcess->selfpid)) {
        //printf("[LOG] send signum %d to pid %d\n", signum, destinationProcess);
        logEvent(self, destinationProcess, signum);
    }
}

_Noreturn void choosePFCandSignal(FailureGen *self) {
    while (1) {
        sleep(1);
        int pickIndex = 0;
        for (int india = 0; india < 4; india++) {
            pickIndex = random_number(0, 2);
            sendAndLog(self, self->signals[india], (*self->PFC_list[pickIndex]));
        }
    }
}

void initializeFileLog(FailureGen *self) {
    if (fileExists(FAILUREGEN_LOGFILE)) {
        self->log_file = fopen(FAILUREGEN_LOGFILE, "a");
    } else {
        self->log_file = fopen(FAILUREGEN_LOGFILE, "w");
    }
    fprintf(self->log_file, "------------------------\n");
    fprintf(self->log_file, "Process\tName\tSignal\n");
    fclose(self->log_file);
}

void FailureGen__init(FailureGen *self, PFC *PFC_list[3]) {
    self->PFC_list[0] = &PFC_list[0];
    self->PFC_list[1] = &PFC_list[1];
    self->PFC_list[2] = &PFC_list[2];
    initializeFileLog(self);
    choosePFCandSignal(self);
}


FailureGen *FailureGen__create() {
    FailureGen *fgen = (FailureGen *) malloc(sizeof(FailureGen));
    fgen->selfpid = getpid();
    fgen->signals[0] = SIGSTOP;
    fgen->signals[1] = SIGINT;
    fgen->signals[2] = SIGCONT;
    fgen->signals[3] = SIGUSR1;
    return fgen;
}


