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
  tache writebuf = {3, 2};


  fd= open(PIPE, O_WRONLY, 0);

  write(fd, &writebuf, sizeof(writebuf));


}
