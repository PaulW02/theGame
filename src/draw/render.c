#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>

#include "render.h"

#include "../collision/collision.h"

#include "../player/soldier.h"
#include "../player/bullet.h"

#include "../handlers/bullethandler.h"

#include "../map/tile.h"
#include "../map/world.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4
#define AMOUNT_TILES 32

PUBLIC void renderPlayers(SDL_Renderer *gRenderer, Soldier soldiers[], SDL_Texture *mSoldier, SDL_Rect gSoldierFrames[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES]){
    SDL_Rect playerPosition;
    for (int i = 0; i < MAX_PLAYERS; i++){
        playerPosition = getSoldierPosition(soldiers[i]);
        checkPlayerOutOfBoundaries(soldiers[i]); 
        soldierWallCollision(tiles, soldiers[i], &playerPosition, getSoldierFrame(soldiers[i]));
        SDL_RenderCopyEx(gRenderer, mSoldier, &gSoldierFrames[getSoldierFrame(soldiers[i])],&playerPosition, 0, NULL, SDL_FLIP_NONE);
    }
}

// Handles bullets
PUBLIC void bulletsRenderer(SDL_Renderer *gRenderer, Bullet bullets[], SDL_Texture **bulletTexture, int *amountOfBullets, int weaponSpeed, int *bulletsActive){
    SDL_Rect bulletPosition, bullet;

    if(*amountOfBullets == 0){
        *bulletsActive = 0;
    }
    for (int i = 0; i < *amountOfBullets; i++){
        bulletPosition = getBulletPositionSDL(bullets[i]);
        bullet = getBulletSDL(bullets[i]);
        move(&bulletPosition, getBulletFrame(bullets[i]), weaponSpeed);
        SDL_RenderCopyEx(gRenderer, *bulletTexture, &bullet, &bulletPosition, getBulletAngle(bullets[i]), NULL, SDL_FLIP_NONE);
        if(checkBulletOutOfBoundaries(bullets[i], bulletPosition)){
            free(bullets[i]);
            *amountOfBullets = deleteBullet(amountOfBullets,bullets, i);
        }else{
            setBulletPositionX(bullets[i], bulletPosition.x);
            setBulletPositionY(bullets[i], bulletPosition.y);
        }
    }
    
}

PUBLIC void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES]){
    
    SDL_Rect position;

    int height=0, width=0, number=0;
    height = getTileHeight();
    width = getTileWidth();

    position.y = 0;
    position.x = 0;
    position.h = getTileHeight();
    position.w = getTileWidth();

    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            number = getTileGrid(i,j);
            Tile tile=createTile(position.x, position.y, number);
            setTileSDLRec(tile, (j*width), (i*height), width, height);
            position=getTileSDLRec(tile);
            if(number==0x03 || number==0x04 || number==0x08||number==0x09){
                setTileCollision(tile, 1);
            }else if(number == 0x0d || number==0x0a){
                setTilePortal(tile, 1);
            }
            else{
                setTileCollision(tile, 0);
                setTilePortal(tile, 0);
            }
            tiles[i][j]=tile;

            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i,j)],&position , 0, NULL, SDL_FLIP_NONE);
        }
    }
}