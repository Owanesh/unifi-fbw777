#include <stdio.h>
#include <stdlib.h>
#include "headers/pfc.h"
#include "headers/failureGenerator.h"
#include "headers/transducers.h"
#include "util/headers/utilities.h"
#include "util/headers/signals.h"
#include <signal.h>
#include <sys/socket.h>
int main(int argc, char *argv[]) {
    Pipe pfcs[3];
    pipe(pfcs[0].pipe);
    pipe(pfcs[1].pipe);
    pipe(pfcs[2].pipe);

    int sock_trans_pfc[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sock_trans_pfc) < 0) {
        perror("[ERR] Can't open socket pair ");
        exit(1);
    }

    Pipe trans_pfc;
    pipe(trans_pfc.pipe);

    char *filename = checkFileIntoMainArgs(argc, argv);
    printf("Data location : %s\n", filename);
    welcomeMessage();

    pid_t PFC_pid_list[3];
    pid_t failureGen_pid;
    pid_t transducers_pid;

    PFC *PFC_list[3];
    PFC_list[0] = PFC__create(filename, "Alpha");
    PFC_list[1] = PFC__create(filename, "Bravo");
    PFC_list[2] = PFC__create(filename, "Charlie");


    if (!(PFC_pid_list[0] = fork())) {
        PFC_list[0]->selfPid = getpid();
        signal(SIGUSR1, handle_sigUSR1);

        close(sock_trans_pfc[0]);
        PFC__setComunicationChannel(PFC_list[0],sock_trans_pfc[1], SOCKCH);

        close(pfcs[0].pipe[0]);
        write(pfcs[0].pipe[1], PFC_list[0], sizeof(PFC *));
        close(pfcs[0].pipe[1]);

        sleep(2);
        PFC_read(PFC_list[0]);
        PFC__destroy(PFC_list[0]);


        close(sock_trans_pfc[1]);
        //exit(0);

    } else if (!(PFC_pid_list[1] = fork())) {
        PFC_list[1]->selfPid = getpid();
        signal(SIGUSR1, handle_sigUSR1);

        close(trans_pfc.pipe[0]);
        PFC__setComunicationChannel(PFC_list[1], trans_pfc.pipe[1], PIPECH);

        close(pfcs[1].pipe[0]);
        write(pfcs[1].pipe[1], PFC_list[1], sizeof(PFC *));
        close(pfcs[1].pipe[1]);

        sleep(2);
        PFC_read(PFC_list[1]);
        PFC__destroy(PFC_list[1]);

        close(trans_pfc.pipe[1]);

        //exit(0);

    } else if (!(PFC_pid_list[2] = fork())) {
        PFC_list[2]->selfPid = getpid();
        signal(SIGUSR1, handle_sigUSR1);

        close(pfcs[2].pipe[0]);
        write(pfcs[2].pipe[1], PFC_list[2], sizeof(PFC *));
        close(pfcs[2].pipe[1]);

        sleep(2);
        PFC_read(PFC_list[2]);
        PFC__destroy(PFC_list[2]);
        //exit(0);

    } else {
        printf("[log] Aeroplanetty.pid %d\n", getpid());

        close(pfcs[0].pipe[1]);
        read(pfcs[0].pipe[0], PFC_list[0], sizeof(PFC *));
        close(pfcs[0].pipe[0]);

        close(pfcs[1].pipe[1]);
        read(pfcs[1].pipe[0], PFC_list[1], sizeof(PFC *));
        close(pfcs[1].pipe[0]);

        close(pfcs[2].pipe[1]);
        read(pfcs[2].pipe[0], PFC_list[2], sizeof(PFC *));
        close(pfcs[2].pipe[0]);

        if (!(failureGen_pid = fork())) {
            FailureGen *fGen = FailureGen__create();
            FailureGen__init(fGen, PFC_list);
        }
        if (!(transducers_pid = fork())) {
            pid_t trans_sock, trans_pipe, trans_file;
            Transducer *transducer = Transducer__create();
            Transducer__init(transducer, PFC_list);
            if (!(trans_sock = fork())) {
                close(sock_trans_pfc[1]);
                Transducer__setComunicationChannel(transducer,sock_trans_pfc[0], SOCKCH);
                close(sock_trans_pfc[0]);
            }
            if (!(trans_pipe = fork())) {
                close(trans_pfc.pipe[1]);
                Transducer__setComunicationChannel(transducer, trans_pfc.pipe[0], PIPECH);
            }
        }

        wait(&PFC_pid_list[0]);
        wait(&PFC_pid_list[1]);
        wait(&PFC_pid_list[2]);
    }
    return 0;
}