#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


#include "collision.h"

#include "../player/soldier.h"
#include "../player/bullet.h"

#include "../map/tile.h"
#include "../map/world.h"

#include "../handlers/bullethandler.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4
#define AMOUNT_TILES 32
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

// Collision detection functions
PUBLIC bool soldierWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Soldier s, SDL_Rect *playerPosition, int frame, SDL_Rect *healthBarPosition){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    //För portaler
    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            if(getTilePortal(tiles[i][j])==1){
                //Rect Player
                leftA = (getSoldierPositionX(s)+6);
                rightA = (getSoldierPositionX(s) + (getSoldierWidth()-8));
                topA = (getSoldierPositionY(s)+6);
                bottomA = (getSoldierPositionY(s) + (getSoldierHeight()-8));

                //Rect Tile
                leftB = getTilePositionX(tiles[i][j]);
                rightB = (getTilePositionX(tiles[i][j]) + getTileWidth());
                topB = getTilePositionY(tiles[i][j]);
                bottomB = (getTilePositionY(tiles[i][j]) + getTileHeight());

                if( (bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) ){
                }else{
                    teleportSoldier(s,tiles, i, j, playerPosition, healthBarPosition);
                }    
            }

            //För väggar
            if(getTileCollision(tiles[i][j])==1){
                
                //Rect Player
                leftA = (getSoldierPositionX(s)+4);
                rightA = (getSoldierPositionX(s) + (getSoldierWidth()-6));
                topA = (getSoldierPositionY(s)+10);
                bottomA = (getSoldierPositionY(s) + (getSoldierHeight()-2));

                //Rect Tile
                leftB = getTilePositionX(tiles[i][j]);
                rightB = (getTilePositionX(tiles[i][j]) + getTileWidth());
                topB = getTilePositionY(tiles[i][j]);
                bottomB = (getTilePositionY(tiles[i][j]) + getTileHeight());

                if( !((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) ){
                    stepBack(s, playerPosition, frame);
                }
            }   
        }
    }
    return false;
}

PUBLIC void bulletWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Bullet bullets[], int *counter){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            if(getTileCollision(tiles[i][j])==1){
                for (int k = 0; k < (*counter); k++){

                    //Rect Bullet
                    leftA = getBulletPositionX(bullets[k]);
                    rightA = (getBulletPositionX(bullets[k]) + getBulletWidth(bullets[k]));
                    topA = getBulletPositionY(bullets[k]);
                    bottomA = (getBulletPositionY(bullets[k]) + getBulletHeight(bullets[k]));

                    //Rect Tile
                    leftB = getTilePositionX(tiles[i][j]);
                    rightB = (getTilePositionX(tiles[i][j]) + getTileWidth());
                    topB = getTilePositionY(tiles[i][j]);
                    bottomB = (getTilePositionY(tiles[i][j]) + getTileHeight());

                    if( (bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) ){
                    }else{
                        deleteBullet(counter, bullets, k);
                    }                    
                }
            }
        }
    }
}

PUBLIC void stepBack(Soldier s, SDL_Rect *playerPosition, int frame){
    int newYPos, newXPos;
    if(getSoldierSpeedY(s)>0){
        newYPos=(playerPosition->y-=getSoldierSpeed(s));
        setSoldierPositionY(s, newYPos);
    }
    if(getSoldierSpeedX(s)>0){
        newXPos=(playerPosition->x-=getSoldierSpeed(s));
        setSoldierPositionX(s, newXPos);
    }
    if(getSoldierSpeedX(s)<0){
        newXPos=(playerPosition->x+=getSoldierSpeed(s));
        setSoldierPositionX(s, newXPos);
    }
    if(getSoldierSpeedY(s)<0){
        newYPos=(playerPosition->y+=getSoldierSpeed(s));
        setSoldierPositionY(s, newYPos);
    }
}

