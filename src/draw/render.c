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
#include "../handlers/playerhandler.h"

#include "../map/tile.h"
#include "../map/world.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4
#define AMOUNT_TILES 32

PUBLIC void renderPlayers(SDL_Renderer *gRenderer, Soldier soldiers[], int id, SDL_Texture *mSoldier[], SDL_Rect gSoldierFrames[][8], Tile tiles[AMOUNT_TILES][AMOUNT_TILES], SDL_Texture *mHealthBar, SDL_Rect healthClips[], SDL_Rect healthBarPositions[], SDL_Texture *mAmmoCounter, SDL_Rect ammoClips[], SDL_Rect ammoPosition){
    SDL_Rect playerPosition;
    int frame, healthImage;
    for (int i = 0; i < MAX_PLAYERS; i++){
        if(getSoldierPositionX(soldiers[i]) != -50){
            frame = getSoldierFrame(soldiers[i]);
            if (frame > 7)
            {
                frame = 7;
            }
            playerPosition = getSoldierPosition(soldiers[i]);
            healthBarPositions[i].y = getSoldierPositionY(soldiers[i]) - 12;
            healthBarPositions[i].x = getSoldierPositionX(soldiers[i]) - 4;
            healthBarPositions[i].h = 8;           // kolla här också!
            healthBarPositions[i].w = 36;
            checkPlayerOutOfBoundaries(soldiers[i]); 
            soldierWallCollision(tiles, soldiers[i], &playerPosition, frame, &healthBarPositions[i]);
            healthImage = getHealthImageBasedOnCurrentHealth(getSoldierHealth(soldiers[i]));
            SDL_RenderCopyEx(gRenderer, mSoldier[i], &gSoldierFrames[i][frame],&playerPosition, 0, NULL, SDL_FLIP_NONE);
            SDL_RenderCopyEx(gRenderer, mHealthBar, &healthClips[healthImage],&healthBarPositions[i], 0, NULL, SDL_FLIP_NONE);
            if(healthImage == 10){
                respawnPlayer(soldiers[i]);
            }
        }  
    }
    // ammunition display
    ammoPosition.y = healthBarPositions[id].y - 8;
    ammoPosition.x = healthBarPositions[id].x + 8;
    ammoPosition.h = 7;
    ammoPosition.w = 5;
    if(!getWeaponReload(getSoldierWeapon(soldiers[id])))
    {
        drawAmmoDisplay(gRenderer, soldiers[id], mAmmoCounter, ammoClips, ammoPosition);
    }
    /*else
    {
        //future reload display
    }*/
}

// Handles bullets
PUBLIC void bulletsRenderer(SDL_Renderer *gRenderer, Soldier soldiers[], Bullet bullets[], SDL_Texture *bulletTexture[], int *amountOfBullets, int *bulletsActive){
    SDL_Rect bulletPosition, bullet;
    int maxRange, weaponSpeed;

    if(*amountOfBullets == 0){
        *bulletsActive = 0;
    }
    for (int i = 0; i < *amountOfBullets; i++){
        bulletPosition = getBulletPositionSDL(bullets[i]);
        bullet = getBulletSDL(bullets[i]);
        weaponSpeed = getWeaponSpeed(getSoldierWeapon(soldiers[getBulletSoldierId(bullets[i])]));
        maxRange = getWeaponRange(getSoldierWeapon(soldiers[getBulletSoldierId(bullets[i])]));
        move(&bulletPosition, getBulletFrame(bullets[i]), weaponSpeed);
        SDL_RenderCopyEx(gRenderer, bulletTexture[getBulletSoldierId(bullets[i])], &bullet, &bulletPosition, getBulletAngle(bullets[i]), NULL, SDL_FLIP_NONE);
        if(checkBulletOutOfBoundaries(bullets[i], bulletPosition)){
            free(bullets[i]);
            *amountOfBullets = deleteBullet(amountOfBullets,bullets, i);
        }else{
            setBulletPositionX(bullets[i], bulletPosition.x);
            setBulletPositionY(bullets[i], bulletPosition.y);
        }
    }
    
}

PUBLIC void drawAmmoDisplay(SDL_Renderer *gRenderer, Soldier s, SDL_Texture *mAmmoCounter, SDL_Rect ammoClips[], SDL_Rect ammoPosition)
{
    Weapon w = getSoldierWeapon(s);
    //Prints current magazine size
    SDL_RenderCopyEx(gRenderer, mAmmoCounter, &ammoClips[getWeaponMagazine(w)/10], &ammoPosition, 0, NULL, SDL_FLIP_NONE);
    ammoPosition.x += 6;
    SDL_RenderCopyEx(gRenderer, mAmmoCounter, &ammoClips[getWeaponMagazine(w)%10], &ammoPosition, 0, NULL, SDL_FLIP_NONE);
    //Prints "/ max ammo"
    ammoPosition.x += 6;
    SDL_RenderCopyEx(gRenderer, mAmmoCounter, &ammoClips[10], &ammoPosition, 0, NULL, SDL_FLIP_NONE);
    ammoPosition.x += 6;
    SDL_RenderCopyEx(gRenderer, mAmmoCounter, &ammoClips[getWeaponMagazine_Size(w)/10], &ammoPosition, 0, NULL, SDL_FLIP_NONE);
    ammoPosition.x += 6;
    SDL_RenderCopyEx(gRenderer, mAmmoCounter, &ammoClips[getWeaponMagazine_Size(w)%10], &ammoPosition, 0, NULL, SDL_FLIP_NONE);
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