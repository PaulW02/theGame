#ifndef clientmanager_h
#define clientmanager_h

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../application.h"
#include "../handlers/playerhandler.h"
#include "../draw/media.h"
#include "../player/soldier.h"
#include "../lobby.h"

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
    Lobby l;
    PlayersData playersData[MAX_PLAYERS];
    Soldier soldiers[MAX_PLAYERS];
    PlayerLobbyInformation playerLobbyInformation[MAX_PLAYERS];
    TCPsocket tcp_sd;
    int id;
    int amountOfPlayersConnected;
    char soldierNames[MAX_PLAYERS][MAX_NAME];
    char soldierImagePaths[MAX_PLAYERS][PATHLENGTH];
    SDL_Texture *mSoldier[MAX_PLAYERS];
    SDL_Renderer *gRenderer;
    SDL_Rect gSpriteClips[MAX_PLAYERS][8];
    SDL_Texture *bulletTexture[MAX_PLAYERS];
    SDL_Texture *mAmmoCounter[MAX_PLAYERS];
    SDL_Rect ammoClips[MAX_PLAYERS][11];
    SDL_Texture *mReloadDisplay[MAX_PLAYERS];
    SDL_Texture *mBulletType[MAX_PLAYERS];
};
typedef struct gameInfo GameInfo;

void setupPlayerAndWeapon(GameInfo *gameInfo);
void setReceivedValuesForCurrentPlayer(GameInfo *gameInfo, int connParams[]);
void getCurrentPlayerInfo(GameInfo *gameInfo, PlayersData *clientPlayersData, int id);
void setReceivedValuesForAllPlayers(GameInfo *gameInfo, PlayersData clientPlayersData);
#endif