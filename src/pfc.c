#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/pfc.h"
#include "util/headers/utilities.h"
#include "util/headers/constant.h"
#include <signal.h>
#include <unistd.h>

void _Noreturn updatePosition(PFC *self, double latitude, double longitude, int timestamp) {
    if (self->latitudes[0] == PFC_RESETVAL || self->longitudes[0] == PFC_RESETVAL) { // Maybe it's the first time
        self->latitudes[0] = latitude;
        self->longitudes[0] = longitude;
        self->timestamps[0] = timestamp;
    } else if (self->latitudes[0] != PFC_RESETVAL && self->latitudes[1] == PFC_RESETVAL) {// Second access
        self->latitudes[1] = latitude;
        self->longitudes[1] = longitude;
        self->timestamps[1] = timestamp;
    } else if (self->latitudes[0] != PFC_RESETVAL && self->latitudes[1] != PFC_RESETVAL) { //swap and update
        self->latitudes[0] = self->latitudes[1];
        self->longitudes[0] = self->longitudes[1];
        self->timestamps[0] = self->timestamps[1];
        self->latitudes[1] = latitude;
        self->longitudes[1] = longitude;
        self->timestamps[1] = timestamp;
    }
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

void PFC__init(PFC *self, char *filename) {
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

void _Noreturn PFCParameter__update(PFCParameter *self, float speed, float distance) {
    self->speed = speed;
    self->distance = distance;
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
                sleep(1);
                PFC__checkFilesize(self);
                //char **splittedLineBuffer = strSplit(line_buf, ",");
            }
            //printf("Line[%06d]: chars=%06zd, buf size=%06zu, contents: %s ", line_count,
            //       line_size, line_buf_size, line_buf);
            line_size = getline(&line_buf, &line_buf_size, self->fpointer);
        }
    }
    /* Free the allocated line buffer */
    free(line_buf);
    line_buf = NULL;
    /* Close the file now that we are done with it */
    fclose(self->fpointer);
}

PFC *PFC__create(char *filename) {
    PFC *pfc = (PFC *) malloc(sizeof(PFC)); //also include PFCParameter (8byte)
    PFC__init(pfc, filename);
    // PFC_read(pfc);
    // create pfcparameter with speed and distance calculated
    return pfc;
}

void PFC__reset(PFC *self) {
    if (self->fpointer != NULL) {
        fclose(self->fpointer);
        self->filesize = PFC_RESETVAL;
        self->filename = "";
    }
    self->latitudes[0], self->latitudes[1] = PFC_RESETVAL;
    self->longitudes[0], self->longitudes[1] = PFC_RESETVAL;
    self->timestamps[0], self->timestamps[1] = PFC_RESETVAL;
}

void PFC__destroy(PFC *pfc) {
    if (pfc) {
        PFC__reset(pfc);
        free(pfc);
    }
}

