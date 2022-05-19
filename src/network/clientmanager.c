#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "clientmanager.h"
#include "packethandler.h"
#include "../sounds/soundeffects.h"
#include "../player/soldier.h"
#include "../handlers/playerhandler.h"
#define PUBLIC /* empty */
#define PRIVATE static

#define CONN_PARAMS_LENGTH 16
#define MAX_PLAYERS 4

PUBLIC void setupPlayerAndWeapon(GameInfo *gameInfo){
        
    for (int i = 0; i < gameInfo->amountOfPlayersConnected; i++)
    {
        setSoldierFileName(gameInfo->soldiers[i], gameInfo->soldierImagePaths[i]);
        weaponChoiceHandler(gameInfo->soldiers[i]);
        loadSoldierMedia(gameInfo->gRenderer, &gameInfo->mSoldier[i], gameInfo->gSpriteClips[i], gameInfo->soldiers[i]);
        loadBulletMedia(gameInfo->gRenderer, &gameInfo->bulletTexture[i], getSoldierWeapon(gameInfo->soldiers[i]));
    }
}

PUBLIC void setReceivedValuesForCurrentPlayer(GameInfo *gameInfo, int connParams[]){
    setSoldierId(gameInfo->soldiers[connParams[0]], connParams[0]);
    setSoldierFrame(gameInfo->soldiers[connParams[0]], connParams[1]);
    setSoldierPositionX(gameInfo->soldiers[connParams[0]], connParams[2]);
    setSoldierPositionY(gameInfo->soldiers[connParams[0]], connParams[3]);
    setSoldierShotFired(gameInfo->soldiers[connParams[0]], connParams[4]);
    setSoldierHealth(gameInfo->soldiers[connParams[0]], connParams[5]);
    //setWeaponMagazine(getSoldierWeapon(gameInfo->soldiers[connParams[0]]), connParams[6]);
    gameInfo->id = connParams[0];
}

PUBLIC void getCurrentPlayerInfo(GameInfo *gameInfo, PlayersData *clientPlayersData, int id){
    clientPlayersData->id = id;
    clientPlayersData->x = getSoldierPositionX(gameInfo->soldiers[id]);
    clientPlayersData->y = getSoldierPositionY(gameInfo->soldiers[id]);
    clientPlayersData->frame = getSoldierFrame(gameInfo->soldiers[id]);
    clientPlayersData->shotFired = getSoldierShotFired(gameInfo->soldiers[id]);
    clientPlayersData->health = getSoldierHealth(gameInfo->soldiers[id]);
    //clientPlayersData->magazine = getWeaponMagazine(getSoldierWeapon(gameInfo->soldiers[id]));
}

PUBLIC void setReceivedValuesForAllPlayers(GameInfo *gameInfo, PlayersData clientPlayersData){
    for (int i = 0; i < gameInfo->amountOfPlayersConnected; i++)
    {

        if(i != clientPlayersData.id){
            setSoldierId(gameInfo->soldiers[i],gameInfo->playersData[i].id);
            setSoldierPositionX(gameInfo->soldiers[i], gameInfo->playersData[i].x);
            setSoldierPositionY(gameInfo->soldiers[i], gameInfo->playersData[i].y);
            setSoldierFrame(gameInfo->soldiers[i], gameInfo->playersData[i].frame);
            setSoldierShotFired(gameInfo->soldiers[i], gameInfo->playersData[i].shotFired);
            setSoldierHealth(gameInfo->soldiers[i], gameInfo->playersData[i].health);
            //setWeaponMagazine(getSoldierWeapon(gameInfo->soldiers[i]), gameInfo->playersData[i].magazine);
        }
    }
}