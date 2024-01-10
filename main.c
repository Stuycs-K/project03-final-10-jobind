#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
#include "nation.h"
#include "territory.h"
int main(){
    int year = 0;
    printf("\nWelcome to RisC and DiplomaC! Defeat the rival nations and take control!\n");
    //initiates data for the player nation
    struct country* playernation = birth();
    //initiates the data for the rivals
    struct country** rivals = rivalbirth();
    //terrainInit
    //**REPLACE W/ TERRAIN GEN
    //Below: Initializes diplomatic relations between all nations, semi-randomly
    //**REPLACE W/ DIPLOM. GEN
    
    //Main Loop (player):
    //**LOOP BODY HERE
    //End of main Loop (player):
    
    //****WIN-LOSE STATE****
    return 0;
}