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

typedef struct soldier *Soldier;


int getSoldierHeight();
int getSoldierWidth();
void setSoldierPositionX(Soldier s, int x);
void setSoldierPositionY(Soldier s, int y);
int getSoldierPositionX(Soldier s);
int getSoldierPositionY(Soldier s);
Soldier createSoldier(int x, int y);
void setSoldierSpeed(Soldier s, int velocity);
int getSoldierSpeed(Soldier s);
void setSoldierSpeedX(Soldier s, int velocityX);
int getSoldierSpeedX(Soldier s);
void setSoldierSpeedY(Soldier s, int velocityY);
int getSoldierSpeedY(Soldier s);

#endif /* spaceman_h */



