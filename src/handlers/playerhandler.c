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

#include "../menu.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4



PUBLIC void initPlayers(Soldier soldiers[]){
    for (int i = 0; i < MAX_PLAYERS; i++)
    {   
        soldiers[i] = createSoldier(-50, -50);
        setSoldierFrame(soldiers[i], 2);
        setSoldierFileName(soldiers[i],"resources/Karaktarer/PUMPKIN/PUMPKINpistol.png");

        setSoldierId(soldiers[i], i);
        setSoldierShotFired(soldiers[i], 0);
        setSoldierConnected(soldiers[i], 0);
        
        weaponChoiceHandler(soldiers[i]);
    }
}

PUBLIC void setValuesForConnectedPlayer(Soldier *newSoldier, int id, char soldierImagePath[], char soldierName[]){
    
    if(id == 0){
        *newSoldier = createSoldier(10, 10);
        setSoldierFrame(*newSoldier, 2);
    }else if(id == 1){
        *newSoldier = createSoldier(470, 10);
        setSoldierFrame(*newSoldier, 6);
    }else if(id == 2){
        *newSoldier = createSoldier(10, 470);
        setSoldierFrame(*newSoldier, 2);
    }else{
        *newSoldier = createSoldier(470, 470);
        setSoldierFrame(*newSoldier, 6);
    }

    setSoldierId(*newSoldier, id);
    setSoldierName(*newSoldier, soldierName);
    setSoldierFileName(*newSoldier,soldierImagePath);
    printf("%s \n", getSoldierFileName(*newSoldier));
    setSoldierShotFired(*newSoldier, 0);
    setSoldierConnected(*newSoldier, 1);
    setSoldierHealth(*newSoldier, 100);
    weaponChoiceHandler(*newSoldier);
}

PUBLIC void weaponChoiceHandler(Soldier soldier)
{
                            //range, power, speed, mag_size, firerate, reloadtime
    Weapon pistol = createWeapon(200,10,10,25,7,100);
    Weapon bow = createWeapon(5,20,7,1,15,15);
    Weapon spear = createWeapon(5,40,4,1,35,35);
    Weapon rodBlue = createWeapon(5,80,5,1,75,75);
    Weapon rodRed = createWeapon(5,80,8,1,5,5);
    
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
    switch(getSoldierId(soldierToRespawn)){
        case 0:
            setSoldierPositionX(soldierToRespawn, 16);
            setSoldierPositionY(soldierToRespawn, 24);        
            break;
        case 1:
            setSoldierPositionX(soldierToRespawn, 470);
            setSoldierPositionY(soldierToRespawn, 24);
            break;
        case 2:
            setSoldierPositionX(soldierToRespawn, 38);
            setSoldierPositionY(soldierToRespawn, 460);
            break;
        case 3:
            setSoldierPositionX(soldierToRespawn, 440);
            setSoldierPositionY(soldierToRespawn, 460);
            break;
    }
}

PUBLIC void scoreBoard(Soldier soldiers[], SDL_Renderer *gRenderer){
    SDL_Color black = {0x00,0x00,0x00}; //Black
    SDL_Color red = {0xFF,0x00,0x00}; // RED
    char score[1024];
    sprintf(score, "%s score: %d", getSoldierName(soldiers[0]), getSoldierKills(soldiers[0]));
    renderText(gRenderer, score, red, 30, 64, 24);

    sprintf(score, "%s score: %d", getSoldierName(soldiers[1]), getSoldierKills(soldiers[1]));
    renderText(gRenderer, score, red, 30, 192, 24);

    sprintf(score, "%s score: %d", getSoldierName(soldiers[2]), getSoldierKills(soldiers[2]));
    renderText(gRenderer, score, red, 30, 320, 24);

    sprintf(score, "%s score: %d", getSoldierName(soldiers[3]), getSoldierKills(soldiers[3]));
    renderText(gRenderer, score, red, 30, 448, 24);
}