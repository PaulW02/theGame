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

PUBLIC void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture)
{
    SDL_Surface* bulletSurface = IMG_Load("resources/THEBULLET.png");
    *bulletTexture = SDL_CreateTextureFromSurface(gRenderer, bulletSurface);
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