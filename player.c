#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "nation.h"
#include "territory.h"
//command-end of info cmd
char * stats(struct country* c){
    char*st=malloc(1000*sizeof(char));
    sprintf(st,"Stats about country %s:\nGDP:%d\nWealth:%d\nMilitary size:%d\n",c->name,c->GDP,c->wealth,c->military);
    return st;
}
//checks if the player forgot an argument for a known command
int checknewline(char * cmd){
  if(strlen(cmd)==0) return 0;
  if(strcmp(cmd,"invest\n")==0||strcmp(cmd,"train\n")==0||strcmp(cmd,"view\n")==0){
    return 1;
  }
  return 0;
}
//processes command input
char* cmdhandler(char*arg,int phase,struct country* c){
  char*cmd = strsep(&arg," ");
  char*outstring=calloc(1000,sizeof(char));
  if(strcmp(cmd,"info\n")==0){
    strcpy(outstring,stats(c));
    return outstring;
  }
  if(phase==0){
    if(checknewline(cmd)){
      sprintf(outstring,"The %s command needs an argument. Type help for clarification.\n",cmd);
      return outstring;
    }
    if(strcmp(cmd,"invest")==0){
      int d[1];
      sscanf(arg, "%d", d);
      int invres = invest(c,d[0]); //investment result
      if(invres==-1){
        strcpy(outstring,"Investment failed, insufficient funds. Try a lower amount, brokey.\n");
      } else{
        sprintf(outstring,"GDP increased by %d. Congrats.\n",invres);
      }
      return outstring;
    } else if(strcmp(cmd,"train")==0){
      int d[1];
      sscanf(arg, "%d", d);
      int tres = train(c,d[0]); //troop training result
      if(tres==-1){
        strcpy(outstring,"Training failed, your troops starved a little. Try to train fewer!\n");
      } else{
        sprintf(outstring,"Troops increased by %d. Urah!\n",tres);
      }
      return outstring;
    } else if(strcmp(cmd,"view")==0){
      int d[1];
      sscanf(arg, "%d", d);
      strcpy(outstring,printer(c,d[0]));
      return outstring;
    } else{
      strcpy(outstring,"Unkown command, please try again or type help for more info.\n");
      return outstring;
    }
  }
  if(phase==1){
    strcpy(outstring,"Unknown Command!\n");
    return outstring;
  }
  if(phase==2){
    strcpy(outstring,"Unknown Command!\n");
    return outstring;
  }
  strcpy(outstring,"PHASE ERROR IN cmdhandler()!\n");
  return outstring;
}
//prints out the commands based on phase. 0=econ,1=diplom,2=war
char* helper(int phase){
  char*p =malloc(1000*sizeof(char));
  if(phase==0){
    strcpy(p,"Help for Economy phase:\
    \ninvest amount - Will take amount from wealth and increase the GDP.\
    \ntrain  amount - Will train proportional* amount of troops, subtracting amount from wealth.\
    \nview   id     - Displays the land owned with ID id. You start with owning IDs 0,1, and 2.\
    \nfinish        - Finishes your turn in the economy phase.\
    \ninfo          - Lists the info about your country\
    \n");
    return p;
  }
  if(phase==1){
    strcpy(p,"Help for Diplomacy phase:\
    \nlist          - Lists the other nations in the game and info about them.\
    \ndiplom whom   - Subtracts 200 from wealth, and attempts to make an alliance with the nation named whom.\
    \nfinish        - Finishes your turn in the diplomacy phase.\
    \ninfo          - Lists the info about your country\
    \n");
    return p;
  }
  if(phase==2){
    strcpy(p,"Help for War phase:\
    \nlist          - Lists the other nations in the game and info about them.\
    \nwar whom      - Attacks the naiton: whom. If your military is greater, you win, take their GDP, and kill them.\
    \nfinish        - Finishes your turn in the diplomacy phase.\
    \ninfo          - Lists the info about your country\
    \n");
    return p;
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
    printf("Congratulations! You have founded the nation of %s!\n", s);
    return c;
}