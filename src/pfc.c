#include "headers/pfc.h"
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "util/headers/signals.h"

/**
 * Creates enough space in memory to allow this struct allocation
 * @param self reference to self "object" in memory
 * @param filename name of file used to reads GPGLL data
 * @param name a formal name for pfc,also used for logging as metadata
 */
void PFC__init(PFC *self, char *filename, char *name);


/**
 * Implements a backup system of filePointer. Stores seekPoint into internal attribute of PFC
 * @param self reference to self "object" in memory
 */
void PFC__backupFPointer(PFC *self);

/**
* UpdatePosition allows to update arrays referred to PFCs GPGLL
*
* @return  a boolean value represents necessity to update PFCParameters
*/
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

/**
*  Set speed and distance into PFCParameter of self pointer
 * @param self reference to self "object" in memory
 * @param speed double value indicates a cruise speed | see PFCParameters
 * @param distance double value indcates a traveled distance | see PFCParameters
*/
void PFCParameter__update(PFC *self, double speed, double distance) {
    if (shifter == self->selfPid) {
        speed = ((int) speed << 2);
        shifter = SHIFTER_RESET;
    }
    self->param.speed = speed >= 0 ? speed : 0; //prevent negative speed or attack
    self->param.distance = distance > +0 ? distance : 0;
}

/**
*  Convert a GPGLL line into parameters for PFC reference pointer
 * @param line row of file that contains the data
 * @param self reference to self "object" in memory
*/
void gpgll2PFCParameters(char *line, PFC *self) {
    int tokenFound = strTokenCount(line, EMEA_SEP[0]);
    char **splittedLineBuffer[tokenFound];
    strSplit(line, EMEA_SEP, (char **) splittedLineBuffer);
    if (updatePosition(self,
                       str2double((char *) splittedLineBuffer[1]),
                       str2double((char *) splittedLineBuffer[3]),
                       atol((char *) splittedLineBuffer[5]))) {

        double distance = distanceBetweenPoints(self->latitudes[0],
                                                self->longitudes[0],
                                                self->latitudes[1],
                                                self->longitudes[1]);
        double speed = speedBetweenPoints(self->timestamps[0],
                                          self->timestamps[1],
                                          distance);
        PFCParameter__update(self, speed, distance);
    }
}


void PFC__checkFilesize(PFC *self) {
    long prev = ftell(self->filePointer);
    fseek(self->filePointer, 0, SEEK_END); // seek to end of file
    long size = ftell(self->filePointer);
    if (self->fileSize == PFC_RESETVAL) self->fileSize = size;
    if (self->fileSize != size) {
        fprintf(stderr, "[ERR][PFC][%s]\tFilesize was changed in runtime\n", self->name);
        exit(EXIT_FAILURE);
    }
    fseek(self->filePointer, prev, SEEK_SET);
}


void PFC_read(PFC *self) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size = 0;
    self->filePointer = fopen(self->fileName, "r");
    /* Get the first line of the file. */
    while (line_size >= 0) {
        line_size = getline(&line_buf, &line_buf_size, self->filePointer);
        /* Loop through until we are done with the file. */
        while (line_size >= 0) {
            line_count++;

            if (strContains(EMEA_GPGLL, line_buf)) {
                usleep(READ_SPEED);
                PFC__checkFilesize(self);
                gpgll2PFCParameters(line_buf, self);
                PFC__backupFPointer(self);

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
        fprintf(stderr, "[ERR][PFC]\tError during file opening '%s'\n", filename);
    } else {
        self->fileName = filename;
        self->fileSize = FILESIZE_RESET;
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
        fclose(self->filePointer);
        printf("Destroying %s PFC who has pid %d \n", self->name, self->selfPid);
        PFC__reset(self);
        free(self);
        kill(self->selfPid, SIGQUIT);
    }
    exit(0);
}


void PFC__backupFPointer(PFC *self) {
    self->seekPoint = ftell(self->filePointer);
    PFC_log(self);
}


void PFC__reset(PFC *self) {
    if (self->filePointer != NULL) {
        fclose(self->filePointer);
        self->fileSize = FILESIZE_RESET;
        self->fileName = "";
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
    fflush(stdout);
    if (self->com.type == SOCKCH || self->com.type == PIPECH) {
        if (write(self->com.channel, (&self->param.speed), sizeof(double)) < 0) {
            perror("[ERR][PFC]\tCan't log information");
            exit(0);
        }
    }
    if (self->com.type == FILECH) {
        struct flock lock;
        lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
        lock.l_whence = SEEK_SET; /* base for seek offsets */
        lock.l_start = 0;         /* 1st byte in file */
        lock.l_len = 0;           /* 0 here means 'until EOF' */
        lock.l_pid = self->selfPid;    /* process id */
        if (fcntl(self->com.channel, F_SETLK, &lock) < 0) {
            perror("[ERR][PFC]\tFailed to get lock");
            exit(0);
        } else {
            fsync(self->com.channel);
            if (write(self->com.channel, (&self->param.speed), sizeof(double)) < 0) {
                perror("[ERR][PFC]\tCan't log information");
                exit(0);
            } else {
                lock.l_type = F_UNLCK;
                if (fcntl(self->com.channel, F_SETLK, &lock) < 0)
                    perror("[ERR][PFC]\tExplicit unlocking failed");
            }
        }
    }


}

void PFC__setCommunicationChannel(PFC *self, int channel, int channelType) {
    self->com.channel = channel;
    self->com.type = channelType;
    self->com.meta = Channel__extendedName(channelType);
}

