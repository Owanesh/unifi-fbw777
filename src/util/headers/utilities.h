#ifndef UNIFI_FBW777_UTILITIES_H
#define UNIFI_FBW777_UTILITIES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Pipe struct is useful to manage better Pipe IPC   :*/
/*::  Also useful to work with array of pipe            :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
typedef struct {
    int pipe[2];   // 0 = read, 1 = write
} Pipe;


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Channel struct is useful to obtain more information :*/
/*::  about IPC. And improve it workflow                  :*/
/*::    type : refers to type of channel                  :*/
/*::    channel : pointer to channel (file/pipe/socket)   :*/
/*::    meta : a short extra information about channel    :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
typedef struct {
    int channel;
    enum {
        UNDEFINEDCH, SOCKCH, PIPECH, FILECH
    } type;
    char *meta;
} Channel;


char *Channel__extendedName(int channelType);

/*:: Generic Math Formulae :*/

double deg2rad(double);

__unused double rad2deg(double);

/*:: String manipulation :*/

bool strContains(const char *search, const char *content);

void strSplit(char *buffer, const char *separator, char **array);

double str2double(char *string);

int strTokenCount(const char *buffer, char token);

__unused int str2i(const char *str);

/*:: Mathematical function :*/

int random_number(int min_num, int max_num);

bool fequal(double a, double b);

/*:: File manipulation :*/
bool fileExists(const char *filename);

/*:: Coordinate manipulation and algorithm :*/

double distanceBetweenPoints(double lat1, double lon1, double lat2, double lon2);

double speedBetweenPoints(int timestamp1, int timestamp2, double distance);


/*:: Improve Ui/Ux :*/

char *checkFileIntoMainArgs(int argc, char *argv[]);

void welcomeMessage();

/*:: Array manipulation :*/

int getIndexOfPFCList(pid_t PFC_pid, pid_t *PFC_list, int position);

/*: InterProcess communication :*/

__unused int make_named_socket(const char *filename);

#endif