#ifndef collision_h
#define collision_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../player/soldier.h"
#include "../map/tile.h"
#include "../player/bullet.h"

#define AMOUNT_TILES 32

bool soldierWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Soldier s, SDL_Rect *playerPosition, int frame);
void bulletWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Bullet bullets[], int *counter);
void stepBack(Soldier s, SDL_Rect *playerPosition, int frame);
void teleportSoldier(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int i, int j, SDL_Rect *playerPosition);
int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition);
void checkPlayerOutOfBoundaries(Soldier s);

#endif