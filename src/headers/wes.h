#ifndef UNIFI_FBW777_WES_H
#define UNIFI_FBW777_WES_H

#include <stdio.h>
#include "pfcDisconnectSwitch.h"

/**
 * Once per second, this process reads values from Transducer's logfile
 * If three values are equals each others send on stdout an "OK" message
 * If only one values isn't equal with others sends an ERROR message to PFCDisconnectSwitch
 * At the end, if are all different from each others WES sends an EMERGENCY message to PFCDisconnectSwitch
 */
typedef struct {
    FILE *fileData[3];         /**< reference to FILEs, used for reading values from transducers */
    FILE *logFile;              /**< reference to FILE, used for logging */
    PFCDisconnectSwitch *pds;   /**< pointer to PFCDisconnectSwitch */
} Wes;

void Wes_setPFCDisconnectSwitch(Wes *self, PFCDisconnectSwitch *pds);

Wes *Wes__create();

#endif
