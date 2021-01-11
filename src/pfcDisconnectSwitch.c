#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char checkStatusOf(pid_t pid) {
    int link[2];
    pid_t execPid;
    char statusCode[5 + 1];
    char strPid[7];
    sprintf(strPid, "%d", pid);
    if (pipe(link) == -1) {
        fprintf(stderr, "%s\n", "[ERR][PFCDS]\tError while opening Pipe");
        exit(EXIT_FAILURE);
    }
    if ((execPid = fork()) == -1) {
        fprintf(stderr, "%s\n", "[ERR][PFCDS]\tError while create a child");
        exit(EXIT_FAILURE);
    }
    if (execPid == 0) {
        dup2(link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);
        execl("/bin/ps", "/bin/ps", strPid, "-o", "stat", NULL);
        fprintf(stderr, "%s\n", "[ERR][PFCDS]\tError on execl()");
        exit(EXIT_FAILURE);
    } else {
        close(link[1]);
        read(link[0], statusCode, sizeof(statusCode));
        wait(NULL);
    }
    return statusCode[5];
}

void pds__handleMessage(int typeOfMessage, int extraInfo){
    //TODO: if emergency message, stop the application, reboot process otherwise
}

