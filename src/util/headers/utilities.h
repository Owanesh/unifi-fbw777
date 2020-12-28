#ifndef UNIFI_FBW777_UTILITIES_H
#define UNIFI_FBW777_UTILITIES_H

/*:: Definition of Boolean Type  :*/
#define bool int
#define true 1
#define false 0


/*:: Generic Math Formulae :*/
double deg2rad(double);
double rad2deg(double);

/*:: String manipulation :*/
bool strContains(const char * search, const char *content);
void strSplit(char *buffer, char *separator, char **array);
double str2double(char *string);
int strTokenCount(char *buffer, char token);

/*:: File manipulation :*/
bool fileExists(const char *fname);

/*:: Coordinate manipulation and algorithm :*/
double distanceBetweenPoints(double lat1, double lon1, double lat2, double lon2, char unit);
double speedBetweenPoints(int timestamp1, int timestamp2, double distance, char unit);

#endif //UNIFI_FBW777_UTILITIES_H
