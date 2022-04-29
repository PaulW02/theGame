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
const char* getSoldierFileName(Soldier s);

#endif /* spaceman_h */


