#include <math.h>
#include "headers/utilities.h"
#include "headers/constant.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define pi 3.14159265358979323846

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  South latitudes are negative, east longitudes are positive           :*/
/*::    lat1, lon1 = Latitude and Longitude of point 1 (in decimal degrees)  :*/
/*::    lat2, lon2 = Latitude and Longitude of point 2 (in decimal degrees)  :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double distanceBetweenPoints(double lat1, double lon1, double lat2, double lon2) {
    double earthRadius_km = EARTH_RADIUS*1e3;
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
double str2double(char *string) {
    char *rest;
    double ret;
    ret = strtod(string, &rest);
    return ret;
}

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Calculate speed between two points with timestamp and distance         :*/
/*::      timestamp1 = Timestamp at point 1                                  :*/
/*::      timestamp2 = Timestamp at point 2                                  :*/
/*::      distance = calculated distance between points                      :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double speedBetweenPoints(int timestamp1, int timestamp2, double distance) {
    return timestamp2 > timestamp1?( distance / (timestamp2 - timestamp1)):0;
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
/*::      search: substring to find                                 :*/
/*::      content: string who maybe contains "search" arg           :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
bool strContains(const char *search, const char *content) {
    char *finded;
    if (!search || !content) return -1;
    finded = strstr(content, search);
    return finded ? true : false;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Check how many char (token) are contained into string         :*/
/*::      token: character to looking for                           :*/
/*::      buffer: string who maybe contains "token" char            :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int strTokenCount(char *buffer, const char token) {
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

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Split a string divided by separator. this fill an array :*/
/*::      buffer: string that will be splitted                :*/
/*::      separator: delimiter of each substring              :*/
/*::      array : will contains substrings                    :*/
/*::      returns : an array with substring in each element   :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void strSplit(char *buffer, const char *separator, char **array) {
    int i = 0;
    char *p = strtok(buffer, separator);
    while (p != NULL) {
        array[i] = p;
        i++;
        p = strtok(NULL, separator);
    }
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Calculate random integer between two limits         :*/
/*::      min_num: min number allowed for a random value  :*/
/*::      max_num: max number allowed for a random value  :*/
/*::      returns : min < (number) < max                  :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int random_number(int min_num, int max_num) {
    int result = 0, low_num = 0, hi_num = 0;
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

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function prints only an header for this program          :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
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

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Gets filename by argv array passed to application             :*/
/*::   ./aeroplanetty <filepath_g18.tx>                             :*/
/*::      if filepath is invalid or file does not exist, will use   :*/
/*::      internal file stored in /resources/G18.txt                :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
char *checkFileIntoMainArgs(int argc, char *argv[]){
    if (argc == 2) {
        printf("The argument supplied is %s\n", argv[1]);
        if (fileExists(argv[1])) {
            return argv[1];
        } else {
            printf("File %s not exist or is invalid. Using default...\n",argv[1]);
        }
    } else if (argc > 2) {
        printf("Too many arguments supplied  - Using default...\n");
    } else {
        printf("No external G18 was provided - Using default...\n");
    }
    return "../resources/G18.txt";
}