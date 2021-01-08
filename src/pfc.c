#include "headers/pfc.h"
/*:: IPC :*/
#include <sys/socket.h>
#include <sys/un.h> /* For AF_UNIX sockets */

int shifter;

// TODO : {DOCS} write some documentation for each function below

void PFC__init(PFC *self, char *filename, char *name);

void PFC__reset(PFC *self);

void PFC__backupFPointer(PFC *self);


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: UpdatePosition allows to update arrays referred to PFCs GPGLL */
/*::     PFC *self : Reference to PFC object                        */
/*::     Other parameters according to GPGLL:                       */
/*::        double latitude, longitude                              */
/*::        int timestamp                                           */
/*::     Return: bool -> necessity to update PFCParameters          */
/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
bool updatePosition(PFC *self, double latitude, double longitude, long timestamp) {
    if (self->latitudes[0] == PFC_RESETVAL || self->longitudes[0] == PFC_RESETVAL) { // Maybe it's the first time
        self->latitudes[0] = latitude;
        self->longitudes[0] = longitude;
        self->timestamps[0] = timestamp;
    } else if (self->latitudes[0] != PFC_RESETVAL && self->latitudes[1] == PFC_RESETVAL) {// Second access
        self->latitudes[1] = latitude;
        self->longitudes[1] = longitude;
        self->timestamps[1] = timestamp;
        return true;
    } else if (self->latitudes[0] != PFC_RESETVAL && self->latitudes[1] != PFC_RESETVAL) { //swap and update
        self->latitudes[0] = self->latitudes[1];
        self->longitudes[0] = self->longitudes[1];
        self->timestamps[0] = self->timestamps[1];
        self->latitudes[1] = latitude;
        self->longitudes[1] = longitude;
        self->timestamps[1] = timestamp;
        return true;
    }
    return false;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Set speed and distance into PFCParameter of self pointer      :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void PFCParameter__update(PFC *self, double speed, double distance) {
    speed = self->needShift ? ((int) speed << 2) : speed;
    self->param.speed = speed > +0 ? speed : 0; //prevent negative speed or attack
    self->param.distance = distance > +0 ? distance : 0;
}

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Convert a GPGLL line into parameters for PFC reference pointer :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void gpgll2PFCParameters(char *line, PFC *self) {
    int tokenFinded = strTokenCount(line, EMEA_SEP[0]);
    char **splittedLineBuffer[tokenFinded];
    strSplit(line, EMEA_SEP, (char **) splittedLineBuffer);
    if (updatePosition(self,
                       str2double((char *) splittedLineBuffer[1]),
                       str2double((char *) splittedLineBuffer[3]),
                       atol((char *) splittedLineBuffer[5]))) {
        double speed = speedBetweenPoints(self->timestamps[0],
                                          self->timestamps[1],
                                          self->param.distance);
        double distance = distanceBetweenPoints(self->latitudes[0],
                                                self->longitudes[0],
                                                self->latitudes[1],
                                                self->longitudes[1]);
        PFCParameter__update(self, speed, distance);
    }
}

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Security check. Filesize is duplicated in each PFC, if anyone  :*/
/*::  modify a line during the execution of program, it stops with   :*/
/*::  exit(EXIT_FAILURE);                                            :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void PFC__checkFilesize(PFC *self) {
    long prev = ftell(self->filePointer);
    fseek(self->filePointer, 0, SEEK_END); // seek to end of file
    long size = ftell(self->filePointer);
    if (self->filesize == PFC_RESETVAL) self->filesize = size;
    if (self->filesize != size) {
        fprintf(stderr, "[ERR] [%s] Filesize was changed in runtime\n", self->name);
        exit(EXIT_FAILURE);
    }
    fseek(self->filePointer, prev, SEEK_SET);
}


void PFC_read(PFC *self) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size = 0;
    self->filePointer = fopen(self->filename, "r");
    /* Get the first line of the file. */
    while (line_size >= 0) {
        line_size = getline(&line_buf, &line_buf_size, self->filePointer);
        /* Loop through until we are done with the file. */
        while (line_size >= 0) {
            line_count++;
            if (shifter == self->selfPid) {
                self->needShift = true;
                shifter = SHIFTER_RESET;
            }
            if (strContains(EMEA_GPGLL, line_buf)) {
                usleep(READ_SPEED);
                PFC__backupFPointer(self);
                PFC__checkFilesize(self);
                gpgll2PFCParameters(line_buf, self);
            }
            line_size = getline(&line_buf, &line_buf_size, self->filePointer);
        }
    }

    free(line_buf); // Free the allocated line buffer
    line_buf = NULL;
    fclose(self->filePointer); // Close the file now that we are done with it
}

void PFC__init(PFC *self, char *filename, char *name) {
    self->name = name;
    PFC__reset(self);
    if (!fileExists(filename)) {
        fprintf(stderr, "[ERR] Error during file opening '%s'\n", filename);
    } else {
        self->filename = filename;
        self->filesize = FILESIZE_RESET;
    }
}

PFC *PFC__create(char *filename, char *name) {
    PFC *pfc = (PFC *) malloc(sizeof(PFC));
    PFC__init(pfc, filename, name);
    pfc->selfPid = getpid();
    return pfc;
}

void PFC__destroy(PFC *self) {
    if (self) {
        printf("Destroying %s PFC who has pid %d \n", self->name, self->selfPid);
        PFC__reset(self);
        kill(SIGQUIT, self->selfPid);
        free(self);
    }
}


void PFC__backupFPointer(PFC *self) {
    self->seekPoint = ftell(self->filePointer);
    if (self->com.type == SOCKCH) {
        PFC_log(self);
    }
}


void PFC__reset(PFC *self) {
    self->com.type = -1;

    if (self->filePointer != NULL) {
        fclose(self->filePointer);
        self->filesize = FILESIZE_RESET;
        self->filename = "";
        self->seekPoint = FILESIZE_RESET;
    }
    self->latitudes[0] = PFC_RESETVAL;
    self->latitudes[1] = PFC_RESETVAL;
    self->longitudes[0] = PFC_RESETVAL;
    self->longitudes[1] = PFC_RESETVAL;
    self->timestamps[0] = PFC_RESETVAL;
    self->timestamps[1] = PFC_RESETVAL;
}


void PFC_log(PFC *self) {
   //printf("[%s}.speed : %f\n",self->name, self->param.speed);
    if (write(self->com.channel, (&self->param.speed), sizeof(double)) < 0)
        perror("writing stream message");
    fflush(stdout);
}

void PFC__setComunicationChannel(PFC *self, int channel, int channelType) {
    self->com.channel = channel;
    self->com.type = channelType;
}
