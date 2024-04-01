#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include"tache.h"

#define PIPE "fifo"

int pid;
struct sigaction act_gestionnaire, act_pere;
int fd;


void fonct_gestionnaire(){
  sleep(1);
  printf("Yahouu\n" );fflush(stdout);

}

void fonct_pere(){
  printf("Fin du gestionnaire de tache\n"); fflush(stdout);
  kill(pid, SIGUSR1);

}

void exec_tache(tache* tache){
    pid_t tache_pid;
    int	status;
    tache_pid = fork();
    if (tache_pid < 0) {
        perror("Erreur lors de la création du processus pour la tâche");fflush(stdout);
        exit(EXIT_FAILURE);
    } else if (tache_pid == 0) {

        printf("Executeur de tâche : ID %d, Temps d'exécution : %ds\n", tache->id, tache->duree_exec);fflush(stdout);
        sleep(tache->duree_exec); // Exécution de la tâche
        printf("Executeur de tâche : Tâche terminée\n");fflush(stdout);

        exit(0);
    }
    else if (tache_pid > 0){
        waitpid(tache_pid, &status, 0);
        printf("fils attendu" );fflush(stdout);

    }
}

int main(){
  pid = fork();
  if(pid == 0){

    act_gestionnaire.sa_handler = fonct_gestionnaire;
    sigaction(SIGUSR1, &act_gestionnaire, 0);

    mknod(PIPE, S_IFIFO | 0660, 0); // création du pipe
    fd= open(PIPE, O_RDONLY, 0); // ouvrir le pipe

    tache readtache = {-1,-1};


    while(1){
      sleep(1);
      if (read(fd, &readtache, sizeof(tache)) < 0){ // lire du pipe
        perror("Error reading pipe");fflush(stdout); exit(1);
      }
      //if (readtache != null) !!!!
      if(readtache.duree_exec != -1){

        exec_tache(&readtache);
        readtache.duree_exec = -1;

      }

    }
  }

  if (pid >0){

    act_pere.sa_handler = fonct_pere;
    sigaction(SIGINT, &act_pere, 0);

    while(1) pause();
  }
}
