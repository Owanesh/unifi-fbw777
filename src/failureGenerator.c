#include <unistd.h>
#include <signal.h>
#include "util/headers/utilities.h"
#include "util/headers/constant.h"
#include "headers/failureGenerator.h"

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Calculate a random number between zero and maxLimit         :*/
/*::  after that check if the calculated number is equal to alpha :*/
/*::    maxLimit : max limit, should expect 10^n                  :*/
/*::    alpha : comparison number                                 :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
bool isProbability(int maxLimit, short alpha);

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Check if a signal needs to be sent to process      */
/*::  in case of send signal with kill() and return true */
/*::  false otherwise                                    */
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
bool isSignalSentTo(int signum, int probability, pid_t destinationProcess);

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Log a signal number into self.log_file             :*/
/*::    self : reference to self "object" in memory      :*/
/*::    pfc : reference to PFC who has received signal   :*/
/*::    signum : integer number, it's the signal number  :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void logEvent(FailureGen *self, PFC *pfc, int signum);

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Initialize log files                                     :*/
/*::  Create file if doesn't exist or open it in "append" mode :*/
/*::  Also create an header                                    :*/
/*::    self : reference to self "object" in memory            :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void failureGenerator_initFileLog(FailureGen *self);

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Provides a correct probability for every signal number      :*/
/*::  This function wraps all signal workflow                     :*/
/*::    self : reference to self "object" in memory               :*/
/*::    signum : int number, a signal number                      :*/
/*::    destinationProcess : reference to PFC "object" in memory  :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void sendAndLog(FailureGen *self, int signum, PFC *destinationProcess);

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function pick a random signal from self references :*/
/*::  and sends one for each PFC references                   :*/
/*::    self : reference to self "object" in memory           :*/
/*::  (works once per second)                                 :*/
/*::  (this function _Noreturn anything)                      :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
_Noreturn void choosePFCandSignal(FailureGen *self) {
    while (1) {
        sleep(1);
        int pickIndex;
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
    failureGenerator_initFileLog(self);
    choosePFCandSignal(self);
}

FailureGen *FailureGen__create() {
    FailureGen *fGen = (FailureGen *) malloc(sizeof(FailureGen));
    fGen->selfPid = getpid();
    fGen->signals[0] = SIGSTOP;
    fGen->signals[1] = SIGINT;
    fGen->signals[2] = SIGCONT;
    fGen->signals[3] = SIGUSR1;
    return fGen;
}

bool isProbability(int maxLimit, short alpha) {
    return random_number(0, maxLimit) == alpha ? true : false;
}

bool isSignalSentTo(int signum, int probability, pid_t destinationProcess) {
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
    char *extendedNameOfSignal;
    switch (signum) {
        case SIGSTOP:
            extendedNameOfSignal = "SIGSTOP";
            break;
        case SIGINT:
            extendedNameOfSignal = "SIGINT";
            break;
        case SIGCONT:
            extendedNameOfSignal = "SIGCONT";
            break;
        case SIGUSR1:
            extendedNameOfSignal = "SIGUSR1";
            break;
    }
    fprintf(self->log_file, "%d\t%s\t%d [%s]\n", pfc->selfPid, pfc->name, signum,extendedNameOfSignal);
    fclose(self->log_file);
}

void failureGenerator_initFileLog(FailureGen *self) {
    unlink(FAILUREGEN_LOGFILE);
    self->log_file = fopen(FAILUREGEN_LOGFILE, "w");
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
    if (isSignalSentTo(signum, probability, destinationProcess->selfPid)) {
        logEvent(self, destinationProcess, signum);
    }
}