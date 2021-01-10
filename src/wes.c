#include <fcntl.h>
#include "headers/wes.h"
#include "util/headers/utilities.h"
#include "util/headers/constant.h"

void Wes__openFile(Wes *self, int index, char *fileName);

void compare_values(double xray, double yankee, double zulu) {
    if (fequal(yankee, xray) && fequal(xray, zulu)) {
        printf("[WES]\tOK\t%f\t%f\t%f\n", xray, yankee, zulu);
    } else if (xray != yankee && yankee != zulu && zulu != xray) {
        //TODO: Send signal of emergency to PFC Disconnect Switch
        printf("[WES]\tEMERGENCY ERROR\n");
    } else {
        if (!fequal(yankee, xray) && fequal(xray, zulu)) { //yankee is different
            printf("[WES]\tYANKEE IS DIFFERENT\n");
        }
        if (fequal(yankee, xray) && !fequal(zulu, xray)) { //yankee is different
            printf("[WES]\tZULU IS DIFFERENT\n");
        }
        if (!fequal(yankee, xray) && fequal(yankee, zulu)) { //yankee is different
            printf("[WES]\tXRAY IS DIFFERENT\n");
        }
    }
    fflush(stdout);
}


void Wes__start(Wes *self) {
    fflush(stdout);
    char *fnames[3] = {TRANSDUCERS_LOGFILE1, TRANSDUCERS_LOGFILE2, TRANSDUCERS_LOGFILE3};
    Wes__openFile(self, 0, fnames[0]);
    Wes__openFile(self, 1, fnames[1]);
    Wes__openFile(self, 2, fnames[2]);

    Wes__startReading(self, fnames);

}

Wes *Wes__create() {
    Wes *wes = (Wes *) malloc(sizeof(Wes));
    wes->selfPid = getpid();
    fflush(stdout);
    return wes;
}

_Noreturn void Wes__startReading(Wes *self, char *fnames[3]) {
    double xray = -1, yankee = -1, zulu = -1;
    double *tempSpeed;
    tempSpeed = (double *) malloc(sizeof(double));
    do {
        sleep(1);
        for (int index = 0; index < 3; index++) {
            fscanf(self->logFiles[index], "%lf", tempSpeed);
            printf("[WES] READ %f\tFROM\t%s\n", (*tempSpeed), fnames[index]);
            fflush(stdout);
            if (index == 0)
                xray = *tempSpeed;
            if (index == 1)
                yankee = *tempSpeed;
            if (index == 2)
                zulu = *tempSpeed;
            fflush(stdout);
        }//end forLoop
        compare_values(xray, yankee, zulu);
    } while (true);
}

void Wes__openFile(Wes *self, int index, char *fileName) {
    if (!fileExists(fileName)) {
        fprintf(stderr, "[ERR][WES]\tCould not load file %s\n", fileName);
    }
    self->logFiles[index] = fopen(fileName, "r");
    if (!self->logFiles[index]) {
        perror("[ERR][WES]\tError during opening file");
        exit(EXIT_FAILURE);
    }
    fflush(stdout);
}