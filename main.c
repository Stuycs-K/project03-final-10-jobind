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
//gets and returns a number from the user
int numprompt(){
    int d[1];
    char s[100];
    fgets(s,100,stdin);
    sscanf(s,"%d",d);
    return *d;
}
/*Accept a connection from a client
*return the to_client socket descriptor
*blocks until connection is made.
*/
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
//turns the phase int into a printable
char * phasetostring(int phase){
  char*p =malloc(100*sizeof(char));
  if(phase==0) strcpy(p,"Economy");
  if(phase==1) strcpy(p,"Diplomacy");
  if(phase==2) strcpy(p,"War");
  return p;
}
//constructs standard string "year: . phase: ."
char * phaseinfo(int year, int phase){
    char*pi=malloc(100*sizeof(char));
    char*p=phasetostring(phase);
    sprintf(pi,"Year: %d. Current Phase: %s\n",year,p);
    free(p);
    return pi;
}
int main(){
  //TESTING SECTION START
  //printf("ABSTEST: %lf, %lf\n", myabs(myrandomdouble()), myabs(myrandomdouble()));
  //printf("randsTEST. myrand: %d, myranddouble: %lf\n", myrandom(1)[0], myrandomdouble());
  //return 0;
  //TESTING SECTION END

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
    struct country** PCList = malloc(players*sizeof(struct country*));
    struct country* curcountry;
    for(int i=0; i<players; i++){
        //curname = calloc(100,sizeof(char));
        curcountry = calloc(1,sizeof(struct country));
        read(descs[i],curcountry,sizeof(struct country));
        curcountry->domain=calloc(7,sizeof(struct terr));
        for(int j=0; j<3; j++){
          char * pfilename = malloc(200*sizeof(char));
          sprintf(pfilename,"_%d_%d_",i,j);
          curcountry->domain[j]=*terrbirth(curcountry->dif,pfilename);
        }
        //printf("%d: %s\n", i, curname);
        PCList[i]=curcountry;
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
      //each phase gets cycled through
      while(phase!=3){
        //each player gets a turn for each phase
        for(int i=0; i<players; i++){
          //each turn can have multiple commands
          while(1){
            char*phinf=phaseinfo(year,phase);
            write(descs[i],phinf,100*sizeof(char));
            char * curcmd = malloc(1000*sizeof(char));
            read(descs[i],curcmd,1000*sizeof(char));
            //if the player types help (anyphase)
            if(strcmp(curcmd,"help\n")==0||strcmp(curcmd,"Help\n")==0){
              char* helpstr = helper(phase);
              write(descs[i],helpstr,1000*sizeof(char));
              free(helpstr);
            //if the player types finish (anyphase)
            } else if(strcmp(curcmd,"finish\n")==0){
              char* finishstr = calloc(1000,sizeof(char));
              sprintf(finishstr,"Finished with phase %s.\n",phasetostring(phase));
              write(descs[i],finishstr,1000*sizeof(char));
              free(finishstr);
              break; //DEVIOUS USE OF BREAK
              
            //otherwise, process the command (phase-dependant)
            } else{
              char * towritecmd = cmdhandler(curcmd,phase,PCList[i]);//REPLACE w/ cmd handler
              write(descs[i],towritecmd,1000*sizeof(char));
            }
            free(curcmd);
            free(phinf);
          }
        }
        phase++;
      }
      year++;
    }
    return 0;
}