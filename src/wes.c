#include <unistd.h>
#include "headers/wes.h"
#include "headers/pfcDisconnectSwitch.h"
#include "util/headers/utilities.h"
#include "util/headers/constant.h"

/*::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Messages used for logging                :*/
/*::::::::::::::::::::::::::::::::::::::::::::::*/
#define WES_MSG_ERRPFC1   "Error at PFC #1 | has a different value"
#define WES_MSG_ERRPFC2   "Error at PFC #2 | has a different value"
#define WES_MSG_ERRPFC3   "Error at PFC #3 | has a different value"
#define WES_MSG_EMERGENCY "Emergency alert | WES obtained three different values"

/*::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Initialize every filePointer into Wes    :*/
/*::::::::::::::::::::::::::::::::::::::::::::::*/
void Wes__openFile(Wes *self, int index, char *fileName);

_Noreturn void Wes__startReading(Wes *self);

void Wes__compareAndLog(Wes *self, double xray, double yankee, double zulu);

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: LogAction allows to save every stdout output into file        :*/
/*::     Wes *self : Reference to Wes object                        :*/
/*::     char *action : a message that will be stored into file     :*/
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Wes__logAction(Wes *self, char *action);

void Wes__init(Wes *self) {
    fflush(stdout);
    char *fileNames[3] = {TRANSDUCERS_LOGFILE1, TRANSDUCERS_LOGFILE2, TRANSDUCERS_LOGFILE3};
    for (int index = 0; index < 3; index++) {
        Wes__openFile(self, index, fileNames[index]);
    }
    Wes__startReading(self);
}

Wes *Wes__create() {
    Wes *wes = (Wes *) malloc(sizeof(Wes));
    if (fileExists(WES_LOGFILE))
        unlink(WES_LOGFILE);
    return wes;
}

void Wes_setPFCDisconnectSwitch(Wes *self, PFCDisconnectSwitch *pds) {
    self->pds = pds;
    Wes__init(self);
}


void Wes__openFile(Wes *self, int index, char *fileName) {
    while (!fileExists(fileName)) {
        fprintf(stderr, "[ERR][WES]\tCould not load file %s\n", fileName);
    }
    self->fileData[index] = fopen(fileName, "r");
    if (!self->fileData[index]) {
        perror("[ERR][WES]\tError during opening file");
        exit(EXIT_FAILURE);
    }
    fflush(stdout);
}


_Noreturn void Wes__startReading(Wes *self) {
    double xray, yankee, zulu;
    double values[3];
    do {
        sleep(1);
        fscanf(self->fileData[0], "%lf", &xray);
        printf("wes is now reading from speedPFC%d value %f\n", 1, xray);
        fscanf(self->fileData[1], "%lf", &yankee);
        printf("wes is now reading from speedPFC%d value %f\n", 2, yankee);
        fscanf(self->fileData[2], "%lf", &zulu);
        printf("wes is now reading from speedPFC%d value %f\n", 3, zulu);
        fflush(stdout);
        Wes__compareAndLog(self, xray, yankee, zulu);
    } while (true);
}

void Wes__compareAndLog(Wes *self, double xray,
                        double yankee, double zulu) {
    if (fequal(yankee, xray) && fequal(xray, zulu)) {
        printf("\033[0;32m[WES]\t(OK)\t%f\t%f\t%f\n\033[0m", xray, yankee, zulu);
        fflush(stdout);
        Wes__logAction(self, "OK");
    } else if (xray != yankee && yankee != zulu && zulu != xray) {
        printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_EMERGENCY);
        fflush(stdout);

        Wes__logAction(self, WES_MSG_EMERGENCY);
        pds__handleMessage(self->pds, WES_EMERGENCY, ERRVAL);
    } else {
        if (!fequal(yankee, xray) && fequal(xray, zulu)) { //yankee is different
            printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_ERRPFC2);
            fflush(stdout);

            Wes__logAction(self, WES_MSG_ERRPFC2);
            pds__handleMessage(self->pds, WES_ERROR, 1);
        }
        if (fequal(yankee, xray) && !fequal(zulu, xray)) { //zulu is different
            printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_ERRPFC3);
            fflush(stdout);

            Wes__logAction(self, WES_MSG_ERRPFC3);
            pds__handleMessage(self->pds, WES_ERROR, 2);
        }
        if (!fequal(yankee, xray) && fequal(yankee, zulu)) { //yankee is different
            printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_ERRPFC1);
            fflush(stdout);

            Wes__logAction(self, WES_MSG_ERRPFC1);
            pds__handleMessage(self->pds, WES_ERROR, 0);
        }
    }
}


void Wes__logAction(Wes *self, char *action) {
    if (fileExists(WES_LOGFILE)) {
        self->logFile = fopen(WES_LOGFILE, "a");
    } else {
        self->logFile = fopen(WES_LOGFILE, "w");
    }
    fprintf(self->logFile, "%s\n", action);
    fclose(self->logFile);
}