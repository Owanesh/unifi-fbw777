#ifndef UNIFI_FBW777_PFC_H
#define UNIFI_FBW777_PFC_H


#include "../util/headers/utilities.h"
#include "../util/headers/constant.h"

/**
*  PFCParameter is an inner sub-struct of PFC
*  stores information about speed and distance
*/
typedef struct {
    double distance;    /**< traveled distance */
    double speed;       /**< cruise speed */
} PFCParameter;


/**
 * Primary Flight Computer (PFC)
 * The role of the PFC is the calculation of control laws by converting the pilot control position into
 * actuation commands, which are then transmitted to the Transducer
 */
typedef struct PFC {
    pid_t selfPid;      /**< a copy of pid of process who runs this code */
    char *name;         /**< formal name for pfc,also used for logging as metadata */
    bool needShift;     /**< flag tripped by signal SIGUSR1.
                             *If is true, this PFC at the next calculation,
                             *will do a leftshift of two bits on speed value*/
    long seekPoint;     /**< reference of last seek-point. Useful to compare with other PFC reading position */
    FILE *filePointer;  /**< reference to FILE, used for reading */
    char *filename;     /**< referenced to filePointer, used for logging */
    long filesize;      /**< this is an automatic value that is checked in runtime to determines if size of File
                             * pointed by PFC reads, is changed */
    double latitudes[2];/**< saves last latitude read an actual latitude */
    double longitudes[2];/**< saves last longitude read an actual longitude */
    long timestamps[2]; /**< reference to FILE, used for logging */
    PFCParameter param; /**< stores distance and speed into a PFCParameter */
    Channel com;        /**< a communication channel with Transducer */
} PFC;


PFC *PFC__create(char *filename, char *name);
void PFC_read(PFC *self);
void PFC__destroy(PFC *self);
void PFC__setCommunicationChannel(PFC *self, int channel, int channelType);
void PFC_log(PFC *self);
void PFC__reset(PFC *self);
void PFC__checkFilesize(PFC *self);

#endif
