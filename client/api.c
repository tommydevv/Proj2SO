#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "api.h"
#include "common/constants.h"

int fdReq;
int fdResp;

int ems_setup(char const* req_pipe_path, char const* resp_pipe_path, char const* server_pipe_path) {
  
  //TODO: create pipes and connect to the server
  
  int OP_CODE = 1 ;
  
  if(mkfifo(req_pipe_path, 0666) != 0){
    if(errno != EEXIST){
      fprintf(stderr, "Failed to create request pipe\n");
      return 1;
    }
  }

  if(mkfifo(resp_pipe_path, 0666) != 0){
    if(errno != EEXIST){
      fprintf(stderr, "Failed to create response pipe\n");
      return 1;
    }
  }

  int fd = open(server_pipe_path, O_WRONLY);
  int a = write(fd, &OP_CODE, sizeof(OP_CODE));
  printf("%d\n", a);
  write(fd, req_pipe_path, 40);
  printf("%d\n", fd);
  write(fd, resp_pipe_path, 40);

  printf("%d\n", fd);

  //abrir req  para ja ficar aberto e ir escrever nas funcoes abaixo e resp pipe, dar read do response paara receber o sesh id 
  fdReq = open(req_pipe_path, O_WRONLY);
  fdResp = open(resp_pipe_path, O_RDONLY);
  

  if (close(fd) != 0){
    fprintf(stderr, "Failed to close server pipe\n");
    return 1;
  }

  if (unlink(fd != 0)){
    fprintf(stderr, "Failed to unlink server pipe\n");
    return 1;
  }


  return 0;
}

int ems_quit(void) { 
  //TODO: close pipes
  int OP_CODE = 2;
  write(fdReq, &OP_CODE, sizeof(int));

  if(close(fdReq) != 0){
    fprintf(stderr, "Failed to close request pipe\n");
    return 1;
  }

  if(unlink(fdReq) != 0){
    fprintf(stderr, "Failed to unlink request pipe\n");
    return 1;
  }

  if(close(fdResp) != 0){
    fprintf(stderr, "Failed to close response pipe\n");
    return 1;
  }

  if(unlink(fdResp) != 0){
    fprintf(stderr, "Failed to unlink response pipe\n");
    return 1;
  }

  return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols) { 
  //TODO: send create request to the server (through the request pipe) and wait for the response (through the response pipe)
  int OP_CODE = 3;

  write(fdReq, &OP_CODE, sizeof(int));
  write(fdReq, event_id, sizeof(event_id));
  write (fdReq, &num_rows, sizeof(num_rows));
  write (fdReq, &num_cols, sizeof(num_cols));

  return 0;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys) {

  //TODO: send reserve request to the server (through the request pipe) and wait for the response (through the response pipe)
  int OP_CODE = 4;
  
  write(fdReq, &OP_CODE, sizeof(int));
  write(fdReq, event_id, sizeof(event_id));
  write(fdReq, &num_seats, sizeof(num_seats));
  write(fdReq, &*xs, sizeof(*xs));
  write(fdReq, &*ys, sizeof(*ys));

  return 0;
}

int ems_show(int out_fd, unsigned int event_id) {
  //TODO: send show request to the server (through the request pipe) and wait for the response (through the response pipe)
  int OP_CODE = 5;
  write(fdReq, &OP_CODE, sizeof(int));
  write(fdReq, event_id, sizeof(event_id));


  return 0;
}

int ems_list_events(int out_fd) {
  //TODO: send list request to the server (through the request pipe) and wait for the response (through the response pipe)
  int OP_CODE = 6;

  write(fdReq, &OP_CODE, sizeof(OP_CODE));

  return 0;
}
