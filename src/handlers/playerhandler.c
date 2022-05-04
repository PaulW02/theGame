#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "playerhandler.h"
#include "../player/soldier.h"
#include "../player/weapon.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4



PUBLIC void initPlayers(Soldier soldiers[]){
    for (int i = 0; i < MAX_PLAYERS; i++)
    {   
        if(i == 0){
            soldiers[i] = createSoldier(10, 10);
            setSoldierFrame(soldiers[i], 2);
            setSoldierFileName(soldiers[i],"resources/Karaktarer/PUMPKIN/PUMPKINpistol.png");
        }else if(i == 1){
            soldiers[i] = createSoldier(470, 10);
            setSoldierFrame(soldiers[i], 6);
            setSoldierFileName(soldiers[i],"resources/Karaktarer/FEMALEwizard/FEMALEpistol.png");
        }else if(i == 2){
            soldiers[i] = createSoldier(470, 470);
            setSoldierFrame(soldiers[i], 6);
            setSoldierFileName(soldiers[i],"resources/Karaktarer/GHOST/GHOSTpistol.png");
        }else{
            soldiers[i] = createSoldier(10, 470);
            setSoldierFrame(soldiers[i], 2);
            setSoldierFileName(soldiers[i],"resources/Karaktarer/BOY/BOYpistol.png");
        }

        setSoldierId(soldiers[i], i);
        setSoldierShotFired(soldiers[i], 0);
        setSoldierPosition(soldiers[i], getSoldierPositionX(soldiers[i]), getSoldierPositionY(soldiers[i]), 32, 32);
        
        weaponChoiceHandler(soldiers[i]);
    }
}


PUBLIC void weaponChoiceHandler(Soldier soldier)
{
                            //range, power, speed, mag_size, firerate, reloadtime
    Weapon pistol = createWeapon(200,10,5,9,50,100);
    Weapon bow = createWeapon(5,6,7,1,50,50);
    Weapon spear = createWeapon(5,6,7,1,50,50);
    Weapon rodBlue = createWeapon(5,6,7,1,50,50);
    Weapon rodRed = createWeapon(5,6,7,1,50,50);
    
    if (strstr(getSoldierFileName(soldier),"pistol"))
    {
        setSoldierWeapon(soldier,pistol);
        setWeaponBullet(getSoldierWeapon(soldier), "resources/pistolbullet.png");
    }
    if (strstr(getSoldierFileName(soldier),"bow"))
    {
        setSoldierWeapon(soldier,bow);
        setWeaponBullet(getSoldierWeapon(soldier), "resources/bowbullet.png");
    }
    if (strstr(getSoldierFileName(soldier),"spear"))
    {
        setSoldierWeapon(soldier,spear);
        setWeaponBullet(getSoldierWeapon(soldier), "resources/spearbullet.png");
    }
    if (strstr(getSoldierFileName(soldier),"rodBLUE"))
    {
        setSoldierWeapon(soldier,rodBlue);
        setWeaponBullet(getSoldierWeapon(soldier), "resources/bluerodbullet.png");
    }
    if (strstr(getSoldierFileName(soldier),"rodRED"))
    {
        setSoldierWeapon(soldier,rodRed);
        setWeaponBullet(getSoldierWeapon(soldier), "resources/redrodbullet.png");
    }
}