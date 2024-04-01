#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include"tache.h"

#define PIPE "fifo"


int main(){
  int fd;
  tache writebuf = {1, 6};
  tache writebuf2 = {2, 5};


  fd= open(PIPE, O_WRONLY, 0);

  write(fd, &writebuf, sizeof(writebuf));
  sleep(2);
  write(fd, &writebuf2, sizeof(writebuf2));


}
