#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "soldier.h"
#include "../map/tile.h"

#ifndef powerup_h
#define powerup_h

void powerupTouched(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int row, int column);
void powerupTimedOut(Soldier s);

#endif