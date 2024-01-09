#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "nation.h"
//This initializes persistant vlaues relevant to the player nation. int difficulty ranges from 1 to 4
//Returns a pointer to struct country, found in nation.h containing the data for the player nation
struct country* playersetup(char*_name,int difficulty){
    struct country* c = malloc(sizeof(struct country));
    strcpy(c->name,_name);
    c->dif=difficulty;
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