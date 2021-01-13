#include "headers/transducers.h"
#include <unistd.h>
#include <fcntl.h>


void transducer__initFileLog(Transducer *self, int indexLogFile, const char *fileName);

void transducer__readFromSocket();

void transducer__speedLog(Transducer *self, int indexLogFile, char *fileName, double PFCSpeed);

void Transducer__init(Transducer *self) {
    // set headers of logfiles
    transducer__initFileLog(self, 0, TRANSDUCERS_LOGFILE1);

    transducer__initFileLog(self, 1, TRANSDUCERS_LOGFILE2);

    transducer__initFileLog(self, 2, TRANSDUCERS_LOGFILE3);
}


void transducer__readFromChannel(Transducer *self, int channel, int channelType, char *fileName, int indexLogfile) {
    sleep(2); //sync
    static double res;
    do {
        sleep(1);
        if (channelType == SOCKCH || channelType == PIPECH) {
            read(channel, &res, sizeof(double));
            if (res < 0) {
                fprintf(stderr, "[ERR][Transducer]\tError while reading data'%s'\n",
                        Channel__extendedName(channelType));
                fflush(stderr);
            } else {
                transducer__speedLog(self, indexLogfile,
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

        fsync(self->communicationChannel.channel);
        read(self->communicationChannel.channel, &res, sizeof(double)); /* 0 signals EOF */
        fflush(stdout);
        transducer__speedLog(self, 2,
                             TRANSDUCERS_LOGFILE3,
                             res);

        lock.l_type = F_UNLCK;
        if (fcntl(self->communicationChannel.channel, F_SETLK, &lock) < 0)
            perror("[ERR][Transducer]\tExplicit unlocking failed ");
    } while (res >= 0);
}

Transducer *Transducer__create() {
    Transducer *transducer = (Transducer *) malloc(sizeof(Transducer));
    return transducer;
}


/** Utilities :*/
void transducer__initFileLog(Transducer *self, int indexLogFile, const char *fileName) {
    unlink(fileName);
    self->log_files[indexLogFile] = fopen(fileName, "w");
    fclose(self->log_files[indexLogFile]);
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

void transducer__speedLog(Transducer *self, int indexLogFile, char *fileName, double PFCSpeed) {
    self->log_files[indexLogFile] = fopen(fileName, "a");
    fprintf(self->log_files[indexLogFile], "%f\n", PFCSpeed);
    fflush(self->log_files[indexLogFile]);
    fclose(self->log_files[indexLogFile]);
}


