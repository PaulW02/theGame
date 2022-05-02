#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "playerhandler.h"
#include "../player/soldier.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4



PUBLIC void initPlayers(Soldier soldiers[]){
    for (int i = 0; i < MAX_PLAYERS; i++)
    {   
        if(i == 0){
            soldiers[i] = createSoldier(10, 10);
            setSoldierFrame(soldiers[i], 2);
        }else if(i == 1){
            soldiers[i] = createSoldier(470, 10);
            setSoldierFrame(soldiers[i], 6);
        }else if(i == 2){
            soldiers[i] = createSoldier(470, 470);
            setSoldierFrame(soldiers[i], 6);
        }else{
            soldiers[i] = createSoldier(10, 470);
            setSoldierFrame(soldiers[i], 2);
        }

        setSoldierId(soldiers[i], i);
        setSoldierPosition(soldiers[i], getSoldierPositionX(soldiers[i]), getSoldierPositionY(soldiers[i]), 32, 32);
        setSoldierFileName(soldiers[i],"resources/Karaktarer/BOY/BOYpistol.png");
        weaponChoiceHandler(soldiers[i]);
    }
}


PUBLIC void weaponChoiceHandler(Soldier soldier)
{
                            //range, power, speed, mag_size, firerate, reloadtime
    Weapon pistol = createWeapon(200,10,10,9,10,100);
    Weapon bow = createWeapon(5,6,7,1,50,50);
    Weapon spear = createWeapon(5,6,7,1,50,50);
    Weapon rodBlue = createWeapon(5,6,7,1,50,50);
    Weapon rodRed = createWeapon(5,6,7,1,50,50);
    
    if (strstr(getSoldierFileName(soldier),"pistol"))
    {
        setSoldierWeapon(soldier,pistol);
    }
    if (strstr(getSoldierFileName(soldier),"bow"))
    {
        setSoldierWeapon(soldier,bow);
    }
    if (strstr(getSoldierFileName(soldier),"spear"))
    {
        setSoldierWeapon(soldier,spear);
    }
    if (strstr(getSoldierFileName(soldier),"rodBLUE"))
    {
        setSoldierWeapon(soldier,pistol);
    }
    if (strstr(getSoldierFileName(soldier),"rodRED"))
    {
        setSoldierWeapon(soldier,pistol);
    }
}