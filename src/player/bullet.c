#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "weapon.h"
#include "soldier.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


#define PUBLIC /* empty */
#define PRIVATE static

#define bulletVel 2
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 5

struct bullet{
    int speed;
    int power;
    int range;
    SDL_Rect bulletPosition;
    SDL_Rect bulletSDL;
    int bulletFrame;
    int bulletAngle;
    Weapon weapon;
    Soldier soldier;
    int bulletSoldierId;
};

PUBLIC Bullet createBullet(int x, int y, Soldier soldier){
    Bullet b = malloc(sizeof(struct bullet));
    b->bulletPosition.x = x;
    b->bulletPosition.y = y;
    b->soldier = soldier;
    b->bulletPosition.w = BULLET_WIDTH;
    b->bulletPosition.h = BULLET_HEIGHT;
    b->bulletSDL.x = 0;
    b->bulletSDL.y = 0;
    b->bulletSDL.w = 10;
    b->bulletSDL.h = 5;
    return b;
}

PUBLIC void setBulletPositionX(Bullet b, int positionX){
    b->bulletPosition.x = positionX;
}

PUBLIC int getBulletPositionX(Bullet b){
    return b->bulletPosition.x;
}

PUBLIC void setBulletPositionY(Bullet b, int positionY){
    b->bulletPosition.y = positionY;
}

PUBLIC int getBulletPositionY(Bullet b){
    return b->bulletPosition.y;
}

PUBLIC Soldier getBulletSoldier(Bullet b){
    return b->soldier;
}

PUBLIC int getBulletHeight(Bullet b){
    return b->bulletPosition.h;
}

PUBLIC int getBulletWidth(Bullet b){
    return b->bulletPosition.w;
}

PUBLIC SDL_Rect getBulletSDL(Bullet b){
    return b->bulletSDL;
}

PUBLIC SDL_Rect getBulletPositionSDL(Bullet b){
    return b->bulletPosition;
}

PUBLIC void setBulletFrame(Bullet b, int frame){
    b->bulletFrame = frame;
}

PUBLIC int getBulletFrame(Bullet b){
    return b->bulletFrame;
}

PUBLIC void setBulletAngle(Bullet b, int bulletAngle){
    b->bulletAngle = bulletAngle;
}

PUBLIC int getBulletAngle(Bullet b){
    return b->bulletAngle;
}

PUBLIC void setBulletSoldierId(Bullet b, int soldierId){
    b->bulletSoldierId = soldierId;
}

PUBLIC int getBulletSoldierId(Bullet b){
    return b->bulletSoldierId;
}