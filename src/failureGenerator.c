#include "util/headers/utilities.h"
#include "util/headers/constant.h"
#include "headers/failureGenerator.h"
#include "headers/pfc.h"

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Calculate a random number between zero and maxlimit         :*/
/*::  after that check if the calculated number is equal to alpha :*/
/*::    maxlimit : max limit, should expect 10^n                  :*/
/*::    alpha : comparison number                                 :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
bool isProbability(int maxlimit, short alpha);

bool isSignalSendedTo(int signum, int probability, pid_t destinationProcess);

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Log a signal number into self.log_file             :*/
/*::    self : referece to self "object" in memory       :*/
/*::    pfc : referece to PFC who has received signal    :*/
/*::    signum : integer number, it's the signal number  :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void logEvent(FailureGen *self, PFC *pfc, int signum);

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Initialize log files                                     :*/
/*::  Create file if doesn't exist or open it in "append" mode :*/
/*::  Also create an header                                    :*/
/*::    self : referece to self "object" in memory             :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void failuregenerator_initfileLog(FailureGen *self);

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Provides correct probability for every signal number      :*/
/*::  This function wraps all signal workflow                   :*/
/*::    self : referece to self "object" in memory              :*/
/*::    signum : int number, a signal number                    :*/
/*::    destinationProcess : referece to PFC "object" in memory :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void sendAndLog(FailureGen *self, int signum, PFC *destinationProcess);

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function pick a random signal from self references :*/
/*::  and sends one for each PFC refereces                    :*/
/*::    self : referece to self "object" in memory            :*/
/*::  (works once per second)                                 :*/
/*::  (this function _Noreturn anything)                      :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
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


void FailureGen__init(FailureGen *self, PFC *PFC_list[3]) {
    self->PFC_list[0] = &PFC_list[0];
    self->PFC_list[1] = &PFC_list[1];
    self->PFC_list[2] = &PFC_list[2];
    failuregenerator_initfileLog(self);
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


bool isProbability(int maxlimit, short alpha) {
    return random_number(0, maxlimit) == alpha ? true : false;
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

void failuregenerator_initfileLog(FailureGen *self) {
    if (fileExists(FAILUREGEN_LOGFILE)) {
        self->log_file = fopen(FAILUREGEN_LOGFILE, "a");
    } else {
        self->log_file = fopen(FAILUREGEN_LOGFILE, "w");
    }
    fprintf(self->log_file, "------------------------\n");
    fprintf(self->log_file, "Process\tName\tSignal\n");
    fclose(self->log_file);
}

void sendAndLog(FailureGen *self, int signum, PFC *destinationProcess) {
    int probability = 0;
    switch (signum) {
        case SIGSTOP:
            probability = (int) SIGSTOP_PROBABILITY;
            break;
        case SIGINT:
            probability = (int) SIGINT_PROBABILITY;
            break;
        case SIGCONT:
            probability = (int) SIGCONT_PROBABILITY;
            break;
        case SIGUSR1:
            probability = (int) SIGUSR1_PROBABILITY;
            break;
    }
    if (isSignalSendedTo(signum, probability, destinationProcess->selfpid)) {
        logEvent(self, destinationProcess, signum);
    }
}