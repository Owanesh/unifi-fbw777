#include <fcntl.h>
#include "headers/wes.h"
#include "util/headers/utilities.h"
#include "util/headers/constant.h"

void Wes__initializeFilePointers(Wes *self, int index, char *fileName);

_Noreturn void Wes__startReading(Wes *self);

void compare_values(double xray, double yankee, double zulu) {
    if (yankee == xray && xray == zulu) {
        printf("[WES]\tOK\t%f\t%f\t%f\n", xray, yankee, zulu);
    } else if (xray != yankee && yankee != zulu && zulu != xray) {
        //TODO: Send signal of emergency to PFC Disconnect Switch
        printf("[WES]\tEMERGENCY ERROR");
    } else {
        if (yankee != xray && xray == zulu) { //yankee is different
            printf("[WES]\tYANKEE IS DIFFERENT\n");
        }
        if (yankee == xray && xray != zulu) { //yankee is different
            printf("[WES]\tZULU IS DIFFERENT\n");
        }
        if (yankee != xray && yankee == zulu) { //yankee is different
            printf("[WES]\tXRAY IS DIFFERENT\n");
        }
    }
    fflush(stdout);
}


void Wes__start(Wes *self) {
    // initialize three filePointers
    //read value after header
    // repeat till EOF
    Wes__initializeFilePointers(self, 0, TRANSDUCERS_LOGFILE1);
    Wes__initializeFilePointers(self, 1, TRANSDUCERS_LOGFILE2);
    Wes__initializeFilePointers(self, 2, TRANSDUCERS_LOGFILE3);
    Wes__startReading(self);

}

Wes *Wes__create() {
    Wes *wes = (Wes *) malloc(sizeof(Wes));
    wes->selfPid = getpid();
    return wes;
}

_Noreturn void Wes__startReading(Wes *self) {
    double xray = -1, yankee = -1, zulu = -1;
    double *tempSpeed;
    tempSpeed = (double *) malloc(sizeof(double));
    int goon;
    do {
        sleep(1);
        for (int index = 0; index < 3; index++) {
            goon=read(*self->logFiles[index], tempSpeed, sizeof(double));
            printf("[WES] Readed %d element : %f\n",goon,(*tempSpeed));
            switch (index) {
                case 0:
                    xray = *tempSpeed;
                    break;
                case 1:
                    yankee = *tempSpeed;
                    break;
                case 2:
                    zulu = *tempSpeed;
                    break;

            }
            fflush(stdout);
        }//end forLoop
        compare_values(xray, yankee, zulu);
    } while (goon != EOF);
}

void Wes__initializeFilePointers(Wes *self, int index, char *fileName) {
    if (!fileExists(fileName)) {
        fprintf(stderr, "[ERR][WES]\tCould not load file %s", fileName);
    }
    *self->logFiles[index] = open(fileName, O_RDONLY);
    if (!self->logFiles[index]) {
        perror("[ERR][WES]\tError during opening file");
        exit(EXIT_FAILURE);
    }

}