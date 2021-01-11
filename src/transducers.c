#include "headers/transducers.h"
#include <unistd.h>
#include <fcntl.h>

// TODO : {DOCS} write some documentation for each function below

void transducer__initFileLog(FILE *filePointer, const char *fileName);

void transducer__readFromSocket();

void transducer__speedLog(FILE *filePointer, char *fileName, double PFCSpeed);

void Transducer__init(Transducer *self, PFC *PFC_list[3]) {
    // set reference for all running PFCs
    self->PFC_list[0] = &PFC_list[0];
    self->PFC_list[1] = &PFC_list[1];
    self->PFC_list[2] = &PFC_list[2];

    // set headers of logfiles
    transducer__initFileLog(self->log_files[0], TRANSDUCERS_LOGFILE1);

    transducer__initFileLog(self->log_files[1], TRANSDUCERS_LOGFILE2);

    transducer__initFileLog(self->log_files[2], TRANSDUCERS_LOGFILE3);
}


void transducer__readFromChannel(Transducer *self, int channel, int channelType, char *fileName, int indexLogfile) {
    sleep(2); //sync
    static double res;
    do {
        if (channelType == SOCKCH || channelType == PIPECH) {
            read(channel, &res, sizeof(double));
            if (res < 0) {
                fprintf(stderr, "[ERR][Transducer]\tError while reading data'%s'\n",
                        Channel__extendedName(channelType));
            } else {
                transducer__speedLog(self->log_files[indexLogfile],
                                     fileName,
                                     res);
            }
        }// else read from file
    } while (res >= 0);
}


void transducer__readFromSocket(Transducer *self) {
    transducer__readFromChannel(self, self->communicationChannel.channel, SOCKCH,
                                TRANSDUCERS_LOGFILE1,
                                0);

}

void transducer__readFromPipe(Transducer *self) {
    transducer__readFromChannel(self, self->communicationChannel.channel, PIPECH,
                                TRANSDUCERS_LOGFILE2,
                                1);
}

void transducer__readFromFile(Transducer *self) {
    struct flock lock;
    lock.l_type = F_WRLCK;    /* read/write (exclusive) lock */
    lock.l_whence = SEEK_SET; /* base for seek offsets */
    lock.l_start = 0;         /* 1st byte in file */
    lock.l_len = 0;           /* 0 here means 'until EOF' */
    lock.l_pid = getpid();    /* process id */
    sleep(2); //sync
    static double res;
    do {
        sleep(1);
        fcntl(self->communicationChannel.channel, F_GETLK, &lock); /* sets lock.l_type to F_UNLCK if no write lock */
        if (lock.l_type != F_UNLCK)
            perror("[ERR][Transducer]\tFile is still write locked ");
        lock.l_type = F_RDLCK; /* prevents any writing during the reading */
        if (fcntl(self->communicationChannel.channel, F_SETLK, &lock) < 0)
            perror("[ERR][Transducer]\tCan't get a read-only lock ");
        while (read(self->communicationChannel.channel, &res, sizeof(double)) > 0) { /* 0 signals EOF */
            transducer__speedLog(self->log_files[2],
                                 TRANSDUCERS_LOGFILE3,
                                 res);
        }
        lock.l_type = F_UNLCK;
        if (fcntl(self->communicationChannel.channel, F_SETLK, &lock) < 0)
            perror("[ERR][Transducer]\tExplicit unlocking failed ");
    } while (res >= 0);
}

Transducer *Transducer__create() {
    Transducer *transducer = (Transducer *) malloc(sizeof(Transducer));
    return transducer;
}


/*:: Utilities :*/
void transducer__initFileLog(FILE *filePointer, const char *fileName) {
    unlink(fileName);
    filePointer = fopen(fileName, "w");
    fclose(filePointer);
}

void Transducer__setCommunicationChannel(Transducer *self, int channel, int channelType) {
    self->communicationChannel.channel = channel;
    self->communicationChannel.type = channelType;
    if (channelType == SOCKCH)
        transducer__readFromSocket(self);
    if (channelType == PIPECH)
        transducer__readFromPipe(self);
    if (channelType == FILECH)
        transducer__readFromFile(self);
}

void transducer__speedLog(FILE *filePointer, char *fileName, double PFCSpeed) {
    filePointer = fopen(fileName, "a");
    fprintf(filePointer, "%f\n", PFCSpeed + 1 - 1);
    fclose(filePointer);
}


