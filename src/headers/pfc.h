#ifndef UNIFI_FBW777_PFC_H
#define UNIFI_FBW777_PFC_H

typedef struct {
    double distance;
    double speed;
} PFCParameter;

typedef struct PFC {
    char *name;
    FILE *fpointer;
    char *filename;
    long filesize;
    double latitudes[2]; // last[0], current[1]
    double longitudes[2];
    int timestamps[2];
    PFCParameter param;
} PFC;

PFC *PFC__create(char *filename, char *name);
void PFC_read(PFC *self);
void _Noreturn updateCoordinates(PFC *self, double latitude, double longitude, int timestamp);
void PFC__reset(PFC *self);
#endif //UNIFI_FBW777_PFC_H
