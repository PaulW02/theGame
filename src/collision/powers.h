

#ifndef powers_h
#define powers_h

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../player/soldier.h"


typedef struct powers *PowerUps;

PowerUps createPowerUps(int x, int y);
int getPowerUpsHeight();
int getPowerUpsWidth();
void setPowerUpsPositionX(PowerUps p, int x);
int getPowerUpsPositionX(PowerUps p);
void setPowerUpsPositionY(PowerUps p, int y);
int getPowerUpsPositionY(PowerUps p);
void setPowerUpsPosition(PowerUps p, int x, int y, int w, int h);
SDL_Rect getPowerUpsPosition(PowerUps p);
void setPowerUpTimer(PowerUps p, int powerUpTimer);
int getPowerUpTimer(PowerUps p);
void powerUpTouched(Soldier s, PowerUps p);
void powerUpTimedOut(Soldier s);
void powerUpRespawn(PowerUps p);
#endif