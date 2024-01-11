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

    free(hints);
    freeaddrinfo(results);
    
    return sd;
}
int main(){
    int year = 0;
    printf("\nWelcome to RisC and DiplomaC! Defeat the rival nations and take control!\n");
    //initiates data for the player nation
    struct country* playernation = birth();
    //initiates the data for the rivals
    struct country** rivals = rivalbirth();
    //terrainInit
    //**REPLACE W/ TERRAIN GEN
    //Below: Initializes diplomatic relations between all nations, semi-randomly
    //**REPLACE W/ DIPLOM. GEN
    
    //Main Loop (player):
    //**LOOP BODY HERE
    //End of main Loop (player):
    
    //****WIN-LOSE STATE****
    int ld = server_setup();
    return 0;
}