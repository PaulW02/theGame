//
//  soldier.h
//  F4
//
//  Created by Jonas Wåhslén on 2016-02-26.
//  Copyright © 2016 Jonas Wåhslén. All rights reserved.
//

#ifndef soldier_h
#define soldier_h
#define MAXCHAR 100

#include <stdio.h>
#include <stdlib.h>
#include "weapon.h"

typedef struct soldier *Soldier;


int getSoldierHeight();
int getSoldierWidth();
void setSoldierPositionX(Soldier s, int x);
void setSoldierPositionY(Soldier s, int y);
int getSoldierPositionX(Soldier s);
int getSoldierPositionY(Soldier s);
Soldier createSoldier(int x, int y);
void setSoldierWeapon(Soldier s, Weapon weapon);
Weapon getSoldierWeapon(Soldier s);
void setSoldierFileName(Soldier s, char soldierFileName[MAXCHAR]);
const char* getSoldierFileName(Soldier s);

#endif /* spaceman_h */



