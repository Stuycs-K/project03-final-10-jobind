//THIS IS THE SERVER
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

    bind(sd, results->ai_addr, results->ai_addrlen);

    //DO STUFF
    listen(sd,12);

    free(hints);
    freeaddrinfo(results);
    
    return sd;
}
int main(){
    /*TESTING SECTION START
    printf("ABSTEST: %d, %d\n", abs(-88), abs(18));
    return 0;
    TESTING SECTION END*/

    int year, players, AIs;

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

    return 0;
}