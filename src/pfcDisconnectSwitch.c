#include "headers/pfcDisconnectSwitch.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
/**
 * LogAction allows to save every stdout output into file
 *  @param self Reference to PFCDisconnectSwitch object
 *  @param action a message that will be stored into file
 */
void PDS__logAction(PFCDisconnectSwitch *self, char *action);

/**
 * By a provided pid, this function runs a `ps` command on shell and parse
 * results
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
#ifdef OS_apple
        execl("/bin/ps", "/bin/ps", strPid, "-o", "stat", NULL);
#else
        execl("/bin/ps", "/bin/ps", "-o", "stat", strPid, NULL);
#endif
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
        system("killall aeroplanetty");
    }
    if (typeOfMessage == WES_ERROR) {
        pid_t processPid = (*self->PFC_list[extraInfo])->selfPid;
        int nextPfc = extraInfo > 1 ? extraInfo == 1 ? 2 : 0 : 1;
        (*self->PFC_list[extraInfo])->seekPoint =
                (*self->PFC_list[nextPfc])->seekPoint;
        char statusOfPidToRestart = checkStatusOf(processPid);

        if (!statusOfPidToRestart) {
            printf("[PFCDS]\tProcess %d\tUnknown status, maybe received a SIGINT signal or maybe isn't started at all \n", processPid);
        }
        else if (statusOfPidToRestart == 'T') {
            printf("[PFCDS]\tProcess %d\tStopped by job control signal\t[SIGSTOP] \n", processPid);
            PFC__reset(*self->PFC_list[extraInfo]);
            PFC_read((*self->PFC_list[extraInfo]));
            printf("[PFCDS]\tProcess %d\tReceived now a new SIGCONT\n", processPid);
        }
        switch (statusOfPidToRestart) {
            case 'S':
                printf("[PFCDS]\tProcess %d\tInterruptible sleep (waiting for an event to complete)\n", processPid);
                break;
            case 'D':
                printf("[PFCDS]\tProcess %d\tUninterruptible sleep (usually IO)", processPid);
                break;
            case 'R':
                printf("[PFCDS]\tProcess %d\tPaging (not valid since the 2.6.xx kernel)", processPid);
                break;
            case 'W':
                printf("[PFCDS]\tProcess %d\tRunning or runnable (on run queue)", processPid);
                break;
            case 'X':
                printf("[PFCDS]\tProcess %d\tDead (should never be seen)", processPid);
                break;
            case 'Z':
                printf("[PFCDS]\tProcess %d\tDefunct ('zombie') process, terminated but not reaped by its parent", processPid);
                break;
        }
    }
}

PFCDisconnectSwitch *PDS__create(PFC *PFC_list[3]) {
    PFCDisconnectSwitch *pds =
            (PFCDisconnectSwitch *)malloc(sizeof(PFCDisconnectSwitch));
    pds->PFC_list[0] = &PFC_list[0];
    pds->PFC_list[1] = &PFC_list[1];
    pds->PFC_list[2] = &PFC_list[2];

    return pds;
}

void PDS__logAction(PFCDisconnectSwitch *self, char *action) {
    if (fileExists(PFCLS_LOGFILE)) {
        self->logFile = fopen(PFCLS_LOGFILE, "a");
    } else {
        self->logFile = fopen(PFCLS_LOGFILE, "w");
    }
    fprintf(self->logFile, "%s\n", action);
    fclose(self->logFile);
}
