#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "media.h"

#include "../map/tile.h"
#include "../map/world.h"

#include "../player/bullet.h"
#include "../player/soldier.h"

#include "../application.h"

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

PUBLIC void loadAmmoMedia(SDL_Renderer *gRenderer, Weapon w, SDL_Texture **mAmmoCounter, SDL_Rect ammoClips[], SDL_Texture **mBulletType)
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

    //Bullet indicator based on weapon
    SDL_Surface* gBulletTypeSurface;
    if(strstr(getWeaponBullet(w), "pistolbullet"))
    {
        gBulletTypeSurface = IMG_Load("resources/bullettype_pistol.png");
        setWeaponBulletTypeRect(w, 0, 0, 5, 8);
    }
    else if(strstr(getWeaponBullet(w), "bow"))
    {
        gBulletTypeSurface = IMG_Load("resources/bullettype_bow.png");
        setWeaponBulletTypeRect(w, 0, 0, 20, 6);
    }
    else if(strstr(getWeaponBullet(w), "spear"))
    {
        gBulletTypeSurface = IMG_Load("resources/bullettype_spear.png");
        setWeaponBulletTypeRect(w, 0, 0, 20, 6);
    }
    else if(strstr(getWeaponBullet(w), "redrod"))
    {
        gBulletTypeSurface = IMG_Load("resources/bullettype_redrod.png");
        setWeaponBulletTypeRect(w, 0, 0, 20, 6);
    }
    else if(strstr(getWeaponBullet(w), "bluerod"))
    {
        gBulletTypeSurface = IMG_Load("resources/bullettype_bluerod.png");
        setWeaponBulletTypeRect(w, 0, 0, 20, 6);
    }

    *mBulletType = SDL_CreateTextureFromSurface(gRenderer, gBulletTypeSurface);
}

PUBLIC void loadReloadMedia(SDL_Renderer *gRenderer, Weapon w, SDL_Texture **mReloadDisplay)
{
    setReloadClips(w, 0, 0, 0, 27, 7);
    setReloadClips(w, 1, 30, 0, 27, 7);
    setReloadClips(w, 2, 60, 0, 27, 7);
    setReloadClips(w, 3, 90, 0, 27, 7);
    setReloadClips(w, 4, 120, 0, 27, 7);

    SDL_Surface* gReloadSurface = IMG_Load("resources/reload_dots.PNG");
    *mReloadDisplay = SDL_CreateTextureFromSurface(gRenderer, gReloadSurface);
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

void loadPowers(SDL_Renderer *gRenderer, SDL_Texture **mPowers, SDL_Rect PowersClips[]){
    SDL_Surface* gPowersSurface = IMG_Load("resources/gift.PNG");
    *mPowers = SDL_CreateTextureFromSurface(gRenderer, gPowersSurface);

    PowersClips[ 0 ].x =0;
    PowersClips[ 0 ].y =0;
    PowersClips[ 0 ].w =16;
    PowersClips[ 0 ].h =16;
}

PUBLIC void updateTimeDisplay(SDL_Renderer *gRenderer, SDL_Texture **timeTexture, SDL_Rect *pos, int currentTime)
{
    //This function sets position, value, font and color for time display
    Uint32 timeMinutes, timeSeconds;
    char timeDisplay[3];
    SDL_Color color = {0x00,0x00,0x00};
    //Translating current time from seconds to format "m:ds"
    if(currentTime < 60)
    {
        timeMinutes = 0;
    }
    else if(currentTime < 120)
    {
        timeMinutes = 1;
    }
    else if (currentTime < 180)
    {
        timeMinutes = 2;
    }
    else
    {
        timeMinutes = 3;
    }
    timeSeconds = currentTime%60;
    if(currentTime < 0)
    {
        timeSeconds = 0;
    }

    //load font
    TTF_Font* font = TTF_OpenFont("resources/fonts/8bitOperatorPlus-Regular.ttf",24);

    //set timeDisplay value and create texture
    sprintf(timeDisplay,"%d:%d%d",timeMinutes, timeSeconds/10, timeSeconds%10);
    SDL_Surface* text = TTF_RenderText_Solid(font,timeDisplay, color);
    *timeTexture = SDL_CreateTextureFromSurface(gRenderer,text);

    //set timeDisplay position and size
    SDL_QueryTexture(*timeTexture,NULL,NULL,&pos->w,&pos->h);
    pos->x=(WINDOW_WIDTH-pos->w)/2;
    pos->y=50;
}