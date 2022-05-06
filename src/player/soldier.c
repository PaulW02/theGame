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
    int id;
    SDL_Rect position;
    int frame;
    int speed;
    int speedX;
    int speedY;
    Weapon weapon;
    int shotFired;
    char soldierFileName[MAXCHAR];
    int connected;
};

PUBLIC Soldier createSoldier(int x, int y){
    Soldier s = malloc(sizeof(struct soldier));
    s->speed = 2;
    s->speedX = 0;
    s->speedY = 0;
    s->position.x = x; 
    s->position.y = y;
    s->frame = 0;
    s->connected = 0;
    return s;
}

PUBLIC void setSoldierId(Soldier s, int id){
    s->id = id;
}

PUBLIC int getSoldierId(Soldier s){
    return s->id;
}

PUBLIC int getSoldierHeight(){
    return HEIGHT;
}

PUBLIC int getSoldierWidth(){
    return WIDTH;
}

PUBLIC void setSoldierPosition(Soldier s, int x, int y, int w, int h){
    s->position.x = x;
    s->position.y = y;
    s->position.w = w;
    s->position.h = h;
}

PUBLIC SDL_Rect getSoldierPosition(Soldier s){
    return s->position;
}

PUBLIC void setSoldierPositionX(Soldier s, int x){
    s->position.x = x;
}

PUBLIC int getSoldierPositionX(Soldier s){
    return s->position.x;
}

PUBLIC void setSoldierPositionY(Soldier s, int y){
    s->position.y = y;
}

PUBLIC int getSoldierPositionY(Soldier s){
    return s->position.y;
}

PUBLIC void setSoldierSpeed(Soldier s, int velocity){
    s->speed = velocity;
}

PUBLIC int getSoldierSpeed(Soldier s){
    return s->speed;
}

PUBLIC void setSoldierSpeedX(Soldier s, int velocityX){
    s->speedX = velocityX;
}

PUBLIC int getSoldierSpeedX(Soldier s){
    return s->speedX;
}

PUBLIC void setSoldierSpeedY(Soldier s, int velocityY){
    s->speedY = velocityY;
}

PUBLIC int getSoldierSpeedY(Soldier s){
    return s->speedY;
}

PUBLIC void setSoldierFrame(Soldier s, int frame){
    s->frame = frame;
}

PUBLIC int getSoldierFrame(Soldier s){
    return s->frame;
}

PUBLIC void setSoldierWeapon(Soldier s, Weapon weapon)
{
    s->weapon = weapon;
}

PUBLIC Weapon getSoldierWeapon(Soldier s)
{
    return  s->weapon;
}

PUBLIC void setSoldierShotFired(Soldier s, int shotFired){
    s->shotFired = shotFired;
}

PUBLIC int getSoldierShotFired(Soldier s){
    return s->shotFired;
}

PUBLIC void setSoldierConnected(Soldier s, int connected){
    s->connected = connected;
}

PUBLIC int getSoldierConnected(Soldier s){
    return s->connected;
}

PUBLIC void setSoldierFileName(Soldier s, char soldierFileName[MAXCHAR])
{
    strcpy(s->soldierFileName,soldierFileName);
}

PUBLIC const char* getSoldierFileName(Soldier s)
{
    return  s->soldierFileName;
}
