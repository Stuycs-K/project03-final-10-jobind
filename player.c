#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//This initializes persistant vlaues relevant to the player nation. int difficulty ranges from 1 to 4
void playersetup(char*name,int difficulty){

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
        printf("---------------\nDifficulties- 1=Easy, 2=Normal, 3=Tough, 4=Unfair\nEnter the difficulty of managing your nation (1-4): ");
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
void birth(){
    char *s = countryname();
    int dif = difficultyselect();
    printf("DIFFICULTY SELECTED: %d\n", dif);
    playersetup(s,dif);
    printf("Congratulations! You have founed the nation of %s!\n", s);
}