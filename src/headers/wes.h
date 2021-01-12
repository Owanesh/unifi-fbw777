#ifndef UNIFI_FBW777_WES_H
#define UNIFI_FBW777_WES_H

#include <stdio.h>
#include "pfcDisconnectSwitch.h"
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  WES struct                                        :*/
/*::    fileData : reference to FILEs, used for reading :*/
/*::               values from transducers              :*/
/*::    selfPid : a copy of pid of process who runs this:*/
/*::                code                                :*/
/*::    log_file : reference to FILE, used for logging  :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
typedef struct {
    FILE *fileData[3];
    FILE *logFile;
    PFCDisconnectSwitch *pds;
} Wes;

void Wes_setPFCDisconnectSwitch(Wes *self, PFCDisconnectSwitch *pds);

Wes *Wes__create();

#endif
