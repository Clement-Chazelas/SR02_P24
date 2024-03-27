#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PIPE "fifo"
#define MAX_TASK_LENGTH 20

int pid;
struct sigaction act_fils, act_pere;
int fd;
char readbuf[MAX_TASK_LENGTH];

void fonct_fils(){
    printf("Fin du processus fils\n");
    fflush(stdout);
    exit(0);
}

void fonct_pere(){
    printf("Fin du gestionnaire de tâche\n");
    fflush(stdout);
    kill(pid, SIGUSR1);
    sleep(1);
    exit(0);
}

void process_task(char *task){
    pid_t task_pid;
    task_pid = fork();
    if (task_pid < 0) {
        perror("Erreur lors de la création du processus pour la tâche");
        exit(EXIT_FAILURE);
    } else if (task_pid == 0) {
        printf("Processus fils pour la tâche : Exécution de la tâche : %s\n", task);
        sleep(2); //Simulation exécution tâche
        // Envoi d'un message de retour
        printf("Processus fils pour la tâche : Tâche terminée\n");
        exit(0);
    }
}

int main(){
    pid = fork();
    int bytes_read;
  
    if(pid == 0){
        act_fils.sa_handler = fonct_fils;
        sigaction(SIGUSR1, &act_fils, 0);
        
        mknod(PIPE, S_IFIFO | 0660, 0); // création du pipe
        fd = open(PIPE, O_RDONLY, 0); // ouvrir le pipe
        
        while(1){
            bytes_read = read(fd, &readbuf, sizeof(readbuf)); // lire du pipe
            if (bytes_read < 0){ 
                perror("Error reading pipe");
                exit(1);
            }
            else if (bytes_read > 0) {
                printf("Gestionnaire de tâches : Demande de tâche reçue : %s\n", readbuf);
                process_task(readbuf);
            }
        }
    }

    if (pid > 0){
        act_pere.sa_handler = fonct_pere;
        sigaction(SIGINT, &act_pere, 0);

        while(1) pause();
    }

    return 0;
}



/* Version avec la structure Task, pas encore fonctionelle

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PIPE "fifo"
#define MAX_TASK_LENGTH 20

int pid;
struct sigaction act_fils, act_pere;
int fd;
char readbuf[MAX_TASK_LENGTH];

struct Task {
    int id;
    int execution_time;
};


void fonct_fils(){
    printf("Fin du processus fils\n");
    fflush(stdout);
    exit(0);
}

void fonct_pere(){
    printf("Fin du gestionnaire de tâche\n");
    fflush(stdout);
    kill(pid, SIGUSR1);
    sleep(1);
    exit(0);
}

void process_task(struct Task task){
    pid_t task_pid;
    task_pid = fork();
    if (task_pid < 0) {
        perror("Erreur lors de la création du processus pour la tâche");
        exit(EXIT_FAILURE);
    } else if (task_pid == 0) {
        printf("Processus fils pour la tâche : ID %d, Temps d'exécution %d\n", task.id, task.execution_time);
        sleep(task.execution_time); // Exécution de la tâche
        printf("Processus fils pour la tâche : Tâche terminée\n");
        exit(0);
    }
}


int main(){
    pid = fork();
    int bytes_read;
    struct Task task;
  
    if(pid == 0){
        act_fils.sa_handler = fonct_fils;
        sigaction(SIGUSR1, &act_fils, 0);
    
        mknod(PIPE, S_IFIFO | 0660, 0); // création du pipe
        fd= open(PIPE, O_RDONLY, 0); // ouvrir le pipe
    
        while(1){
            pause();
            bytes_read = read(fd, &task, sizeof(task)); // lire depuis le pipe
            if (bytes_read < 0){ // gestion d'erreur
                perror("Error reading pipe");
                exit(1);
            }
            else if (bytes_read > 0) {
                printf("Gestionnaire de tâches : Demande de tâche reçue : ID %d, Temps d'exécution %d\n", task.id, task.execution_time);
                process_task(task); // traiter la tâche
            }
        }
    }

    if (pid >0){
        act_pere.sa_handler = fonct_pere;
        sigaction(SIGINT, &act_pere, 0);

        while(1) pause();
    }

    return 0;
}
*/
