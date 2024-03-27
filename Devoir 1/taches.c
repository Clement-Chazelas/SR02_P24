#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>


#define PIPE "fifo"


int main(){
  int fd;
  char writebuf[20] = "hello"; // ouvrir le pipe
  fd= open(PIPE, O_WRONLY, 0); // écrire dans le pipe
  write(fd, writebuf, sizeof(writebuf));
  sleep(2);
  char writebuf2[20] = "no";
  write(fd, writebuf2, sizeof(writebuf));

}
