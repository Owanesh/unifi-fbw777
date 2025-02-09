#include "headers/pfc.h"
#include "headers/wes.h"
#include "headers/failureGenerator.h"
#include "headers/transducers.h"
#include "headers/pfcDisconnectSwitch.h"
#include "util/headers/signals.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    Pipe PFCs[3];
    pipe(PFCs[0].pipe);
    pipe(PFCs[1].pipe);
    pipe(PFCs[2].pipe);

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
    pid_t failureGen_pid, transducers_pid, wes_pid;

    PFC *PFC_list[3];
    PFC_list[0] = PFC__create(filename, "Alpha");
    PFC_list[1] = PFC__create(filename, "Bravo");
    PFC_list[2] = PFC__create(filename, "Charlie");



    if (!(PFC_pid_list[0] = fork())) {
        PFC_list[0]->selfPid = getpid();
        signal(SIGUSR1, handle_sigUSR1);


        close(sock_trans_pfc[0]);
        PFC__setCommunicationChannel(PFC_list[0], sock_trans_pfc[1], SOCKCH);

        close(PFCs[0].pipe[0]);
        write(PFCs[0].pipe[1], PFC_list[0], sizeof(PFC *));
        close(PFCs[0].pipe[1]);

        sleep(2);
        PFC_read(PFC_list[0]);
        PFC__destroy(PFC_list[0]);


        close(sock_trans_pfc[1]);
        exit(0);

    } else if (!(PFC_pid_list[1] = fork())) {
        PFC_list[1]->selfPid = getpid();
        signal(SIGUSR1, handle_sigUSR1);

        close(trans_pfc.pipe[0]);
        PFC__setCommunicationChannel(PFC_list[1], trans_pfc.pipe[1], PIPECH);

        close(PFCs[1].pipe[0]);
        write(PFCs[1].pipe[1], PFC_list[1], sizeof(PFC *));
        close(PFCs[1].pipe[1]);

        sleep(2);
        PFC_read(PFC_list[1]);
        PFC__destroy(PFC_list[1]);

        close(trans_pfc.pipe[1]);

        exit(0);

    } else if (!(PFC_pid_list[2] = fork())) {
        PFC_list[2]->selfPid = getpid();
        signal(SIGUSR1, handle_sigUSR1);

        int fileDescriptor;
        unlink(TRANPFC_FILE);
        while ((fileDescriptor = open(TRANPFC_FILE, O_RDWR | O_CREAT, 0666)) < 0)  /* -1 signals an error */
        {
            perror("[ERR][PFC #3] Open failed...");
        }
        PFC__setCommunicationChannel(PFC_list[2], fileDescriptor, FILECH);

        close(PFCs[2].pipe[0]);
        write(PFCs[2].pipe[1], PFC_list[2], sizeof(PFC *));
        close(PFCs[2].pipe[1]);
        sleep(2);
        PFC_read(PFC_list[2]);
        PFC__destroy(PFC_list[2]);

        close(fileDescriptor);

        exit(0);

    } else {
        close(PFCs[0].pipe[1]);
        read(PFCs[0].pipe[0], PFC_list[0], sizeof(PFC *));
        close(PFCs[0].pipe[0]);

        close(PFCs[1].pipe[1]);
        read(PFCs[1].pipe[0], PFC_list[1], sizeof(PFC *));
        close(PFCs[1].pipe[0]);

        close(PFCs[2].pipe[1]);
        read(PFCs[2].pipe[0], PFC_list[2], sizeof(PFC *));
        close(PFCs[2].pipe[0]);

        if (!(failureGen_pid = fork())) {
            FailureGen *fGen = FailureGen__create();
            FailureGen__init(fGen, PFC_list);
            exit(0);
        }
        if (!(transducers_pid = fork())) {
            pid_t trans_sock, trans_pipe, trans_file;
            Transducer *transducer = Transducer__create();
            Transducer__init(transducer);
            if (!(trans_sock = fork())) {
                close(sock_trans_pfc[1]);
                Transducer__setCommunicationChannel(transducer, sock_trans_pfc[0], SOCKCH);
                close(sock_trans_pfc[0]);
            }
            if (!(trans_pipe = fork())) {
                close(trans_pfc.pipe[1]);
                Transducer__setCommunicationChannel(transducer, trans_pfc.pipe[0], PIPECH);
                close(trans_pfc.pipe[0]);
            }
            if (!(trans_file = fork())) {
                int fileDescriptor;
                if ((fileDescriptor = open(TRANPFC_FILE, O_RDONLY)) < 0)  // -1 signals an error
                {
                    perror("[ERR][Transducer]\tCan't open file");
                    exit(0);
                } else {
                    Transducer__setCommunicationChannel(transducer, fileDescriptor, FILECH);
                }
                close(fileDescriptor);
            }
            free(transducer);
            exit(0);

        }
       if (!(wes_pid = fork())) {
            PFCDisconnectSwitch *pds;
            pds = PDS__create(PFC_list);

            sleep(3);

            Wes *wes = Wes__create();
            Wes__setPFCDisconnectSwitch(wes,pds);
            free(wes);
            exit(0);
        }

        wait(&failureGen_pid);
        wait(&transducers_pid);
        wait(&wes_pid);
        wait(&PFC_pid_list[0]);
        wait(&PFC_pid_list[1]);
        wait(&PFC_pid_list[2]);
    }
    return 0;
}