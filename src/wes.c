#include <unistd.h>
#include "headers/wes.h"
#include "headers/pfcDisconnectSwitch.h"
#include "util/headers/utilities.h"
#include "util/headers/constant.h"


/** First PFC has provided a different values from second and third PFC*/
#define WES_MSG_ERRPFC1   "Error at PFC #1 | has a different value"
/** Second PFC has provided a different values from first and third PFC*/
#define WES_MSG_ERRPFC2   "Error at PFC #2 | has a different value"
/** Third PFC has provided a different values from second and first PFC*/
#define WES_MSG_ERRPFC3   "Error at PFC #3 | has a different value"
/** All of three PFC has different values*/
#define WES_MSG_EMERGENCY "Emergency alert | WES obtained three different values"

/**
*  Initialize all Wes.fileNames and every Wes.dataFiles
 * @param self reference to self "object" in memory
 */
void Wes__init(Wes *self);

/**
*  Initialize a filePointer in READ mode on provided FileName ad stores it into dataFiles attribute of Wes
 * @param self  reference to self "object" in memory
 * @param index  of array of dataFiles
 * @param fileName  Filename of file that will be opened
 */
void Wes__openFile(Wes *self, int index, char *fileName);

_Noreturn void Wes__startReading(Wes *self);

void Wes__compareAndLog(Wes *self, double xray, double yankee, double zulu);

/**
* LogAction allows to save every stdout output into file
*  @param self Reference to Wes object
*  @param action a message that will be stored into file
*/
void Wes__logAction(Wes *self, char *action);


void Wes__init(Wes *self) {
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

void Wes__setPFCDisconnectSwitch(Wes *self, PFCDisconnectSwitch *pds) {
    self->pds = pds;
    Wes__init(self);
}


void Wes__openFile(Wes *self, int index, char *fileName) {
    while (!fileExists(fileName)) {
        fprintf(stderr, "[ERR][WES]\tCould not load file %s\n", fileName);
    }
    self->dataFiles[index] = fopen(fileName, "r");
    if (!self->dataFiles[index]) {
        perror("[ERR][WES]\tError during opening file");
        exit(EXIT_FAILURE);
    }
    fflush(stdout);
}


_Noreturn void Wes__startReading(Wes *self) {
    double xray, yankee, zulu;
    do {
        sleep(1);
        fscanf(self->dataFiles[0], "%lf", &xray);
        printf("wes is now reading from speedPFC%d value %f\n", 1, xray);
        fscanf(self->dataFiles[1], "%lf", &yankee);
        printf("wes is now reading from speedPFC%d value %f\n", 2, yankee);
        fscanf(self->dataFiles[2], "%lf", &zulu);
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