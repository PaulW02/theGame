#ifndef collision_h
#define collision_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "powers.h"
#include "../player/soldier.h"
#include "../map/tile.h"
#include "../player/bullet.h"
#include "../handlers/playerhandler.h"

#define AMOUNT_TILES 32

bool soldierWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Soldier s, SDL_Rect *playerPosition, int frame, SDL_Rect *healthBarPosition);
void bulletWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Bullet bullets[], int *counter);
void stepBack(Soldier s, SDL_Rect *playerPosition, int frame);
void teleportSoldier(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int row, int column, SDL_Rect *playerPosition, SDL_Rect *healthBarPosition);
int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition);
void checkPlayerOutOfBoundaries(Soldier s);
void bulletPlayerCollision(Bullet bullets[], Soldier soldiers[], int *amountOfBullets, int playerKills[]);
void powersPlayerCollision(Soldier soldiers[], PowerUps powers);

#endif