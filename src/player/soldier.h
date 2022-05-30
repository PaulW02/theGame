//
//  soldier.h
//  F4
//
//  Created by Jonas Wåhslén on 2016-02-26.
//  Copyright © 2016 Jonas Wåhslén. All rights reserved.
//

#ifndef soldier_h
#define soldier_h

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "weapon.h"

typedef struct soldier *Soldier;

void setSoldierId(Soldier s, int id);
int getSoldierId(Soldier s);
void setSoldierPosition(Soldier s, int x, int y, int w, int h);
SDL_Rect getSoldierPosition(Soldier s);
int getSoldierHeight();
int getSoldierWidth();
void setSoldierPositionX(Soldier s, int x);
int getSoldierPositionX(Soldier s);
void setSoldierPositionY(Soldier s, int y);
int getSoldierPositionY(Soldier s);
Soldier createSoldier(int x, int y);
void setSoldierSpeed(Soldier s, int velocity);
int getSoldierSpeed(Soldier s);
void setSoldierSpeedX(Soldier s, int velocityX);
int getSoldierSpeedX(Soldier s);
void setSoldierSpeedY(Soldier s, int velocityY);
int getSoldierSpeedY(Soldier s);
void setSoldierFrame(Soldier s, int frame);
int getSoldierFrame(Soldier s);
void setSoldierWeapon(Soldier s, Weapon weapon);
Weapon getSoldierWeapon(Soldier s);
void setSoldierShotFired(Soldier s, int shotFired);
int getSoldierShotFired(Soldier s);
void setSoldierFileName(Soldier s, char soldierFileName[]);
char* getSoldierFileName(Soldier s);
void setSoldierConnected(Soldier s, int connected);
int getSoldierConnected(Soldier s);
void setSoldierFrameTimer(Soldier s, int timer);
int getSoldierFrameTimer(Soldier s);
void setSoldierHealth(Soldier s, int health);
int getSoldierHealth(Soldier s);
void setSoldierKills(Soldier s, int kills);
int getSoldierKills(Soldier s);
void setSoldierPowerUp(Soldier s, int powerUp);
int getSoldierPowerUp(Soldier s);
void setSoldierPowerUpTimer(Soldier s, int powerUpTimer);
int getSoldierPowerUpTimer(Soldier s);
void setSoldierRespawnTimer(Soldier s, int timer);
int getSoldierRespawnTimer(Soldier s);
void setSoldierName(Soldier s, char soldierName[]);
char* getSoldierName(Soldier s);
void setSoldierDead(Soldier s, int dead);
int getSoldierDead(Soldier s);

#endif



