#ifndef UNIFI_FBW777_CONSTANT_H
#define UNIFI_FBW777_CONSTANT_H

#define READ_SPEED 1000000 //used with usleep, less than 1 sec
#define EARTH_RADIUS 6365
#define PROBABILITY_NUMBER 3

/*:: EMEA Standard Codes   :*/
#define EMEA_GPGLL "$GPGLL"

#define EMEA_SEP ","

/*:: RESET VALUES :*/

#define PFC_RESETVAL -1
#define FILESIZE_RESET -1
#define SHIFTER_RESET 0

/*:: TMP FILE PATH :*/

#define TMPFOLDER ".tmp/"
#define FAILUREGEN_LOGFILE "failures.log"
#define WES_LOGFILE "status.log"
#define PFCLS_LOGFILE "switch.log"
#define TRANPFC_FILE "third_transpfc.aeroplanetty"
#define TRANSDUCERS_LOGFILE1 "speedPFC1.log"
#define TRANSDUCERS_LOGFILE2 "speedPFC2.log"
#define TRANSDUCERS_LOGFILE3 "speedPFC3.log"


#endif //UNIFI_FBW777_CONSTANT_H
