#ifndef UNIFI_FBW777_PFC_H
#define UNIFI_FBW777_PFC_H


#include "../util/headers/utilities.h"
#include "../util/headers/constant.h"



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  PFCParameter is an inner sub-struct of PFC          :*/
/*::  stores information about speed and distance         :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
typedef struct {
    double distance;
    double speed;
} PFCParameter;



typedef struct PFC {
    pid_t selfPid;
    char *name;
    bool needShift;
    long seekPoint;
    FILE *filePointer;
    char *filename;
    long filesize;
    double latitudes[2]; // last[0], current[1]
    double longitudes[2];
    long timestamps[2];
    PFCParameter param;
    Channel com;
} PFC;


PFC *PFC__create(char *filename, char *name);
void PFC_read(PFC *self);
void PFC__destroy(PFC *self);
void PFC__setCommunicationChannel(PFC *self, int channel, int channelType);
void PFC_log(PFC *self);
void PFC__reset(PFC *self);

#endif
