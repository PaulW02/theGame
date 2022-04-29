//
//  spaceman.c
//  F4
//
//  Created by Jonas Wåhslén on 2016-02-26.
//  Copyright © 2016 Jonas Wåhslén. All rights reserved.
//

#include <string.h>
#include "soldier.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "weapon.h"


#define PUBLIC /* empty */
#define PRIVATE static
#define MAXCHAR 100

PRIVATE int WIDTH = 32;
PRIVATE int HEIGHT = 32;

struct soldier{
    int soldierXPos;
    int soldierYPos;
    int soldierFrame;
    int speed;
    int health;
    Weapon weapon;
    char soldierFileName[MAXCHAR];
};

PUBLIC Soldier createSoldier(int x, int y){
    Soldier s = malloc(sizeof(struct soldier));
    s->soldierYPos = y;
    s->soldierXPos = x;
    s->soldierFrame = 0;
    s->speed = 1;
    s->health = 100; 
    return s;
}

PUBLIC int getSoldierHeight(){
    return HEIGHT;
}

PUBLIC int getSoldierWidth(){
    return WIDTH;
}

PUBLIC void setSoldierPositionX(Soldier s, int x){
    s->soldierXPos = x;
}

PUBLIC void setSoldierPositionY(Soldier s, int y){
    s->soldierYPos = y;
}

PUBLIC int getSoldierPositionX(Soldier s){
    return s->soldierXPos;
}

PUBLIC int getSoldierPositionY(Soldier s){
    return s->soldierYPos;
}

PUBLIC void setSoldierWeapon(Soldier s, Weapon weapon)
{
    s->weapon = weapon;
}

PUBLIC Weapon getSoldierWeapon(Soldier s)
{
    return  s->weapon;
}

PUBLIC void setSoldierFileName(Soldier s, char soldierFileName[MAXCHAR])
{
    strcpy(s->soldierFileName,soldierFileName);
}

PUBLIC const char* getSoldierFileName(Soldier s)
{
    return  s->soldierFileName;
}


PUBLIC void tick(int direction){
    
    
}

