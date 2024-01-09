#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
#include "nation.h"
//Initializes processes and world type
int main(){
    int year = 0;
    printf("\nWelcome to RisC and DiplomaC! Defeat the rival nations and take control!\n");
    struct country* playernation = birth();
    rivalbirth();
    //rivalInit
    //terrainInit
    return 0;
}