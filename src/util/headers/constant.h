#ifndef UNIFI_FBW777_CONSTANT_H
#define UNIFI_FBW777_CONSTANT_H

#define READ_SPEED 936 //used with usleep, less than 1 sec
#define EARTH_RADIUS 6365
#define PROBABILITY_NUMBER 3
/*:: EMEA Standard Codes   :*/
#define EMEA_GPGLL "$GPGLL"
/* #define EMEA_GPGSV "$GPGSV"
 * #define EMEA_GPGSA "$GPGSA"
 * #define EMEA_GPVTG "$GPVTG"
 * #define EMEA_GPRMC "$GPRMC"
 * #define EMEA_GPGGA "$GPGGA"
*/
#define EMEA_SEP ","

/*:: RESET VALUES :*/

#define PFC_RESETVAL -1
#define FILESIZE_RESET -1

/*:: TMP FILE PATH :*/
#define TMPFOLDER "/tmp/"
#define FAILUREGEN_LOGFILE "failures.log"
#define WES_LOGFILE "status.log"
#define PFCLS_LOGFILE "switch.log"

#endif //UNIFI_FBW777_CONSTANT_H
