#ifndef UNIFI_FBW777_CONSTANT_H
#define UNIFI_FBW777_CONSTANT_H

/** Microseconds between two reads of PFC  :*/
#define READ_SPEED 1000000
/** Radius of earth at Genoa  :*/
#define EARTH_RADIUS 6365
#define PROBABILITY_NUMBER 3

/** EMEA Standard Codes GPGLL  :*/
#define EMEA_GPGLL "$GPGLL"
/** Separator for each parameter of GPGLL row in file  :*/
#define EMEA_SEP ","

/** RESET VALUES :*/

#define PFC_RESETVAL -1
#define FILESIZE_RESET -1
#define SHIFTER_RESET 0
#define ERRVAL -1

/** Log filepath for FailureGen*/
#define FAILUREGEN_LOGFILE ".log/failures.log"
/** Log filepath for Wes*/
#define WES_LOGFILE ".log/status.log"
/** Log filepath for PFCDisconnectSwitch*/
#define PFCLS_LOGFILE ".log/switch.log"
/** Log filepath for communication between PFC and Transducer*/
#define TRANPFC_FILE ".log/third_transpfc.aeroplanetty"
/** Log filepath for PFC#1 */
#define TRANSDUCERS_LOGFILE1 ".log/speedPFC1.log"
/** Log filepath for PFC#2*/
#define TRANSDUCERS_LOGFILE2 ".log/speedPFC2.log"
/** Log filepath for PFC#3*/
#define TRANSDUCERS_LOGFILE3 ".log/speedPFC3.log"

/** Macro for detect if is running on unix machine */
#if defined(__unix__) || defined(__unix)
#define OS_linux
/** Macro for detect if is running on apple/mac machine */
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_apple
#endif
#endif
