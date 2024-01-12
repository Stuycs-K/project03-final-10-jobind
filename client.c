//THIS IS THE CLIENT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "player.h"
#include "nation.h"
#include "territory.h"
int client_tcp_handshake(char * server_address) {
  struct addrinfo *hints, *results;
  hints = malloc(sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  //getaddrinfo
  getaddrinfo(server_address, "9000", hints, &results);
  //printf("getaddrinfo\n");
  int serverd;
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  //printf("socket\n");
  //connect to the server
  connect(serverd, results->ai_addr, results->ai_addrlen);
  //printf("connect\n");
  free(hints);
  freeaddrinfo(results);

  return serverd;
}

int main(){
    int sd = client_tcp_handshake(NULL);
    
    return 0;
}