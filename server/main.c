#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "common/constants.h"
#include "common/io.h"
#include "operations.h"

int main(int argc, char* argv[]) {

  int fd;
  char buffer[40];
  char buffer2[40];

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
  if(unlink(argv[1]) != 0){
    fprintf(stderr, "Failed to unlink register pipe\n");
    return 1;
  } 

  if (mkfifo(argv[1], 0666) != 0){
    return 1;
    
  }

  while (1) {
    int OP_CODE;
    //TODO: Read from pipe
    fd = open(argv[1], O_RDWR);
    printf("%d\n", fd);

    if (read(fd, &OP_CODE, sizeof(int)) < 0){
      fprintf(stderr, "Failed to read from register pipe\n");
      return 1;
    }

    if(read(fd, buffer, sizeof(buffer))< 0){
      fprintf(stderr, "Failed to read from register pipe\n");
      return 1;
    }

    if(read(fd, buffer2, sizeof(buffer2))< 0){
      fprintf(stderr, "Failed to read from register pipe\n");
      return 1;
    }

    //TODO: Write new client to the producer-consumer buffer
  }

  //TODO: Close Server
  if(close(fd) != 0){
    fprintf(stderr, "Failed to close register pipe\n");
    return 1;
  }



  ems_terminate();
}