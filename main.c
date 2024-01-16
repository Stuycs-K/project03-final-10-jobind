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
int first(char*cmd, char*comp){
  char*f=malloc((strlen(cmd)+1)*sizeof(char));
  char*l=malloc((strlen(cmd)+1)*sizeof(char));
  strcpy(l,cmd);
  f=strsep(&l," ");
  //free(l);
  if(strcmp(f,comp)==0) return 0;
  return 1;
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
        AIlist[i]->GDP=130*AIlist[i]->dif;
        AIlist[i]->wealth=150*(AIlist[i]->dif+1);
    }
    //THE BIG LOOPOWSKI
    while(year<10){
      phase=0;
      //each phase gets cycled through
      while(phase!=3){
        //each player gets a turn for each phase
        int deadcount=0;
        for(int i=0; i<players; i++){
          if(PCList[i]->dif==0){
            deadcount++;
          }
        }
        if(deadcount==players){
          printf("GAME HAS ENDED. EVERYONE IS DEAD.\n");
          return 0;
        }
        for(int i=0; i<AIs; i++){
          if(AIlist[i]->dif==0){
            deadcount++;
          }
        }
        printf("Deadcount: %d\n",deadcount);
        for(int i=0; i<players; i++){
          //each turn can have multiple commands
          if(PCList[i]->dif==0){
            continue;
          }
          while(1){
            if(PCList[i]->dif==0){
              break;
            }
            char*phinf=phaseinfo(year,phase);
            write(descs[i],phinf,100*sizeof(char));
            char*tempcheck = calloc(1000,sizeof(char));
            //printf("%d\n",deadcount-AIs-players);
            if(deadcount-AIs-players==-1){
              strcpy(tempcheck,"Congratulations. You have won! Game ending...\n");
              write(descs[i],tempcheck,1000*sizeof(char));
              printf("GAME ENDING...\n");
              fflush(stdout);
              return 0;
            }
            if(PCList[i]->wealth>2000){
              strcpy(tempcheck,"Congratulations. You have won through riches! Game ending...\n");
              write(descs[i],tempcheck,1000*sizeof(char));
              for(int j=0; j<players; j++){
                if(j==i){continue;}
                strcpy(tempcheck,"Unfortunately, another player has won due to riches. Game ending...\n");
                write(descs[j],tempcheck,1000*sizeof(char));
              }
              printf("GAME ENDING...\n");
              fflush(stdout);
              return 0;
            }
            write(descs[i],tempcheck,1000*sizeof(char));
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
              
            //otherwise, check diplom or ally (strange cases)
            } else{
              if(phase==1&&first(curcmd,"diplom")==0){
                strsep(&curcmd," ");
                if(strlen(curcmd)==0){
                  char* towritecmd = calloc(1000,sizeof(char));
                  strcpy(towritecmd,"Command diplom requires second argument.\n");
                  write(descs[i],towritecmd,1000*sizeof(char));
                  free(towritecmd);
                  continue; //DEVIOUS USE OF CONTINUE
                }
                if(PCList[i]->wealth<200){
                  char* towritecmd = calloc(1000,sizeof(char));
                  strcpy(towritecmd,"Command diplom requires at least 200 wealth.\n");
                  write(descs[i],towritecmd,1000*sizeof(char));
                  free(towritecmd);
                  continue; //DEVIOUS USE OF CONTINUE
                }
                PCList[i]->wealth-=200;
                curcmd[strlen(curcmd)-1]='\0';
                int towhom = findc(PCList,players,curcmd);
                if(towhom<0){
                  towhom = findc(AIlist,AIs,curcmd);
                  if(towhom<0){
                    char* towritecmd = calloc(1000,sizeof(char));
                    strcpy(towritecmd,"Command diplom requires a valid country name.\n");
                    write(descs[i],towritecmd,1000*sizeof(char));
                    free(towritecmd);
                  }
                  //DIPLOMING AN AI
                  continue;
                }
                while(1){
                  write(descs[towhom],phinf,100*sizeof(char));
                  char*dipupdate=calloc(1000,sizeof(char));
                  sprintf(dipupdate,"Country %s is sending requesting an alliance. Do you accept?(yes/no)",PCList[i]->name);
                  write(descs[towhom],dipupdate,1000*sizeof(char));
                  read(descs[towhom],dipupdate,1000*sizeof(char));
                  if(strcmp(dipupdate,"yes\n")==0){
                    dipupdate=calloc(1000,sizeof(char));
                    sprintf(dipupdate,"Country %s accepted the alliance!",PCList[towhom]->name);
                    write(descs[i],dipupdate,1000*sizeof(char));
                    dipupdate=calloc(1000,sizeof(char));
                    strcpy(dipupdate,"You accepted the alliance");
                    write(descs[towhom],dipupdate,1000*sizeof(char));
                    free(dipupdate);
                    break;
                  }else if(strcmp(dipupdate,"no\n")==0){
                    dipupdate=calloc(1000,sizeof(char));
                    sprintf(dipupdate,"Country %s rejected the alliance!",PCList[towhom]->name);
                    write(descs[i],dipupdate,1000*sizeof(char));
                    dipupdate=calloc(1000,sizeof(char));
                    strcpy(dipupdate,"You rejected the alliance");
                    write(descs[towhom],dipupdate,1000*sizeof(char));
                    free(dipupdate);
                    break;
                  }
                  char*badtype=calloc(1000,sizeof(char));
                  strcpy(badtype,"Please type yes or type no next time\n");
                  write(descs[towhom],badtype,1000*sizeof(char));
                  free(dipupdate);
                }
              } else if(phase==1&&strcmp(curcmd,"list\n")==0||phase==2&&strcmp(curcmd,"list\n")==0){
                char*lstring=calloc(1000,sizeof(char));
                for(int j=0; j<players; j++){
                  if(j==i)continue;
                  if(PCList[j]->dif==0){
                    continue;
                  }
                  char*tstr=calloc(100,sizeof(char));
                  sprintf(tstr,"Name:%s GDP:%d Wealth:%d\n",PCList[j]->name,PCList[j]->GDP,PCList[j]->wealth);
                  strcat(lstring,tstr);
                }
                for(int j=0; j<AIs; j++){
                  char*tstr=calloc(100,sizeof(char));
                  if(AIlist[j]->dif==0){
                    continue;
                  }
                  sprintf(tstr,"Name:%s GDP:%d Wealth:%d\n",AIlist[j]->name,AIlist[j]->GDP,AIlist[j]->wealth);
                  strcat(lstring,tstr);
                }
                write(descs[i],lstring,1000*sizeof(char));
                free(lstring);
              } else if(phase==2&&first(curcmd,"war")==0){
                //UNDER CONSTRUCTION
                strsep(&curcmd," ");
                if(strlen(curcmd)==0){
                  char* towritecmd = calloc(1000,sizeof(char));
                  strcpy(towritecmd,"Command war requires second argument.\n");
                  write(descs[i],towritecmd,1000*sizeof(char));
                  free(towritecmd);
                  continue; //DEVIOUS USE OF CONTINUE
                }
                if(PCList[i]->wealth<200){
                  char* towritecmd = calloc(1000,sizeof(char));
                  strcpy(towritecmd,"Command war requires at least 200 wealth.\n");
                  write(descs[i],towritecmd,1000*sizeof(char));
                  free(towritecmd);
                  continue; //DEVIOUS USE OF CONTINUE
                }
                PCList[i]->wealth-=200;
                curcmd[strlen(curcmd)-1]='\0';
                int towhom = findc(PCList,players,curcmd);
                if(towhom<0){
                  towhom = findc(AIlist,AIs,curcmd);
                  if(towhom<0){
                    char* towritecmd = calloc(1000,sizeof(char));
                    strcpy(towritecmd,"Command war requires a valid country name.\n");
                    write(descs[i],towritecmd,1000*sizeof(char));
                    free(towritecmd);
                  }
                  //Warring AN AI
                  if(PCList[i]->military < AIlist[towhom]->military*AIlist[towhom]->dif){
                    char* towritecmd = calloc(1000,sizeof(char));
                    strcpy(towritecmd,"You died! That's unfortunate.\n");
                    PCList[i]->dif=0;
                    write(descs[i],towritecmd,1000*sizeof(char));
                    AIlist[towhom]->GDP+=PCList[i]->GDP/2;
                    AIlist[towhom]->wealth+=PCList[i]->wealth/2;
                  } else{
                    char* towritecmd = calloc(1000,sizeof(char));
                    strcpy(towritecmd,"You won! That's fantastic!\n");
                    AIlist[towhom]->dif=0;
                    write(descs[i],towritecmd,1000*sizeof(char));
                    PCList[i]->GDP+=AIlist[towhom]->GDP*2/3;
                    PCList[i]->wealth+=AIlist[towhom]->wealth*2/3;
                  }
                  continue;
                }
                //Warring a PLAYER
                if(PCList[i]->military < PCList[towhom]->military){
                  char* towritecmd = calloc(1000,sizeof(char));
                  strcpy(towritecmd,"You died! That's unfortunate.\n");
                  PCList[i]->dif=0;
                  write(descs[i],towritecmd,1000*sizeof(char));
                  PCList[towhom]->GDP+=PCList[i]->GDP;
                  PCList[towhom]->wealth+=PCList[i]->wealth;
                  char* news = calloc(1000,sizeof(char));
                  sprintf(news,"#Country %s attacked you, but you survived! Great!\n", PCList[i]->name);
                  write(descs[towhom],phinf,100*sizeof(char));
                  write(descs[towhom],news,1000*sizeof(char));
                } else{
                  char* towritecmd = calloc(1000,sizeof(char));
                  strcpy(towritecmd,"You won! That's fantastic!\n");
                  PCList[towhom]->dif=0;
                  write(descs[i],towritecmd,1000*sizeof(char));
                  PCList[i]->GDP+=PCList[towhom]->GDP;
                  PCList[i]->wealth+=PCList[towhom]->wealth;
                  char* news = calloc(1000,sizeof(char));
                  sprintf(news,"#Country %s attacked you, and you died... sorry!\n", PCList[i]->name);
                  write(descs[towhom],phinf,100*sizeof(char));
                  write(descs[towhom],news,1000*sizeof(char));
                }
                //ABOVE CONSTRUCTION
              } else{
                char * towritecmd = cmdhandler(curcmd,phase,PCList[i]);//REPLACE w/ cmd handler
                write(descs[i],towritecmd,1000*sizeof(char));
                free(towritecmd);
              }
            }
            free(phinf);
          }
        }
        //Each AI goes
        for(int i=0; i<0; i++){
          if(AIlist[i]->dif==0){continue;}
          invest(AIlist[i],200);
          train(AIlist[i],200);
        }
        phase++;
      }
      for(int i=0; i<players; i++){
        PCList[i]->wealth+=PCList[i]->GDP;
      }
      for(int i=0; i<AIs; i++){
        AIlist[i]->wealth+=AIlist[i]->GDP;
      }
      year++;
    }
    return 0;
}