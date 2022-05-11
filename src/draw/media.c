#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "media.h"

#include "../map/tile.h"
#include "../map/world.h"

#include "../player/bullet.h"
#include "../player/soldier.h"

#define PUBLIC /* empty */
#define PRIVATE static

// Load media functions

PUBLIC void loadSoldierMedia(SDL_Renderer *gRenderer, SDL_Texture **mSoldier, SDL_Rect gSoldierFrames[], Soldier s)
{
    SDL_Surface* gSoldierSurface = IMG_Load(getSoldierFileName(s));
    *mSoldier = SDL_CreateTextureFromSurface(gRenderer, gSoldierSurface);
    
     
    gSoldierFrames[ 0 ].x =   0;
    gSoldierFrames[ 0 ].y =   0;
    gSoldierFrames[ 0 ].w =  32;
    gSoldierFrames[ 0 ].h = 32;
    
    gSoldierFrames[ 1 ].x =  66;
    gSoldierFrames[ 1 ].y =   0;
    gSoldierFrames[ 1 ].w =  32;
    gSoldierFrames[ 1 ].h = 32;
    
    gSoldierFrames[ 2 ].x = 0;
    gSoldierFrames[ 2 ].y =   64;
    gSoldierFrames[ 2 ].w =  32;
    gSoldierFrames[ 2 ].h = 32;
    
    gSoldierFrames[ 3 ].x = 66;
    gSoldierFrames[ 3 ].y =   64;
    gSoldierFrames[ 3 ].w =  32;
    gSoldierFrames[ 3 ].h = 32;
    
    gSoldierFrames[ 4 ].x = 0;
    gSoldierFrames[ 4 ].y =   96;
    gSoldierFrames[ 4 ].w =  32;
    gSoldierFrames[ 4 ].h = 32;
    
    gSoldierFrames[ 5 ].x = 66;
    gSoldierFrames[ 5 ].y =   96;
    gSoldierFrames[ 5 ].w =  32;
    gSoldierFrames[ 5 ].h = 32;

    gSoldierFrames[ 6 ].x = 0;
    gSoldierFrames[ 6 ].y =   32;
    gSoldierFrames[ 6 ].w =  32;
    gSoldierFrames[ 6 ].h = 32;

    gSoldierFrames[ 7 ].x = 66;
    gSoldierFrames[ 7 ].y =   32;
    gSoldierFrames[ 7 ].w =  32;
    gSoldierFrames[ 7 ].h = 32;
}

PUBLIC void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Weapon w)
{
    SDL_Surface* bulletSurface = IMG_Load(getWeaponBullet(w));
    *bulletTexture = SDL_CreateTextureFromSurface(gRenderer, bulletSurface);
}

PUBLIC void loadHealthMedia(SDL_Renderer *gRenderer, SDL_Texture **mHealthBar, SDL_Rect healthClips[]){
    SDL_Surface* gHealthSurface = IMG_Load("resources/AllHealthStatuses6x6.PNG");
    *mHealthBar = SDL_CreateTextureFromSurface(gRenderer, gHealthSurface);

    healthClips[ 0 ].x =0;
    healthClips[ 0 ].y =0;
    healthClips[ 0 ].w =36;
    healthClips[ 0 ].h =8;

    healthClips[ 1 ].x =36;
    healthClips[ 1 ].y =0;
    healthClips[ 1 ].w =36;
    healthClips[ 1 ].h =8;

    healthClips[ 2 ].x =72;
    healthClips[ 2 ].y =0;
    healthClips[ 2 ].w =36;
    healthClips[ 2 ].h =8;

    healthClips[ 3 ].x =108;
    healthClips[ 3 ].y =0;
    healthClips[ 3 ].w =36;
    healthClips[ 3 ].h =8;

    healthClips[ 4 ].x =144;
    healthClips[ 4 ].y =0;
    healthClips[ 4 ].w =36;
    healthClips[ 4 ].h =8;

    healthClips[ 5 ].x =180;
    healthClips[ 5 ].y =0;
    healthClips[ 5 ].w =36;
    healthClips[ 5 ].h =8;

    healthClips[ 6 ].x =216;
    healthClips[ 6 ].y =0;
    healthClips[ 6 ].w =36;
    healthClips[ 6 ].h =8;

    healthClips[ 7 ].x =252;
    healthClips[ 7 ].y =0;
    healthClips[ 7 ].w =36;
    healthClips[ 7 ].h =8;

    healthClips[ 8 ].x =288;
    healthClips[ 8 ].y =0;
    healthClips[ 8 ].w =36;
    healthClips[ 8 ].h =8;

    healthClips[ 9 ].x =324;
    healthClips[ 9 ].y =0;
    healthClips[ 9 ].w =36;
    healthClips[ 9 ].h =8;

    healthClips[ 10 ].x =360;
    healthClips[ 10 ].y =0;
    healthClips[ 10 ].w =36;
    healthClips[ 10 ].h =8;
}

