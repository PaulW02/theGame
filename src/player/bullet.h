
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "soldier.h"

#ifndef bullet_h
#define bullet_h

typedef struct bullet *Bullet;

Bullet createBullet(int x, int y, Soldier soldier);
void setBulletPositionX(Bullet b, int positionX);
int getBulletPositionX(Bullet b);
void setBulletPositionY(Bullet b, int positionY);
int getBulletPositionY(Bullet b);
Soldier getBulletSoldier(Bullet b);
int getBulletHeight(Bullet b);
int getBulletWidth(Bullet b);
SDL_Rect getBulletSDL(Bullet b);
SDL_Rect getBulletPositionSDL(Bullet b);
void setBulletFrame(Bullet b, int frame);
int getBulletFrame(Bullet b);
void setBulletAngle(Bullet b, int bulletAngle);
int getBulletAngle(Bullet b);
void setBulletSoldierId(Bullet b, int soldierId);
int getBulletSoldierId(Bullet b);

#endif