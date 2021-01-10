#ifndef UNIFI_FBW777_WES_H
#define UNIFI_FBW777_WES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
      FILE *logFiles[3];
      pid_t selfPid;
} Wes;

_Noreturn void Wes__startReading(Wes *self, char *fnames[3]);
void Wes__start(Wes *self);

Wes *Wes__create();
void     compare_values(double xray, double yankee, double zulu);
#endif //UNIFI_FBW777_WES_H