PUBLIC void loadAmmoMedia(SDL_Renderer *gRenderer, SDL_Texture **mAmmoCounter, SDL_Rect ammoClips[])
{
    SDL_Surface* gAmmoSurface = IMG_Load("resources/ammo_numbers.PNG");
    *mAmmoCounter = SDL_CreateTextureFromSurface(gRenderer, gAmmoSurface);

    ammoClips[ 0 ].x =0;
    ammoClips[ 0 ].y =0;
    ammoClips[ 0 ].w =5;
    ammoClips[ 0 ].h =7;

    ammoClips[ 1 ].x =6;
    ammoClips[ 1 ].y =0;
    ammoClips[ 1 ].w =5;
    ammoClips[ 1 ].h =7;

    ammoClips[ 2 ].x =12;
    ammoClips[ 2 ].y =0;
    ammoClips[ 2 ].w =5;
    ammoClips[ 2 ].h =7;

    ammoClips[ 3 ].x =18;
    ammoClips[ 3 ].y =0;
    ammoClips[ 3 ].w =5;
    ammoClips[ 3 ].h =7;

    ammoClips[ 4 ].x =24;
    ammoClips[ 4 ].y =0;
    ammoClips[ 4 ].w =5;
    ammoClips[ 4 ].h =7;

    ammoClips[ 5 ].x =30;
    ammoClips[ 5 ].y =0;
    ammoClips[ 5 ].w =5;
    ammoClips[ 5 ].h =7;

    ammoClips[ 6 ].x =36;
    ammoClips[ 6 ].y =0;
    ammoClips[ 6 ].w =5;
    ammoClips[ 6 ].h =7;

    ammoClips[ 7 ].x =42;
    ammoClips[ 7 ].y =0;
    ammoClips[ 7 ].w =5;
    ammoClips[ 7 ].h =7;

    ammoClips[ 8 ].x =48;
    ammoClips[ 8 ].y =0;
    ammoClips[ 8 ].w =5;
    ammoClips[ 8 ].h =7;

    ammoClips[ 9 ].x =54;
    ammoClips[ 9 ].y =0;
    ammoClips[ 9 ].w =5;
    ammoClips[ 9 ].h =7;

    ammoClips[ 10 ].x =60;
    ammoClips[ 10 ].y =0;
    ammoClips[ 10 ].w =5;
    ammoClips[ 10 ].h =7;
}

PUBLIC void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]){

    SDL_Surface* gTilesSurface = IMG_Load("resources/GrassTowersPortals.PNG");
    *mTiles = SDL_CreateTextureFromSurface(gRenderer, gTilesSurface);
    for (int i = 0; i < 16; i++) {
        gTiles[i].x = i*getTileWidth();
        gTiles[i].y = 0;
        gTiles[i].w = getTileWidth();
        gTiles[i].h = getTileHeight();
    }
}