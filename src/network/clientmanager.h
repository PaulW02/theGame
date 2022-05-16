#ifndef clientmanager_h
#define clientmanager_h

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../application.h"
#include "../draw/media.h"
#include "../player/soldier.h"

struct playersData{
    int id;
	int x;
	int y;
    int frame;
    int shotFired;
    int health;
    int magazine;
};
typedef struct playersData PlayersData;

struct gameInfo{
    PlayersData playersData[MAX_PLAYERS];
    Soldier soldiers[MAX_PLAYERS];
    TCPsocket tcp_sd;
    int id;
    int amountOfPlayersConnected;
    char soldierImagePaths[MAX_PLAYERS][PATHLENGTH];
    SDL_Texture *mSoldier[MAX_PLAYERS];
    SDL_Renderer *gRenderer;
    SDL_Rect gSpriteClips[MAX_PLAYERS][8];
    SDL_Texture *bulletTexture[MAX_PLAYERS];
};
typedef struct gameInfo GameInfo;

void setupPlayerAndWeapon(GameInfo *gameInfo);
void setReceivedValuesForCurrentPlayer(GameInfo *gameInfo, int connParams[]);
void getCurrentPlayerInfo(GameInfo *gameInfo, PlayersData *clientPlayersData, int id);
void setReceivedValuesForAllPlayers(GameInfo *gameInfo, PlayersData clientPlayersData);
#endif