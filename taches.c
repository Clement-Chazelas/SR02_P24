#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "tache.h"

#define PIPE "fifo"

int pid;
struct sigaction act_gestionnaire, act_pere, act_tache;
int fd;

void fonct_gestionnaire() {
    printf("Fin du gestionnaire de tache\n");
    fflush(stdout);
    exit(0);
}

void fonct_pere() {
    printf("Fin du gestionnaire de tache\n");
    fflush(stdout);
    if (pid != 0) {
        kill(pid, SIGUSR1); // Envoie un signal au processus enfant pour qu'il arrête la tâche
    }
    exit(0);
}

void fonct_tache() {
    exit(0);
}

void exec_tache(tache* tache) {
    pid_t tache_pid;

    tache_pid = fork();
    if (tache_pid < 0) {
        perror("Erreur lors de la création du processus pour la tâche");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    else if (tache_pid == 0) {
        pid_t timer_pid;
        timer_pid = fork();
        if (timer_pid == 0) {
            // Timer qui vérifie qu'une tache ne dépasse pas 5 secondes
            int t = 0;
            while (t < 5) {
                sleep(1);
                t++;
            }
            if (kill(getppid(), SIGUSR2) == 0) {
                printf("Fin de la tache %d car execution >5s \n", tache->id);
                fflush(stdout);
            }
            exit(0);
        }
        else if (timer_pid > 0) {
            // Handler qui gère l'interruption d'une tache trop longue
            act_tache.sa_handler = fonct_tache;
            sigaction(SIGUSR2, &act_tache, 0);
            printf("Executeur de tâche : ID %d, Temps d'exécution : %ds\n", tache->id, tache->duree_exec);
            fflush(stdout);
            // Simulation d'une execution de tache
            sleep(tache->duree_exec);
            printf("Executeur de tâche : Tâche %d terminée\n", tache->id);
            fflush(stdout);
            exit(0);
        }
    }
}

int main() {
    pid = fork();
    if (pid == 0) {
        act_gestionnaire.sa_handler = fonct_gestionnaire;
        sigaction(SIGUSR1, &act_gestionnaire, 0);
        mknod(PIPE, S_IFIFO | 0660, 0); // création du pipe
        fd = open(PIPE, O_RDONLY, 0); // ouvrir le pipe
        tache readtache = {-1,-1};
        while (1) {
            if (read(fd, &readtache, sizeof(tache)) < 0) { // lire du pipe
                perror("Error reading pipe");
                exit(1);
            }
            if (readtache.duree_exec != -1) {
                exec_tache(&readtache);
                readtache.duree_exec = -1;
            }
        }
    }
    if (pid > 0) {
        act_pere.sa_handler = fonct_pere;
        sigaction(SIGINT, &act_pere, 0);
        while (1) {
            pause();
        }
    }
}
