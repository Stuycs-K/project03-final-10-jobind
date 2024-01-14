//THIS IS THE SERVER
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include "player.h"
#include "nation.h"
#include "territory.h"
/*Accept a connection from a client
 *return the to_client socket descriptor
 *blocks until connection is made.
 */
int numprompt(){
    int d[1];
    char s[100];
    fgets(s,100,stdin);
    sscanf(s,"%d",d);
    return *d;
}
int server_tcp_handshake(int listen_socket){
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    //accept the client connection
    client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
    //printf("No block?\n");
    return client_socket;
}

int server_setup(){
    //use getaddrinfo
    struct addrinfo * hints, * results;
    hints = calloc(1,sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    hints->ai_flags = AI_PASSIVE; //only needed on server
    getaddrinfo(NULL, "9000", hints, &results);  //Server sets node to NULL
    //client: getaddrinfo("149.89.150.100", "9845", hints, &results);

    //create socket
    int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    //EXPERIMENTAL CODE
    int yes = 1;
    if ( setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 ) {
        printf("sockopt  error\n");
        printf("%s\n",strerror(errno));
        exit(-1);
    }
    //^^^^^^^
    bind(sd, results->ai_addr, results->ai_addrlen);

    //DO STUFF
    listen(sd,12);

    free(hints);
    freeaddrinfo(results);
    
    return sd;
}
char * phaseinfo(int year, int phase){
    char*pi=malloc(100*sizeof(char));
    char*p =malloc(100*sizeof(char));
    if(phase==0) strcpy(p,"Economy");
    if(phase==1) strcpy(p,"Diplomacy");
    if(phase==2) strcpy(p,"War");
    sprintf(pi,"Year: %d. Current Phase: %s\n",year,p);
    return pi;
}
char * stats(struct country* c){
    char*st=malloc(512*sizeof(char));
    sprintf(st,"Stats about country %s:\nGDP:%d\nWealth:%d\nMilitary size:%d\n",c->name,c->GDP,c->wealth,c->military);
    return st;
}
int main(){
    /*TESTING SECTION START
    printf("ABSTEST: %d, %d\n", abs(-88), abs(18));
    return 0;
    TESTING SECTION END*/

    int year, players, AIs, phase;
    year=0; phase=0;

  printf("Welcome to the server-side of RisC and DiplomaC! Please enter the number of clients that will be connecting: ");
    players = numprompt();
    printf("Waiting for connections...\n");
    if(players<=0){
        printf("No players, no game. Exiting...\n");
        return 0;
    }
    
    int ld = server_setup();
    int descs[players];
    for(int i=0; i<players; i++){
        descs[i]=server_tcp_handshake(ld);
    }
    printf("All connected!\n");

    struct country* curcountry;
    for(int i=0; i<players; i++){
        //curname = calloc(100,sizeof(char));
        curcountry = calloc(1,sizeof(struct country));
        read(descs[i],curcountry,sizeof(struct country));
        //printf("%d: %s\n", i, curname);
        printf("Initial data from nation %d received.\n", i);
    }
    AIs = rivalcount();
    struct country** AIlist = rivalbirth(AIs);
    //AIs = sizeof(AIlist);
    //printf("NUMBER OF AIS SELECTED: %d\n",AIs);
    for(int i=0; i<AIs; i++){
        char * t = aicountryname(i);
        strcpy(AIlist[i]->name,t);
    }
    //THE BIG LOOPOWSKI
    while(year<3){
      phase=0;
      while(phase!=3){
        for(int i=0; i<players; i++){
          write(descs[i],phaseinfo(year,phase),100*sizeof(char));
        }
        phase++;
      }
      year++;
    }
    return 0;
}