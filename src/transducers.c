#include "headers/transducers.h"
#include "util/headers/utilities.h"
#include "util/headers/constant.h"
#include <stdlib.h>
// TODO : {DOCS} write some documentation for each function below

void transducer__initFileLog(FILE *filePointer, const char *fileName);

void transducer__readFromSocket();

void transducer__speedLog(FILE *filePointer, char *fileName, int PFCPid, char *PFCName, double PFCSpeed);

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


void transducer__readFromChannel(Transducer *self, int channel,int channelType, char *fileName, int indexLogfile, int indexPFC) {
    sleep(2); //sync
    static double res;
    do {
        if (channelType==SOCKCH || channelType==PIPECH) {
            read(channel, &res, sizeof(double));
            if (res < 0) {
                perror("[ERR] Transducer.connectAndReadFromSocket ");
            } else {
                //printf("[%d]\treads\t[%f]\t[%d] \n",getpid(),res,channelType);
                transducer__speedLog(self->log_files[indexLogfile],
                                     fileName,
                                     (*self->PFC_list[indexPFC])->selfPid,
                                     (*self->PFC_list[indexPFC])->name,
                                     res);
            }
        }// else read from file
    } while (res >= 0);
}


void transducer__readFromSocket(Transducer *self) {
    transducer__readFromChannel(self, self->comunicationChannel.channel,SOCKCH,
                                TRANSDUCERS_LOGFILE1,
                                0, 0);

}

void transducer__readFromPipe(Transducer *self) {
    transducer__readFromChannel(self, self->comunicationChannel.channel,PIPECH,
                                TRANSDUCERS_LOGFILE2,
                                1, 1);
}

Transducer *Transducer__create() {
    Transducer *transducer = (Transducer *) malloc(sizeof(Transducer));
    return transducer;
}


/*:: Utilities :*/
void transducer__initFileLog(FILE *filePointer, const char *fileName) {
    if (fileExists(fileName))
        unlink(fileName);
    filePointer = fopen(fileName, "w");
    fprintf(filePointer, "------------------------\n");
    fprintf(filePointer, "Process\tName\tSpeed\n");
    fclose(filePointer);
}

void Transducer__setComunicationChannel(Transducer *self, int channel, int channelType) {
    self->comunicationChannel.channel = channel;
    self->comunicationChannel.type = channelType;
    if (channelType == SOCKCH) {
        transducer__readFromSocket(self);
    }
    if (channelType == PIPECH) {
        transducer__readFromPipe(self);
    }
}

void transducer__speedLog(FILE *filePointer, char *fileName, int PFCPid, char *PFCName, double PFCSpeed) {
    if (fileExists(fileName)) {
        filePointer = fopen(fileName, "a");
    } else {
        filePointer = fopen(fileName, "w");
    }
    fprintf(filePointer, "%d\t%s\t%f\n", PFCPid, PFCName, PFCSpeed);
    fclose(filePointer);
}
