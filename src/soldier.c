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
    int soldierNumber;
};

PUBLIC Soldier createSoldier(int x, int y, int num){
    Soldier s = malloc(sizeof(struct soldier));
    s->soldierYPos = y;
    s->soldierXPos = x;
    s->soldierNumber = num;
    s->soldierFrame = 0;
    s->speed = 1;
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


PUBLIC void tick(int direction){
    
}

PUBLIC void setSoldierNumber(Soldier s, int number){
    s->soldierNumber = number;
}

PUBLIC int getSoldierNumber(Soldier s){
    return s->soldierNumber;
}

