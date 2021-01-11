#ifndef UNIFI_FBW777_WES_H
#define UNIFI_FBW777_WES_H

#include <stdio.h>

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
      pid_t selfPid;
      FILE *logFile;
} Wes;


Wes *Wes__create();
#endif
