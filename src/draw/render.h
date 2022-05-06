#ifndef render_h
#define render_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>

#include "../player/soldier.h"
#include "../player/bullet.h"

#include "../map/tile.h"

#define AMOUNT_TILES 32

void renderPlayers(SDL_Renderer *gRenderer, Soldier soldiers[], SDL_Texture *mSoldier, SDL_Rect gSoldierFrames[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES], SDL_Texture *mHealthBar, SDL_Rect healthClips[], SDL_Rect healthBarPositions[]);
void bulletsRenderer(SDL_Renderer *gRenderer, Bullet bullets[], SDL_Texture **bulletTexture, int *amountOfBullets, int weaponSpeed, int *bulletsActive);
void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES]);

#endif