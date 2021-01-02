#ifndef UNIFI_FBW777_PFC_H
#define UNIFI_FBW777_PFC_H
#include <unistd.h>

typedef struct {
    double distance;
    double speed;
} PFCParameter;

typedef struct PFC {
    pid_t selfpid;
    char *name;
    long seekpoint;
    FILE *fpointer;
    char *filename;
    long filesize;
    double latitudes[2]; // last[0], current[1]
    double longitudes[2];
    long timestamps[2];
    PFCParameter param;
} PFC;

PFC *PFC__create(char *filename, char *name, pid_t pid);
void PFC_read(PFC *self);
void  PFC__destroy(PFC *self);
#endif //UNIFI_FBW777_PFC_H
