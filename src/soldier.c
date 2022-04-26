//
//  spaceman.c
//  F4
//
//  Created by Jonas Wåhslén on 2016-02-26.
//  Copyright © 2016 Jonas Wåhslén. All rights reserved.
//

#include "soldier.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int WIDTH = 32;
PRIVATE int HEIGHT = 32;

struct soldier{
    int soldierXPos;
    int soldierYPos;
    int soldierFrame;
    int speed;
    int speedX;
    int speedY;
};

PUBLIC Soldier createSoldier(int x, int y){
    Soldier s = malloc(sizeof(struct soldier));
    s->soldierYPos = y;
    s->soldierXPos = x;
    s->soldierFrame = 0;
    s->speed = 2;
    s->speedX = 0;
    s->speedY = 0;
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

PUBLIC void tick(int direction){
    
    
}

