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
    Weapon pistol = createWeapon(200,10,10,25,5,100);
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

PUBLIC int getHealthImageBasedOnCurrentHealth(int currentHealth){
    if (currentHealth > 90 && currentHealth <= 100){
        return 0;
    }else if(currentHealth > 80 && currentHealth <= 90){
        return 1;
    }else if(currentHealth > 70 && currentHealth <= 80){
        return 2;
    }else if(currentHealth > 60 && currentHealth <= 70){
        return 3;
    }else if(currentHealth > 50 && currentHealth <= 60){
        return 4;
    }else if(currentHealth > 40 && currentHealth <= 50){
        return 5;
    }else if(currentHealth > 30 && currentHealth <= 40){
        return 6;
    }else if(currentHealth > 20 && currentHealth <= 30){
        return 7;
    }else if(currentHealth > 10 && currentHealth <= 20){
        return 8;
    }else if(currentHealth > 0 && currentHealth <= 10){
        return 9;
    }else{
        return 10;
    }
}

PUBLIC void respawnPlayer(Soldier soldierToRespawn){
    setSoldierHealth(soldierToRespawn, 100);
    setWeaponMagazine(getSoldierWeapon(soldierToRespawn), getWeaponMagazine_Size(getSoldierWeapon(soldierToRespawn)));
    setSoldierPositionX(soldierToRespawn, 256);
    setSoldierPositionY(soldierToRespawn, 200);
}