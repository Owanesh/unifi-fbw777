#ifndef UNIFI_FBW777_WES_H
#define UNIFI_FBW777_WES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
      int *logFiles[3];
      pid_t selfPid;
} Wes;

void Wes__start(Wes *self);
Wes *Wes__create();
void     compare_values(double xray, double yankee, double zulu);
#endif //UNIFI_FBW777_WES_H
