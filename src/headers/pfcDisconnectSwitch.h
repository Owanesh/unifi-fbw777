#ifndef UNIFI_FBW777_PFCDISCONNECTSWITCH_H
#define UNIFI_FBW777_PFCDISCONNECTSWITCH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pfc.h"

/* *****************************************************/
/**  PROCESS STATE CODES                                                       :*/
/**  Here are the different values that the s, stat and state output           :*/
/**  specifiers (header "STAT" or "S") will display to describe the state of   :*/
/**  a process:                                                                :*/
/**                                                                            :*/
/**        D    uninterruptible sleep (usually IO)                             :*/
/**        R    running or runnable (on run queue)                             :*/
/**        S    interruptible sleep (waiting for an event to complete)         :*/
/**        T    stopped by job control signal                                  :*/
/**        t    stopped by debugger during the tracing                         :*/
/**        W    paging (not valid since the 2.6.xx kernel)                     :*/
/**        X    dead (should never be seen)                                    :*/
/**        Z    defunct ("zombie") process, terminated but not reaped by       :*/
/**             its parent                                                     :*/
/**                                                                            :*/
/**  For BSD formats and when the stat keyword is used, additional             :*/
/**  characters may be displayed:                                              :*/
/**                                                                            :*/
/**        <    high-priority (not nice to other users)                        :*/
/**        N    low-priority (nice to other users)                             :*/
/**        L    has pages locked into memory (for real-time and custom IO)     :*/
/**        s    is a session leader                                            :*/
/**        l    is multi-threaded                                              :*/
/**        +    is in the foreground process group                             :*/
/*****************************************************:*/

#define WES_ERROR 2
#define WES_EMERGENCY 1

/**
 *  PFCDisconnectSwitch reveices by Wes a message
 *  if message is an WES_EMERGENCY type then, kill entire app
 *  else if message is WES_ERROR type, then check and log status of process
*/
typedef struct {
    PFC **PFC_list[3];/**< references to three PFC, useful to implement an easier workflow.
                         * Sending signals or reading metadata of PFC*/
    FILE *logFile; /**< reference to FILE, used for logging */
} PFCDisconnectSwitch;

/**
 * Creates enough space in memory to allow this struct allocation
 * @return a pointer reference to space of memory where is stored a PFCDisconnectSwitch struct
 */
PFCDisconnectSwitch *PDS__create(PFC *PFC_list[3]);
void pds__handleMessage(PFCDisconnectSwitch *self,int typeOfMessage, int extraInfo);

#endif