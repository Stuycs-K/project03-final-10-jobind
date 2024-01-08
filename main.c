#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//Create name of nation, initial conditions, etc
char* countryname(){
    while(1){
        char *s = malloc(100*sizeof(char*));
        fgets(s,100,stdin);
        if(strlen(s)>1){
            return s;
        }
    }
}
void birth(){
    printf("---------------\nEnter your new nation's name here: ");
    char *s = countryname();
    printf("Congratulations! You have founed the nation of %s!", s);
}
//Initializes processes and world type
int main(){
    int year = 0;
    printf("Welcome to RisC and DiplomaC! Defeat the rival nations and take control!\n");
    birth();
    //playerInit
    //rivalInit
    //terrainInit
    return 0;
}