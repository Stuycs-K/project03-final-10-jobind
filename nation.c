#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
#include "territory.h"
//NOTES IN h FILE:
struct country {int dif; int GDP; int wealth; int military; char name[100]; struct domain* _domain; };
//function lets the player select how many enemy nations there are
char* aicountryname(int d){
    while(1){
        printf("---------------\nEnter the name of AI nation %d: ",d);
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
int rivalcount(){
    while(1){
        printf("---------------\nEnter the number of AIs in the game (1-9): ");
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
struct country** rivalbirth(int n){
    printf("INTERNAL: #OFAIS: %d\n", n);
    printf("Enter the AI level of the rival nations. (1-4) \n");
    int d = difficultyselect();
    struct country** w = malloc(n*sizeof(struct country*));
    for(int i=0; i<n; ++i){
        char name[100];
        sprintf(name,"%d",i);
        w[i]=nationsetup(name,d);
    }
    return w;
}
//Messed around in desmos for an interesting mapping from [0,1] to [0.6,0.85]
double smoothinate(double r){
    int x = abs(r);
    return (x-0.3+1/(x+0.5))/2;
}
//Returns -1 if failed, else returns the effect on GDP;
int invest(struct country* c, int d){
    if(d>c->wealth) return -1;
    int dGDP = (int)(smoothinate(myrandomdouble())*d);
    c->GDP+=dGDP;
    return dGDP;
}