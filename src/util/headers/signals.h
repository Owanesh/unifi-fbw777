#ifndef UNIFI_FBW777_SIGNALS_H
#define UNIFI_FBW777_SIGNALS_H

#include "../../headers/pfc.h"

typedef void (*sighandler_t)(int);

sighandler_t handle_sigUSR1(int num);

#endif //UNIFI_FBW777_SIGNALS_H
