#ifndef UNIFI_FBW777_FAILUREGENERATOR_H
#define UNIFI_FBW777_FAILUREGENERATOR_H
#include "../util/headers/utilities.h"
#include "../util/headers/constant.h"

#include "pfc.h"

/**
 *  Definition of every signal's probability
 */
#define SIGSTOP_PROBABILITY 1e2
#define SIGINT_PROBABILITY 1e4
#define SIGCONT_PROBABILITY 1e1
#define SIGUSR1_PROBABILITY 1e2

/**
 *  Failure Generator process every second, pick a random PFC and
 *  send a signal choosed by self.signals
 *  Notice: Each signal has it own probability to be chose
*/
typedef struct {
    FILE *log_file;     /**< reference to FILE, used for logging */
    pid_t selfPid;      /**< a copy of pid of process who runs this code */
    PFC **PFC_list[3];  /**< references to three PFC, useful to implement an easier workflow.
                             * Sending signals or reading metadata of PFC*/
    int signals[4];     /**< list of signals. */
} FailureGen;

/**
 * Creates enough space in memory to allow this struct allocation
 * @param self reference to self "object" in memory
 * @param PFC_list an array of three pointers, each one refers to PFC
 */
void FailureGen__init(FailureGen *self,  PFC *PFC_list[3]);
/**
 * Creates enough space in memory to allow this struct allocation
 * @return a pointer reference to space of memory where is stored a FailureGen struct
 */
FailureGen * FailureGen__create();
#endif