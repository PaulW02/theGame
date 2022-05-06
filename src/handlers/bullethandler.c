#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bullethandler.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "../player/soldier.h"
#include "../player/bullet.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4

PUBLIC void createAllCurrentBullets(Soldier soldiers[], Bullet bullets[], int *amountOfBullets, int *bulletsActive){
    int bulletAngle;
    if(*amountOfBullets == 0){
        *bulletsActive = 0;
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    { 
        if (getSoldierShotFired(soldiers[i]))
        {
            *bulletsActive = 1;
            Bullet b = createBullet(getSoldierPositionX(soldiers[i]), getSoldierPositionY(soldiers[i])+14, soldiers[i]);
            setBulletFrame(b, getSoldierFrame(soldiers[i]));
            setBulletSoldierId(b, getSoldierId(soldiers[i]));
            bulletAngle = checkBulletAngle(getBulletFrame(b));
            setBulletAngle(b,bulletAngle);
            bullets[*amountOfBullets] = b;
            (*amountOfBullets)++;

        } 
    }
}


PUBLIC void move(SDL_Rect *b, int frame, int speed){
    switch (frame)
    {
    case 0:
        b->y += speed;
        break;
    case 1:
        b->y += speed;
        break;
    case 2:
        b->x += speed;
        break;
    case 3:
        b->x += speed;
        break; 
    case 4:
        b->y -= speed;
        break;
    case 5:
        b->y -= speed;
        break;
    case 6:
        b->x -= speed;
        break;
    case 7:
        b->x -= speed;
        break;
    default:
        break;
    }
}


PUBLIC int checkBulletAngle(int frame){
    if(frame == 0 || frame == 1){
        return 90;
    }else if( frame == 2 || frame == 3){
        return 0;
    }else if(frame == 6 || frame == 7){
        return 180;
    }else{   
        return 90;
    }
}

PUBLIC int deleteBullet(int *amountOfBullets, Bullet bullets[],int delete)
{
    for (int i = delete; i < (*amountOfBullets-1); i++){
        bullets[i] = bullets[i+1];
    }
    (*amountOfBullets)--;
    return *amountOfBullets;
}

PUBLIC int checkBulletRangeMax(Bullet b, SDL_Rect bulletPosition, int maxRange, SDL_Rect playerPosition)
{
    if(bulletPosition.x == playerPosition.x+maxRange || bulletPosition.y == playerPosition.y+maxRange){
        return 1;
    }else{
        return 0;
    }
}