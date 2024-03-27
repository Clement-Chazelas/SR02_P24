#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PIPE "fifo"

int pid;
struct sigaction act_fils, act_pere;
int fd;
char readbuf[20];

void fonct_fils(){
  printf("Fin du processus fils\n"); fflush(stdout);

}

void fonct_pere(){
  printf("Fin du gestionnaire de tache\n"); fflush(stdout);
  kill(pid, SIGUSR1);
  sleep(1);
}

int main(){
  pid = fork();
  if(pid == 0){
    printf("2\n"); fflush(stdout);
    act_fils.sa_handler = fonct_fils;
    sigaction(SIGUSR1, &act_fils, 0);
    printf("2\n"); fflush(stdout);
    /*
    mknod(PIPE, S_IFIFO | 0660, 0); // création du pipe
    fd= open(PIPE, O_RDONLY, 0); // ouvrir le pipe
    */
    while(1){
      pause();
      /*if (read(fd, &readbuf, sizeof(readbuf)) < 0){ // lire du pipe
        perror("Error reading pipe"); exit(1);
      }*/
    }
  }

  if (pid >0){
    printf("1\n"); fflush(stdout);
    act_pere.sa_handler = fonct_pere;
    sigaction(SIGINT, &act_pere, 0);

    while(1) pause();
  }
}
