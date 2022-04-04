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

PRIVATE int WIDTH = 16;
PRIVATE int HEIGHT = 16;

struct soldier{
    int soldierXPos;
    int soldierYPos;
    int soldierFrame;
    int speed;
};

PUBLIC Soldier createSoldier(int x, int y){
    Soldier s = malloc(sizeof(struct soldier));
    s->soldierYPos = y;
    s->soldierXPos = x;
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

PUBLIC int getSoldierPositionX(Soldier s){
    return s->soldierXPos;
}

PUBLIC int getSoldierPositionY(Soldier s){
    return s->soldierYPos;
}


PUBLIC void tick(int direction){
    
    
}

