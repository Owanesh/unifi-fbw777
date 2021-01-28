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
 *  @param process processID
 *  @param status a char that identifies status of process
 *  @param explanation a brief description of status
 */
void PDS__logAction(PFCDisconnectSwitch *self, pid_t process, char status, char *explanation);

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
            PDS__logAction(self, processPid, '~',
                           "Unknown status, maybe received a SIGINT signal or maybe isn't started at all");
        } else if (statusOfPidToRestart == 'T') {
            PDS__logAction(self, processPid, statusOfPidToRestart, "Stopped by job control signal	[SIGSTOP]");
        }
        switch (statusOfPidToRestart) {
            case 'S':
                PDS__logAction(self, processPid, statusOfPidToRestart,
                               "Interruptible sleep (waiting for an event to complete)");
                break;
            case 'D':
                PDS__logAction(self, processPid, statusOfPidToRestart, "Uninterruptible sleep (usually IO)");
                break;
            case 'R':
                PDS__logAction(self, processPid, statusOfPidToRestart, "Paging (not valid since the 2.6.xx kernel)");
                break;
            case 'W':
                PDS__logAction(self, processPid, statusOfPidToRestart, "Running or runnable (on run queue)");
                break;
            case 'X':
                PDS__logAction(self, processPid, statusOfPidToRestart, "Dead (should never be seen)");
                break;
            case 'Z':
                PDS__logAction(self, processPid, statusOfPidToRestart,
                               "Defunct ('zombie') process, terminated but not reaped by its parent");
                break;
        }
    }
}

PFCDisconnectSwitch *PDS__create(PFC *PFC_list[3]) {
    PFCDisconnectSwitch *pds =
            (PFCDisconnectSwitch *) malloc(sizeof(PFCDisconnectSwitch));
    pds->PFC_list[0] = &PFC_list[0];
    pds->PFC_list[1] = &PFC_list[1];
    pds->PFC_list[2] = &PFC_list[2];
    unlink(PFCLS_LOGFILE);
    return pds;
}

void PDS__logAction(PFCDisconnectSwitch *self, pid_t process, char status, char *explanation) {
    if (fileExists(PFCLS_LOGFILE)) {
        self->logFile = fopen(PFCLS_LOGFILE, "a");
    } else {
        self->logFile = fopen(PFCLS_LOGFILE, "w");
    }
    fprintf(self->logFile, "%d\t%c %s\n", process, status, explanation);
    fclose(self->logFile);
}
