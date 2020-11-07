#ifndef UNIFI_FBW777_PFC_H
#define UNIFI_FBW777_PFC_H

typedef struct {
    float distance;
    float speed;
} PFCParameters;

typedef struct PFC {
    FILE *fpointer;
    float latitude[2]; // last[0], current[1]
    float longitude[2];
    PFCParameters param;
} PFC;

void handle_signal();

void comunicate_to_transducer(float speed);

#endif //UNIFI_FBW777_PFC_H
