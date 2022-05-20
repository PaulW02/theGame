#ifndef playerhandler_h
#define playerhandler_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "../player/soldier.h"
#include "../menu.h"
void initPlayers(Soldier soldiers[]);
void weaponChoiceHandler(Soldier soldier);
int getHealthImageBasedOnCurrentHealth(int currentHealth);
void respawnPlayer(Soldier soldierToRespawn);
void scoreBoard(Soldier soldiers[], SDL_Renderer *gRenderer);
#endif