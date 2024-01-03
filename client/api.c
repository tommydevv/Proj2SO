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
int session_id = 0;

char session_id[81];

int ems_setup(char const* req_pipe_path, char const* resp_pipe_path, char const* server_pipe_path) {
  
  //TODO: create pipes and connect to the server

  if(session_id >= MAX_SESSION_COUNT){

    return 1;
  } 
  
  char menssagem[128] = "1 ";
  
  if(mkfifo(req_pipe_path, 0666) != 0){
    if(errno != EEXIST){
      fprintf(stderr, "Failed to create request pipe\n");
      return 1;
    }
  }
  session_id ++;

  if(mkfifo(resp_pipe_path, 0666) != 0){
    if(errno != EEXIST){
      fprintf(stderr, "Failed to create response pipe\n");
      return 1;
    }
  }
  //abrir req  para ja ficar aberto e ir escrever nas funcoes abaixo e resp pipe, dar read do response paara receber o sesh id 
  fdReq = open(req_pipe_path, O_WRONLY);
  fdResp = open(resp_pipe_path, O_RDONLY);


  strcat(menssagem, req_pipe_path);
  strcat(menssagem, " ");
  strcat(menssagem, resp_pipe_path);
  strcat(menssagem, "\n");
  
  int fd = open(server_pipe_path, O_WRONLY);

  write(fd, menssagem, 128);

  

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
  char menssagem[1] = "2";

  write(fdReq, menssagem, sizeof(menssagem));

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

  session_id --;

  return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols) {
  //TODO: send create request to the server (through the request pipe) and wait for the response (through the response pipe)
  char menssagem[128];

  sprintf(menssagem, "3 %u %zu %zu\n", event_id, num_rows, num_cols);

  write(fdReq, menssagem, sizeof(menssagem));

  return 0;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys) {

  //TODO: send reserve request to the server (through the request pipe) and wait for the response (through the response pipe)
  char menssagem[128];

  sprintf(menssagem, "4 %u %zu %zu %zu\n", event_id, num_seats, *xs, *ys);

  write(fdReq, menssagem, sizeof(menssagem));

  return 0;
}

int ems_show(int out_fd, unsigned int event_id) {
  //TODO: send show request to the server (through the request pipe) and wait for the response (through the response pipe)
  char menssagem[128];

  sprintf(menssagem, "5 %u\n", event_id);

  write(fdReq, menssagem, sizeof(menssagem));


  return 0;
}

int ems_list_events(int out_fd) {
  //TODO: send list request to the server (through the request pipe) and wait for the response (through the response pipe)
  char menssagem[2] = "6 ";

  write(fdReq, menssagem, sizeof(menssagem));

  return 0;
}