PUBLIC void teleportSoldier(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int row, int column, SDL_Rect *playerPosition, SDL_Rect *healthBarPosition){
   int newYPos, newXPos;
   
   if(getTileNumber(tiles[row][column])==0x0d){
       if((column==9)&&(row==6)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[25][26])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[25][26])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }
        else if((column==26)&&(row==24)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[4][9])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[4][9])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }        
       
   }
   else if(getTileNumber(tiles[row][column])==0x0a){
       if((column==24)&&(row==14)){   
            newYPos=(playerPosition->y=(getTilePositionY(tiles[19][8])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[19][8])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }
        else if((column==9)&&(row==18)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[12][24])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[12][24])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }        
       
   }  
}

PUBLIC int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition)
{
    if(bulletPosition.x >= WINDOW_WIDTH || bulletPosition.y >= WINDOW_HEIGHT || bulletPosition.x <= -10 || bulletPosition.y <= -10){
        return 1;
    }else{
        return 0;
    }
}

PUBLIC void checkPlayerOutOfBoundaries(Soldier s)
{
    if (getSoldierPositionX(s) > WINDOW_WIDTH-32){
        setSoldierPositionX(s, WINDOW_WIDTH-32);
    }else if(getSoldierPositionX(s) < 0){
        setSoldierPositionX(s, 0);
    }
    
    if(getSoldierPositionY(s) > WINDOW_HEIGHT-32){
        setSoldierPositionY(s, WINDOW_WIDTH-32);
    }else if(getSoldierPositionY(s) < 0){
        setSoldierPositionY(s, 0);
    }
}

PUBLIC void bulletPlayerCollision(Bullet bullets[], Soldier soldiers[], int *amountOfBullets){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    int healthImage, currentScore=0;
    for (int i = 0; i < (*amountOfBullets); i++){
        for (int j = 0; j < MAX_PLAYERS; j++){
               //Rect Bullet
            leftA = getBulletPositionX(bullets[i]);
            rightA = (getBulletPositionX(bullets[i]) + getBulletWidth(bullets[i]));
            topA = getBulletPositionY(bullets[i]);
            bottomA = (getBulletPositionY(bullets[i]) + getBulletHeight(bullets[i]));

                //Rect Player
            leftB = (getSoldierPositionX(soldiers[j]));
            rightB = (getSoldierPositionX(soldiers[j]) + (getSoldierWidth()));
            topB = (getSoldierPositionY(soldiers[j]));
            bottomB = (getSoldierPositionY(soldiers[j]) + (getSoldierHeight()));

            if( ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) )){
            }else{
                if(((getBulletSoldierId(bullets[i])) != (j))){
                    deleteBullet(amountOfBullets, bullets, i);
                    setSoldierHealth(soldiers[j], getSoldierHealth(soldiers[j]) - getWeaponPower(getSoldierWeapon(soldiers[getBulletSoldierId(bullets[i])])));
                    healthImage = getHealthImageBasedOnCurrentHealth(getSoldierHealth(soldiers[j]));
                    if(healthImage == 10){                        
                        setSoldierKills(soldiers[getBulletSoldierId(bullets[i])], getSoldierKills(soldiers[getBulletSoldierId(bullets[i])])+1);   // ökar antal kills
                        currentScore = getSoldierKills(soldiers[getBulletSoldierId(bullets[i])]);
                        printf("SoldierPresent: %d kills", currentScore);
                    }
                }
            }   
        }
    }
}

PUBLIC void powersPlayerCollision(Soldier soldiers[], PowerUps powers){

   int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    for (int j = 0; j < MAX_PLAYERS; j++){

        //Rect Player
        leftB = (getSoldierPositionX(soldiers[j])+8);
        rightB = (getSoldierPositionX(soldiers[j]) + (getSoldierWidth())-8);
        topB = (getSoldierPositionY(soldiers[j])+6);
        bottomB = (getSoldierPositionY(soldiers[j]) + (getSoldierHeight())-8);
    
                        //Rect Player
        leftA = getPowerUpsPositionX(powers);
        rightA = getPowerUpsPositionX(powers) + getPowerUpsWidth(powers);
        topA = getPowerUpsPositionY(powers);
        bottomA = getPowerUpsPositionY(powers) + getPowerUpsHeight(powers);
        
        if(! ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) )){
            powerUpTouched(soldiers[j], powers);
        }
    }
}