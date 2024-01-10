#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
//NOTES IN h FILE:
struct country {int dif; int GDP; int wealth; int military; char name[100]; struct domain* _domain; };
//function lets the player select how many enemy nations there are
int rivalcount(){
    while(1){
        printf("---------------\nEnter the number of rival nations in your world (1-9): ");
        int d[1];
        char s[100];
        fgets(s,100,stdin);
        sscanf(s,"%d",d);
        if(*d<=9&&*d>=1){
            return *d;
        }
        printf("Not an integer from 1-9! Try again\n");
    }
}
struct country* nationsetup(char*_name, int _dif){
    struct country* c = malloc(sizeof(struct country));
    strcpy(c->name,_name);
    c->dif=_dif;
    return c;
}
struct country** rivalbirth(){
    int n = rivalcount();
    printf("Enter the AI level of the rival nations. (1-4) \n");
    int d = 5-difficultyselect(); //we have to subtract dif from 5, since harder enemies -> lower dif, since they get better odds.
    struct country** w = malloc(n*sizeof(struct country*));
    for(int i=0; i<n; ++i){
        char name[100];
        sprintf(name,"%d",i);
        w[i]=nationsetup(name,d);
    }
    return w;
}