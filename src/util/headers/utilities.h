#ifndef UNIFI_FBW777_UTILITIES_H
#define UNIFI_FBW777_UTILITIES_H
#include <stdio.h>

/*:: Definition of Boolean Type  :*/
#define bool int
#define true 1
#define false 0


/*:: Generic Math Formulae :*/
double deg2rad(double);
double rad2deg(double);

/*:: String manipulation :*/
bool strContains(const char * search, const char *content);
void strSplit(char *buffer, const char *separator, char **array);
double str2double(char *string);
int strTokenCount(char *buffer, char token);

/*:: Mathematical function :*/
int random_number(int min_num, int max_num);
/*:: File manipulation :*/
bool fileExists(const char *fname);

/*:: Coordinate manipulation and algorithm :*/
double distanceBetweenPoints(double lat1, double lon1, double lat2, double lon2);
double speedBetweenPoints(int timestamp1, int timestamp2, double distance);


/*:: Improve Ui/Ux :*/
char *checkFileIntoMainArgs(int argc, char *argv[]);
void welcomeMessage();
#endif //UNIFI_FBW777_UTILITIES_H
