#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "headers/utilities.h"
#include "headers/constant.h"

/** Pi Greco approx */
#define pi 3.14159265358979323846

/**
*  South latitudes are negative, east longitudes are positive
*/
double distanceBetweenPoints(double lat1,   /**< Latitude of point 1 (in decimal degrees) */
                             double lon1,   /**< Longitude of point 1 (in decimal degrees) */
                             double lat2,   /**< Latitude of point 2 (in decimal degrees) */
                             double lon2    /**< Longitude of point 2 (in decimal degrees) */
) {
    double earthRadius_km = EARTH_RADIUS * 1e3;
    double diffLat, diffLon, radLat1, radLat2, alpha, bravo, charlie;
    diffLat = deg2rad(lat2 - lat1);
    diffLon = deg2rad(lon2 - lon1);
    radLat1 = deg2rad(lat1);
    radLat2 = deg2rad(lat2);
    alpha = sin(diffLat / 2) * sin(diffLat / 2) + cos(radLat1) * cos(radLat2) * sin(diffLon / 2) * sin(diffLon / 2);
    bravo = 2 * atan2(sqrt(alpha), sqrt(1 - alpha));
    charlie = earthRadius_km * bravo;
    return charlie;
}

/**
*  This function converts decimal degrees to radians
*/
double deg2rad(double deg) {
    return (deg * pi / 180);
}

/**
*  This function converts radians to decimal degrees
*/
__unused double rad2deg(double rad) {
    return (rad * 180 / pi);
}

/**
*  This function converts a string into double number
*/
double str2double(char *string) {
    char *rest;
    double ret;
    ret = strtod(string, &rest);
    return ret;
}

/**
*  Calculate speed between two points with timestamp and distance
*/
double speedBetweenPoints(int timestamp1,   /**< Timestamp at point 1 */
                          int timestamp2,   /**< Timestamp at point 2 */
                          double distance   /**< calculated distance between points */
) {
    return timestamp2 > timestamp1 ? (distance / (timestamp2 - timestamp1)) : 0;
}

/**
*  Check if a file exist
*/
bool fileExists(const char *filename) {
    FILE *file;
    if ((file = fopen(filename, "r"))) {
        fclose(file);
        return true;
    }
    return false;
}

/***
*  Check if substring is contained into a string
*/
bool strContains(const char *search, /**< substring to find */
                 const char *content /**< string who maybe contains "search" arg */
) {
    char *found;
    if (!search || !content) return -1;
    found = strstr(content, search);
    return found ? true : false;
}

/**
*  Check how many char (token) are contained into string
*/
int strTokenCount(const char *buffer,   /**< string who maybe contains "token" char */
                  const char token      /**< character to looking for */
) {
    int count = 0;
    size_t i = 0;
    while (buffer[i] != '\0') {
        if (buffer[i] == token) {
            count++;
        }
        i++;
    }
    return count;
}

/**
*  Converts a string (char *) to an integer value
*/
__unused int str2i(const char *str) {
    int num = 0;
    int i = 0;
    bool isNegative = false;
    if (str[i] == '-') {
        isNegative = true;
        i++;
    }
    while (str[i] && (str[i] >= '0' && str[i] <= '9')) {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    if (isNegative) num = -1 * num;
    return num;
}

/**
*  Split a string divided by separator. this fill an array
*/
void strSplit(char *buffer,             /**< string that will be splitted */
              const char *separator,    /**< delimiter of each substring */
              char **array              /**< will contains substrings  */
) {
    int i = 0;
    char *p = strtok(buffer, separator);
    while (p != NULL) {
        array[i] = p;
        i++;
        p = strtok(NULL, separator);
    }
}

/**
*  Calculate random integer between two limits
*      returns : min < (number) < max
*/
int random_number(int min_num,/**< min number allowed for a random value */
                  int max_num /**< max number allowed for a random value */
) {
    int result, low_num, hi_num;
    if (min_num < max_num) {
        low_num = min_num;
        hi_num = max_num + 1;
    } else {
        low_num = max_num + 1;
        hi_num = min_num;
    }
    // clock() and random() to increase entropy
    srand((unsigned) time(NULL) + clock() + random());
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

/**
*  This function prints only an header for this program
*/
void welcomeMessage() {
    printf("\033[0;35m");
    printf("\n"
           "┌─┐┬ ┬ ┬┌┐ ┬ ┬┬ ┬┬┬─┐┌─┐\n"
           "├┤ │ └┬┘├┴┐└┬┘││││├┬┘├┤ \n"
           "└  ┴─┘┴ └─┘ ┴ └┴┘┴┴└─└─┘\n"
           "");
    printf("\033[;36m© 2020 Owanesh\n");
    printf("\033[1;32m========================\n");
    printf("\033[0m");
}

/**
*  Gets filename by argv array passed to application
*   ./aeroplanetty <filepath_g18.tx>
*      if filepath is invalid or file does not exist, will use
*      internal file stored in /resources/G18.txt
*/
char *checkFileIntoMainArgs(int argc, char *argv[]) {
    if (argc == 2) {
        printf("The argument supplied is %s\n", argv[1]);
        if (fileExists(argv[1])) {
            return argv[1];
        } else {
            printf("File %s not exist or is invalid. Using default...\n", argv[1]);
        }
    } else if (argc > 2) {
        printf("Too many arguments supplied  - Using default...\n");
    } else {
        printf("No external G18 was provided - Using default...\n");
    }
    return "../resources/G18.txt";
}

/**
*  Get index of position element in array   (recursive-chain)
*/
int getIndexOfPFCList(pid_t PFC_pid,   /**< element to find */
                      pid_t *PFC_list, /**< array of elements */
                      int position     /**< position, needs to be 0 at calling */
) {
    if (PFC_pid == 0)
        return -1;
    else if (PFC_pid == PFC_list[position])
        return position;
    else
        return getIndexOfPFCList(PFC_pid, PFC_list, ++position);
}

/**
*  Creates and returns a named-provided socket
*/
__unused int make_named_socket(const char *filename /**< name of namedSocket */) {
    unlink(filename);
    struct sockaddr_un sockFile;
    int sock;
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[ERR] Socket error: ");
        exit(EXIT_FAILURE);
    }
    sockFile.sun_family = AF_UNIX;
    strcpy (sockFile.sun_path, filename);
    if (bind(sock, (struct sockaddr *) &sockFile, sizeof(sockFile)) < 0) {
        perror("[ERR] Bind error : ");
        exit(EXIT_FAILURE);
    }
    return sock;
}

/**
* Provides an extended name for Channel.type that is an enum
*     SOCKCH : Socket
*     PIPECH : Pipe
*     FILECH : File
*/
char *Channel__extendedName(int channelType /**< a Channel type value, referred to its enum type*/) {
    switch (channelType) {
        default:
            return "Undefined";
        case SOCKCH:
            return "Socket";
        case PIPECH:
            return "Pipe";
        case FILECH:
            return "File";
    }
}

/**
 * fequal compares two doubles number
 *
 *  To compare them, will subtract each other and compare the result
 *  with and entropy that determines what is the "level" of equity
*/
bool fequal(double a, double b) {
    return fabs(a - b) < 0.000001;
}
