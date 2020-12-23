#include <math.h>
#include "headers/utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define pi 3.14159265358979323846



/*::    South latitudes are negative, east longitudes are positive           :*/
/*::    lat1, lon1 = Latitude and Longitude of point 1 (in decimal degrees)  :*/
/*::    lat2, lon2 = Latitude and Longitude of point 2 (in decimal degrees)  :*/
/*::    unit = the unit you desire for results                               :*/
/*::           where: 'M' is statute miles                1.1515             :*/
/*::                  'K' is kilometers    (default)      1.609344           :*/
/*::                  'N' is nautical miles               0.8684             :*/

double distanceBetweenPoints(double lat1, double lon1, double lat2, double lon2, char unit) {
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else {
        theta = lon1 - lon2;
        dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
        dist = acos(dist);
        dist = rad2deg(dist);
        dist = dist * 1.609344;
        switch (unit) {
            case 'K':
                break;
            case 'M':
                dist = dist * 60 * 1.1515;
                break;
            case 'N':
                dist = dist * 0.8684;
                break;
        }
        return (dist);
    }
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
    return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
    return (rad * 180 / pi);
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts a string into double number            :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double str2double(char *string){
    char *ptr;
    double ret;
    ret = strtod(string, &ptr);
    return ret;
}



/*::    Calculate speed between two points with timestamp and distance       :*/
/*::    timestamp1 = Timestamp at point 1                                    :*/
/*::    timestamp2 = Timestamp at point 2                                    :*/
/*::    distance = calculated distance between points                        :*/
/*::    unit = the unit you desire for results                               :*/
/*::           where: 'M' is miles    (default)                              :*/
/*::                  'K' is kilometers per hour                             :*/
double speedBetweenPoints(int timestamp1, int timestamp2, double distance, char unit) {
    double time_between = (timestamp2 - timestamp1) / 1000.0;
    double speed = distance / time_between;
    switch (unit) {
        case 'M':
            break;
        case 'K':
            speed = (speed * 3600.0) / 1000.0;
            break;
    }
    return speed;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Check if a file exist                                         :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
bool fileExists(const char *fname) {
    FILE *file;
    if ((file = fopen(fname, "r"))) {
        fclose(file);
        return true;
    }
    return false;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Check if substring is contained into a string                 :*/
/*::  search: substring to find                                     :*/
/*::  content: string who maybe contains "search" arg               :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
bool strContains(const char *search, const char *content) {
    char *finded;
    if (!search || !content) return -1;
    finded = strstr(content, search);
    return finded ? true : false;
}

int strTokenCount(char *buffer, char token) {
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

char **strSplit(char *buffer, char *separator) {
    int i = 0;
    int tokenFinded = strTokenCount(buffer, (char) separator);
    char *p = strtok(buffer, separator);
    char **array[tokenFinded];
    while (p != NULL) {
        array[i] = p;
        i++;
        p = strtok(NULL, separator);
    }
    return array;
}