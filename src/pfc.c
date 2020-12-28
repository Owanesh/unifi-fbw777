#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/pfc.h"
#include "util/headers/utilities.h"
#include "util/headers/constant.h"
#include <signal.h>
#include <unistd.h>

bool updatePosition(PFC *self, double latitude, double longitude, int timestamp) {
    bool needUpdatePFCParameters = false;
    if (self->latitudes[0] == PFC_RESETVAL || self->longitudes[0] == PFC_RESETVAL) { // Maybe it's the first time
        self->latitudes[0] = latitude;
        self->longitudes[0] = longitude;
        self->timestamps[0] = timestamp;
        needUpdatePFCParameters = false;
    } else if (self->latitudes[0] != PFC_RESETVAL && self->latitudes[1] == PFC_RESETVAL) {// Second access
        self->latitudes[1] = latitude;
        self->longitudes[1] = longitude;
        self->timestamps[1] = timestamp;
        needUpdatePFCParameters = true;
    } else if (self->latitudes[0] != PFC_RESETVAL && self->latitudes[1] != PFC_RESETVAL) { //swap and update
        self->latitudes[0] = self->latitudes[1];
        self->longitudes[0] = self->longitudes[1];
        self->timestamps[0] = self->timestamps[1];
        self->latitudes[1] = latitude;
        self->longitudes[1] = longitude;
        self->timestamps[1] = timestamp;
        needUpdatePFCParameters = true;
    }
    return needUpdatePFCParameters;
}

void signalHandler(int sig) {
    switch (sig) {
        case SIGSTOP:
            break;
        case SIGINT:
            break;

        case SIGCONT:
            break;
        case SIGUSR1:
            // TODO: altera il valore del prossimo calcolo della velocità,
            //  effettuando un left shift di 2 bits della velocità calcolata
            //  una volta arrotondata all’intero più vicino.
            break;

    }
}

void PFC__init(PFC *self, char *filename, char *name) {
    self->name = name;
    // TODO; read first latitude and longitude and create a PFCParamenters set
    PFC__reset(self);
    if (!fileExists(filename)) {
        fprintf(stderr, "[ERR] Error during file opening '%s'\n", filename);
    } else {
        self->filename = filename;
        self->filesize = PFC_RESETVAL;
    }
}

void PFC__checkFilesize(PFC *self) {
    long prev = ftell(self->fpointer);
    fseek(self->fpointer, 0, SEEK_END); // seek to end of file
    long size = ftell(self->fpointer);
    if (self->filesize == PFC_RESETVAL) self->filesize = size;
    if (self->filesize != size) {
        fprintf(stderr, "[ERR] Filesize was changed in runtime\n");
        exit(EXIT_FAILURE);
    }
    fseek(self->fpointer, prev, SEEK_SET);
}

void PFCParameter__update(PFCParameter *self, float speed, float distance) {
    self->speed = speed;
    self->distance = distance;
}

void gpgll2PFCParameters(char *line, PFC *self) {
    int tokenFinded = strTokenCount(line, EMEA_SEP[0]);
    char **splittedLineBuffer[tokenFinded];
    strSplit(line, EMEA_SEP, (char **) splittedLineBuffer);
    if (updatePosition(self, str2double((char *) splittedLineBuffer[1]), str2double((char *) splittedLineBuffer[3]),
                       atoi((const char *) splittedLineBuffer[5]))) {
        self->param.distance = distanceBetweenPoints(self->latitudes[0], self->longitudes[0], self->latitudes[1],
                                                     self->longitudes[1], MEASURE_UNIT);

        self->param.speed = speedBetweenPoints(self->timestamps[0], self->timestamps[1], self->param.distance,
                                               MEASURE_UNIT);

    }


}

void PFC_read(PFC *self) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size = 0;
    self->fpointer = fopen(self->filename, "r");
    /* Get the first line of the file. */
    while (line_size >= 0) {
        line_size = getline(&line_buf, &line_buf_size, self->fpointer);
        /* Loop through until we are done with the file. */
        while (line_size >= 0) {
            line_count++;
            if (strContains(EMEA_GPGLL, line_buf)) {
                sleep(READ_SPEED); // once per second
                /*printf("\ni'm %s == Line[%06d]: chars=%06zd, buf size=%06zu, contents: %s ", self->name, line_count,
                       line_size, line_buf_size, line_buf);*/
                PFC__checkFilesize(self);
                gpgll2PFCParameters(line_buf, self);
                // printf("I'm %s and i'm at %f km/h and i've already done %f km\n", self->name, self->param.speed,
                // self->param.distance);

            }
            /*printf("\n\t I'm %s == Line[%06d]: chars=%06zd, buf size=%06zu, contents: %s ", self->name, line_count,
                line_size, line_buf_size, line_buf);*/
            line_size = getline(&line_buf, &line_buf_size, self->fpointer);
        }
    }
    /* Free the allocated line buffer */
    free(line_buf);
    line_buf = NULL;
    /* Close the file now that we are done with it */
    fclose(self->fpointer);
}

PFC *PFC__create(char *filename, char *name) {
    PFC *pfc = (PFC *) malloc(sizeof(PFC));
    PFC__init(pfc, filename, name);
    return pfc;
}

void PFC__reset(PFC *self) {
    if (self->fpointer != NULL) {
        fclose(self->fpointer);
        self->filesize = PFC_RESETVAL;
        self->filename = "";
    }
    self->latitudes[0] = PFC_RESETVAL;
    self->latitudes[1] = PFC_RESETVAL;
    self->longitudes[0] = PFC_RESETVAL;
    self->longitudes[1] = PFC_RESETVAL;
    self->timestamps[0] = PFC_RESETVAL;
    self->timestamps[1] = PFC_RESETVAL;
}

void PFC__destroy(PFC *pfc) {
    if (pfc) {
        PFC__reset(pfc);
        free(pfc);
    }
}

