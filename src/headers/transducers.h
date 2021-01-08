#ifndef UNIFI_FBW777_TRANSDUCERS_H
#define UNIFI_FBW777_TRANSDUCERS_H

#include "pfc.h"
#include <stdlib.h>

typedef struct {
    PFC **PFC_list[3];
    FILE *log_files[3];
    Channel pfcChannels[3];
} Transducer;

Transducer *Transducer__create();
void Transducer__init(Transducer *self, PFC *PFC_list[3]);
void Transducer__setComunicationChannel(Transducer *self, int channel, int channelType);

#endif //UNIFI_FBW777_TRANSDUCERS_H
