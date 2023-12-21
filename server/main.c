#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "client/main.c"
#include "common/constants.h"
#include "common/io.h"
#include "operations.h"

int main(int argc, char* argv[]) {

  int fd;

  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s\n <pipe_path> [delay]\n", argv[0]);
    return 1;
  }

  char* endptr;
  unsigned int state_access_delay_us = STATE_ACCESS_DELAY_US;

  if (argc > 2) {
    unsigned long int delay = strtoul(argv[2], &endptr, 10);



    if (*endptr != '\0' || delay > UINT_MAX) {
      fprintf(stderr, "Invalid delay value or value too large\n");
      return 1;
    }

    state_access_delay_us = (unsigned int)delay;
  }


  if (ems_init(state_access_delay_us)) {
    fprintf(stderr, "Failed to initialize EMS\n");
    return 1;
  }

  //TODO: Intialize server, create worker threads

  if (mkfifo(argv[1], 0666) != 0){

    if(errno != EEXIST){

      fprintf(stderr, "Failed to create register pipe\n");
      return 1;
      
    }
  }

  while (1) {
    //TODO: Read from pipe
    fd = open(argv[1], O_RDWR);
    //TODO: Write new client to the producer-consumer buffer
  }

  //TODO: Close Server
  close(fd);

  ems_terminate();
}