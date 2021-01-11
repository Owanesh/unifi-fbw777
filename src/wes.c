#include "headers/wes.h"
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
    wes->selfPid = getpid();
    fflush(stdout);
    if (fileExists(WES_LOGFILE))
        unlink(WES_LOGFILE);
    Wes__init(wes);
    return wes;
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
    double tempSpeed;
    double values[3];
    do {
        sleep(1);
        for (int index = 0; index < 3; index++) {
            fscanf(self->fileData[index], "%lf", &tempSpeed);
            values[index] = tempSpeed;
        }
        Wes__compareAndLog(self, values[0], values[1], values[2]);
    } while (true);
}

void Wes__compareAndLog(Wes *self, double xray, double yankee, double zulu) {
    if (fequal(yankee, xray) && fequal(xray, zulu)) {
        printf("\033[0;32m[WES]\t(OK)\t%f\t%f\t%f\n\033[0m", xray, yankee, zulu);
        Wes__logAction(self, "OK");
    } else if (xray != yankee && yankee != zulu && zulu != xray) {
        // TODO: Send signal of emergency to PFC Disconnect Switch
        printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_EMERGENCY);
        Wes__logAction(self, WES_MSG_EMERGENCY);
    } else {
        if (!fequal(yankee, xray) && fequal(xray, zulu)) { //yankee is different
            printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_ERRPFC2);
            Wes__logAction(self, WES_MSG_ERRPFC2);
        }
        if (fequal(yankee, xray) && !fequal(zulu, xray)) { //zulu is different
            printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_ERRPFC3);
            Wes__logAction(self, WES_MSG_ERRPFC3);
        }
        if (!fequal(yankee, xray) && fequal(yankee, zulu)) { //yankee is different
            printf("\033[0;31m[WES]\t%s\033[0m\n", WES_MSG_ERRPFC1);
            Wes__logAction(self, WES_MSG_ERRPFC1);
        }
    }
    fflush(stdout);
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