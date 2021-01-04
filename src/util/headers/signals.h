#ifndef UNIFI_FBW777_SIGNALS_H
#define UNIFI_FBW777_SIGNALS_H
#include <signal.h>
#include <unistd.h>

extern int shifter;


void handle_sigUSR1(int num);

#endif //UNIFI_FBW777_SIGNALS_H
