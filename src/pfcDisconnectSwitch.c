#include "headers/pfcDisconnectSwitch.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
/**
 * By a provided pid, this function runs a `ps` command on shell and parse results
 * @param pid Process identificator
 * @param char A state of process, more info on `man ps`
*/
char checkStatusOf(pid_t pid) {
    /* why execute a ps command instead of read from /proc ?
     * /proc/$pid file isn't valid in BSD systems like MacOS
     */
    int link[2];
    pid_t execPid;
    char statusCode[5 + 1];
    char strPid[7];
    sprintf(strPid, "%d", pid);
    if (pipe(link) == -1) {
        fprintf(stderr, "%s\n", "[ERR][PFCDS]\tError while opening Pipe");
        exit(EXIT_FAILURE);
    }
    if ((execPid = fork()) == -1) {
        fprintf(stderr, "%s\n", "[ERR][PFCDS]\tError while create a child");
        exit(EXIT_FAILURE);
    }
    if (execPid == 0) {
        dup2(link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);
        execl("/bin/ps", "/bin/ps", strPid, "-o", "stat", NULL);
        fprintf(stderr, "%s\n", "[ERR][PFCDS]\tError on execl()");
        exit(EXIT_FAILURE);
    } else {
        close(link[1]);
        read(link[0], statusCode, sizeof(statusCode));
        wait(NULL);
    }
    return statusCode[5];
}

void pds__handleMessage(PFCDisconnectSwitch *self, int typeOfMessage, int extraInfo) {
    if (typeOfMessage == WES_EMERGENCY) {
        kill(getppid(), SIGTERM);
        kill(getpgrp(), SIGTERM);
    }
    if (typeOfMessage == WES_ERROR) {
        pid_t processPid = (*self->PFC_list[extraInfo])->selfPid;
        int nextPfc = extraInfo > 1 ? extraInfo == 1 ? 2 : 0 : 1;
        (*self->PFC_list[extraInfo])->seekPoint = (*self->PFC_list[nextPfc])->seekPoint;
        char statusOfPidToRestart = checkStatusOf(processPid);
        if (!statusOfPidToRestart) {
            printf("[PFCDS]\tProcess %d has received a SIGINT signal or maybe isn't started at all \n", processPid);
        }
        else if (statusOfPidToRestart == 'T') {
            printf("[PFCDS]\tProcess %d\tstopped by job control signal\t[SIGSTOP] \n",
                   processPid);
            PFC__reset(*self->PFC_list[extraInfo]);
            PFC_read((*self->PFC_list[extraInfo]));
            printf("[PFCDS]\tProcess %d\treceived now a new SIGCONT\n", processPid);
        } else if (statusOfPidToRestart == 'S') {
            printf("[PFCDS]\tProcess %d\tinterruptible sleep (waiting for an event to complete)\n",
                   processPid);
        } else
            printf("[PFCDS]\tProcess %d\tstatus [%c]\tcheck table at `man ps` for more.\n",
                   processPid, statusOfPidToRestart);
    }
}


PFCDisconnectSwitch *PDS__create(PFC *PFC_list[3]) {
    PFCDisconnectSwitch *pds = (PFCDisconnectSwitch *) malloc(sizeof(PFCDisconnectSwitch));
    pds->PFC_list[0] = &PFC_list[0];
    pds->PFC_list[1] = &PFC_list[1];
    pds->PFC_list[2] = &PFC_list[2];

    return pds;
}
