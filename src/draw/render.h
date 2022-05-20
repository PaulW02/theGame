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

void renderPlayers(SDL_Renderer *gRenderer, Soldier soldiers[], int id, SDL_Texture *mSoldier, SDL_Rect gSoldierFrames[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES], SDL_Texture *mHealthBar, SDL_Rect healthClips[], SDL_Rect healthBarPositions[], SDL_Texture *mAmmoCounter, SDL_Rect ammoClips[], SDL_Rect ammoPosition, SDL_Texture *mBulletType, SDL_Texture *mReloadDisplay);
void bulletsRenderer(SDL_Renderer *gRenderer, Soldier soldiers[], Bullet bullets[], SDL_Texture **bulletTexture, int *amountOfBullets, int *bulletsActive);
void drawAmmoDisplay(SDL_Renderer *gRenderer, Soldier s, SDL_Texture *mAmmoCounter, SDL_Rect ammoClips[], SDL_Rect ammoPosition);
void drawReloadDisplay(SDL_Renderer *gRenderer, Weapon w, SDL_Texture *mReloadDisplay);
void drawBulletIndicator(SDL_Renderer *gRenderer, Weapon w, SDL_Texture *mBulletType);
void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES]);

#endif