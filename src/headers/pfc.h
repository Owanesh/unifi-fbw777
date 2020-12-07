#ifndef UNIFI_FBW777_PFC_H
#define UNIFI_FBW777_PFC_H

typedef struct {
    float distance;
    float speed;
} PFCParameter;

typedef struct PFC {
    FILE *fpointer;
    char *filename;
    long filesize;
    float latitude[2]; // last[0], current[1]
    float longitude[2];
    PFCParameter param;
} PFC;

PFC* PFC__create(char *filename);
void PFCParameter__update(PFCParameter *self, float speed, float distance);
void PFC__destroy(PFC* pfc);
void PFC_read();
void PFC__checkfilesize(PFC *self);
void PFC_verbose(PFC *self);
void comunicate_to_transducer(float speed);

#endif //UNIFI_FBW777_PFC_H
