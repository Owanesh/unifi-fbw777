#include <stdio.h>
#include <stdlib.h>
#include "headers/pfc.h"

void PFC__init(PFC *self, char *filename) {
    // open file with filename
    // read first latitude and longitude and create a PFCParamenters set
    self->latitude[0] = 1.1;
    self->filename = filename;
}

void PFC__checkfilesize(PFC *self) {
    long prev = ftell(self->fpointer);
    fseek(self->fpointer, 0, SEEK_END); // seek to end of file
    long size = ftell(self->fpointer);
    self->filesize = (self->filesize != 0)?:size;
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

void PFC_verbose(PFC *self) {
    printf("[debug] PFC_debug \n");
    printf("\tPFCparam-> speed \t %f\n", self->param.speed);
    printf("\tPFCparam-> distance \t %f\n", self->param.distance);
    printf("\tLatitude [0](last) \t %f\n", self->latitude[0]);
    printf("\tLatitude [1](current) \t %f\n", self->latitude[1]);
    printf("\tLongitude [0](last) \t %f\n", self->longitude[0]);
    printf("\tLongitude [1](current) \t %f\n", self->longitude[1]);
    printf("[debug/end] PFC_debug\n");
}
int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}


void PFC_read(PFC *self) {
    char c[1000];
    self->fpointer = fopen(self->filename, "r");
    fscanf(self->fpointer, "%[^\n]", c);
    printf("Data from the file:\n%s", c);
    fclose(self->fpointer);
}

PFC *PFC__create(char *filename) {
    PFC *pfc = (PFC *) malloc(sizeof(PFC)); //also include PFCParameter (8byte)
    PFC__init(pfc, filename);

    // read from file
    // create pfcparameter with speed and distance calculated
    // PFCParameter__update(&(pfc->param), 1.4, 5.3);
    return pfc;
}


void PFC__reset(PFC *self) {
    /* remove any filepointer linked */
}

void PFC__destroy(PFC *pfc) {
    if (pfc) {
        PFC__reset(pfc);
        free(pfc);
    }
}

