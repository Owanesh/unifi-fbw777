#ifndef UNIFI_FBW777_TRANSDUCERS_H
#define UNIFI_FBW777_TRANSDUCERS_H

#include "pfc.h"

/**
 *  Transducer process every second, reads speed
 *  of three PFC and stares them into logfiles
*/
typedef struct {
    FILE *logFiles[3];              /**< reference to FILEs, used for logging */
    Channel communicationChannel;    /**< a communication channel with PFC */
} Transducer;

/**
 * Creates enough space in memory to allow this struct allocation
 * @return a pointer reference to space of memory where is stored a Transducer struct
 */
Transducer *Transducer__create();
void Transducer__init(Transducer *self);
/**
*  Initialize a communication between Transducer and PFC
 * @param self reference to self "object" in memory
 * @param channel defines channel attribute of struct Channel
 * @param channelType what kind of channel is | see Channel.type
 */
void Transducer__setCommunicationChannel(Transducer *self, int channel, int channelType);

#endif
