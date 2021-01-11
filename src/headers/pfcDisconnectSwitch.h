#ifndef UNIFI_FBW777_PFCDISCONNECTSWITCH_H
#define UNIFI_FBW777_PFCDISCONNECTSWITCH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pfc.h"

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  PROCESS STATE CODES                                                       :*/
/*::  Here are the different values that the s, stat and state output           :*/
/*::  specifiers (header "STAT" or "S") will display to describe the state of   :*/
/*::  a process:                                                                :*/
/*::                                                                            :*/
/*::        D    uninterruptible sleep (usually IO)                             :*/
/*::        R    running or runnable (on run queue)                             :*/
/*::        S    interruptible sleep (waiting for an event to complete)         :*/
/*::        T    stopped by job control signal                                  :*/
/*::        t    stopped by debugger during the tracing                         :*/
/*::        W    paging (not valid since the 2.6.xx kernel)                     :*/
/*::        X    dead (should never be seen)                                    :*/
/*::        Z    defunct ("zombie") process, terminated but not reaped by       :*/
/*::             its parent                                                     :*/
/*::                                                                            :*/
/*::  For BSD formats and when the stat keyword is used, additional             :*/
/*::  characters may be displayed:                                              :*/
/*::                                                                            :*/
/*::        <    high-priority (not nice to other users)                        :*/
/*::        N    low-priority (nice to other users)                             :*/
/*::        L    has pages locked into memory (for real-time and custom IO)     :*/
/*::        s    is a session leader                                            :*/
/*::        l    is multi-threaded                                              :*/
/*::        +    is in the foreground process group                             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

typedef struct {
    PFC *pfcList[3];
    enum {
        WES_ERROR, WES_EMERGENCY
    } action;
    FILE *logFile;
} PFCDisconnectSwitch;


void pds__handleMessage(int typeOfMessage, int extraInfo);

#endif
