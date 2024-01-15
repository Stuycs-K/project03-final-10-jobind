#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "nation.h"
//processes command input
char* cmdhandler(char*cmd,int phase,struct country* c){
  char*outstring=calloc(1000,sizeof(char));
  if(strcmp(cmd,"info\n")==0){

  }
  if(phase==0){

  }
  if(phase==1){

  }
  if(phase==2){

  }
  return outstring;
}
//prints out the commands based on phase. 0=econ,1=diplom,2=war
char* helper(int phase){
  char*p =malloc(1000*sizeof(char));
  if(phase==0){
    strcpy(p,"Help for Economy phase:\
    \ninvest amount - Will take amount from wealth and increase the GDP.\
    \ntrain  amount - Will train amount troops, subtracting amount from wealth.\
    \nfinish        - Finishes your turn in the economy phase.\
    \ninfo          - Lists the info about the other countries\
    \n");
    return p;
  }
  if(phase==1){
    strcpy(p,"Diplomacy");
    return "";
  }
  if(phase==2){
    strcpy(p,"War");
    return "";
  }
}
//This initializes persistant vlaues relevant to the player nation. int difficulty ranges from 1 to 4
//Returns a pointer to struct country, found in nation.h containing the data for the player nation
struct country* playersetup(char*_name,int _dif){
    struct country* c = malloc(sizeof(struct country));
    strcpy(c->name,_name);
    c->dif=_dif;
    return c;
}
//Returns name of nation.
char* countryname(){
    while(1){
        printf("---------------\nEnter your new nation's name here: ");
        char *s = malloc(100*sizeof(char*));
        fgets(s,100,stdin);
        if(strlen(s)>1){
            s[strlen(s)-1]=0;
            if(strchr(s, ' ')!=NULL){
                printf("No SPACES in the name, please.\n");
            } else{
                return s;
            }
        }
    }
}
int difficultyselect(){
    while(1){
        printf("---------------\nDifficulties- 1=Easy, 2=Normal, 3=Tough, 4=Unfair: ");
        int d[1];
        char s[100];
        fgets(s,100,stdin);
        sscanf(s,"%d",d);
        if(*d<=4&&*d>=1){
            return *d;
        }
        printf("Not an integer from 1-4! Try again\n");
    }
}
struct country* birth(){
    char *s = countryname();
    printf("Enter the difficulty of managing your nation. (1-4)\n");
    int dif = difficultyselect();
    printf("DIFFICULTY SELECTED: %d\n", dif);
    struct country* c = playersetup(s,dif);
    printf("Congratulations! You have founed the nation of %s!\n", s);
    return c;
}