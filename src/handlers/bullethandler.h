#ifndef bullethandler_h
#define bullethandler_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "../player/soldier.h"
#include "../player/bullet.h"
#include "../player/weapon.h"

void createAllCurrentBullets(Soldier soldiers[], Bullet bullets[], int *amountOfBullets, int *bulletsActive);
void move(SDL_Rect *b, int frame, int speed);
int checkBulletAngle(int frame);
int deleteBullet(int *amountOfBullets, Bullet bullets[],int delete);
int checkBulletRangeMax(Bullet b, SDL_Rect bulletPosition, int maxRange, SDL_Rect playerPosition);

#endif