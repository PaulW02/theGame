#ifndef media_h
#define media_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../player/soldier.h"
#include "../player/bullet.h"
#include "../player/weapon.h"

#include "../map/tile.h"


void loadSoldierMedia(SDL_Renderer *gRenderer, SDL_Texture **mSoldier, SDL_Rect gSoldierFrames[], Soldier s);
void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Weapon w);
void loadHealthMedia(SDL_Renderer *gRenderer, SDL_Texture **mHealthBar, SDL_Rect healthClips[]);
void loadAmmoMedia(SDL_Renderer *gRenderer, Weapon w, SDL_Texture **mAmmoCounter, SDL_Rect ammoClips[], SDL_Texture **mBulletType);
void loadReloadMedia(SDL_Renderer *gRenderer, Weapon w, SDL_Texture **mReloadDisplay);
void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]);
void loadPowers(SDL_Renderer *gRenderer, SDL_Texture **mPowers, SDL_Rect PowersClips[]);
void updateTimeDisplay(SDL_Renderer *gRenderer, SDL_Texture **timeTexture, SDL_Rect *pos, int currentTime);

#endif