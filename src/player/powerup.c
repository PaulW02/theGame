#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "soldier.h"
#include "../map/tile.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void powerupTouched(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int row, int column){
    if(getTilePowerup(tiles[row][column])==1){
        setSoldierSpeedUpTimer(s, 1000);
        setSoldierSpeed(s, 3);
    }
    /*else if(getTilePowerup(tiles[row][column])==2){
        //setSoldierDamageUpTimer(s, 1000);
    }else if(getTilePowerup(tiles[row][column])==3){
        //setSoldierNoClipTimer(s, 1000)
    }*/
}

PUBLIC void powerupTimedOut(Soldier s){
    if(getSoldierSpeedUpTimer(s)==0){
        setSoldierSpeed(s, 2);
    }
}

