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
    char *fileName;     /**< referenced to filePointer, used for logging */
    long fileSize;      /**< this is an automatic value that is checked in runtime to determines if size of File
                             * pointed by PFC reads, is changed */
    double latitudes[2];/**< saves last latitude read an actual latitude */
    double longitudes[2];/**< saves last longitude read an actual longitude */
    long timestamps[2]; /**< reference to FILE, used for logging */
    PFCParameter param; /**< stores distance and speed into a PFCParameter */
    Channel com;        /**< a communication channel with Transducer */
} PFC;


/**
 * Creates enough space in memory to allow this struct allocation
 * @param filename name of file used to reads GPGLL data
 * @param name a formal name for pfc,also used for logging as metadata
 * @return a pointer reference to space of memory where is stored a PFC struct
 */
PFC *PFC__create(char *filename, char *name);

/**
 *  Reads each line of file pointed by PFC.filePointer
 *  and if row starts with GPGLL then calculate distance and speed
 *  and send this values to Transducers through PFC.com
 *  also backup position of self.filePointer
 * @param self reference to self "object" in memory
 */
void PFC_read(PFC *self);

/**
 *  Closes PFC.filePointer, reset PFC and deallote memory in ram
 *  At the end stops the process sending a SIGQUIT signal
 *  and if row starts with GPGLL then calculate distance and speed
 *  and send this values to Transducers through PFC.com
 *  also backup position of self.filePointer
 * @param self reference to self "object" in memory
 */
void PFC__destroy(PFC *self);

/**
*  Initialize a communication between PFC and Transducers
 * @param self reference to self "object" in memory
 * @param channel defines channel attribute of struct Channel
 * @param channelType what kind of channel is | see Channel.type
 */
void PFC__setCommunicationChannel(PFC *self, int channel, int channelType);

/**
 *  Determines if communication will be through socket, pipe or shared files
 *  and write on it
 *
 *  If communication will be through file, acquires an exclusive lock
 *  before write on it
 * @param self reference to self "object" in memory
 */
void PFC_log(PFC *self);

/**
 * Reset every filePointer or last/current value stored as latitude/longitude or timestamp
 * @param self reference to self "object" in memory
 */
void PFC__reset(PFC *self);

/**
 * This function implements a bare security feature
 * Filesize is duplicated in each PFC, if anyone
 *  modify a line during the execution of program, it stops with
 *  exit(EXIT_FAILURE);
 *  Check every calls if fileSize pointed by filePointer is equal to stored fileSize
 * at first run, sets fileSize by move filePointer to EOF and use ftell() to know
 * seekPosition of filePointer
 *
 * @attention detect if a single char is removed, but not if it is changed
 * @param self reference to self "object" in memory
 */
void PFC__checkFilesize(PFC *self);

#endif
