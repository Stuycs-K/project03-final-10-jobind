#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
//struct country {int dif; char name[100];};

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
struct country** rivalbirth(){
    int n = rivalcount();
    printf("Enter the AI level of the rival nations. (1-4) \n");
    int d = difficultyselect();
    return NULL;
}