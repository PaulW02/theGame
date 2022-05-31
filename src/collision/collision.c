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
    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            //Tile position
            const SDL_Rect tilePos = getTileSDLRec(tiles[i][j]);
            
            //Portals
            if(getTilePortal(tiles[i][j])==1){

                //Adjusted SoldierPosition
                const SDL_Rect soldierPos = {
                    (getSoldierPositionX(s)+6),
                    (getSoldierPositionY(s)+6),
                    (getSoldierWidth()-8),
                    (getSoldierHeight()-8)
                };
                
                //Intersect?
                if(SDL_HasIntersection(&tilePos, &soldierPos) == SDL_TRUE){
                    teleportSoldier(s,tiles, i, j, playerPosition, healthBarPosition);
                }
            }

            //Walls
            if(getTileCollision(tiles[i][j])==1){
                
                const SDL_Rect soldierPos = {
                    (getSoldierPositionX(s)+4),
                    (getSoldierPositionY(s)+8),
                    (getSoldierWidth()-8),
                    (getSoldierHeight()-8)
                };

                if(SDL_HasIntersection(&tilePos, &soldierPos) == SDL_TRUE){
                    stepBack(s, playerPosition, frame);
                }
            }   
        }
    }
    return false;
}

PUBLIC void bulletWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Bullet bullets[], int *counter){
    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            if(getTileCollision(tiles[i][j])==1){
                for (int k = 0; k < (*counter); k++){

                    //Tile & Bullet position
                    const SDL_Rect tilePos = getTileSDLRec(tiles[i][j]);
                    const SDL_Rect bulletPos = getBulletPositionSDL(bullets[k]);

                    //Intersect?
                    if(SDL_HasIntersection(&tilePos, &bulletPos) == SDL_TRUE){
                        deleteBullet(counter, bullets, k);
                    }
                }
            }
        }
    }
}

PUBLIC void stepBack(Soldier s, SDL_Rect *playerPosition, int frame){
    //Pushed back depending on which way soldier walking
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
   //Soldier teleported to another portal of the same color
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

PUBLIC void bulletPlayerCollision(Bullet bullets[], Soldier soldiers[], int *amountOfBullets, int playerKills[]){
    int healthImage, currentScore;
    for (int i = 0; i < (*amountOfBullets); i++){
        for (int j = 0; j < MAX_PLAYERS; j++){
            
            //Player and bullet rectangle
            const SDL_Rect soldierPos = getSoldierPosition(soldiers[j]);
            const SDL_Rect bulletPos = getBulletPositionSDL(bullets[i]);

            //Intersect?
            if(SDL_HasIntersection(&bulletPos, &soldierPos) == SDL_TRUE){

                //Not able to shoot yourself!
                if(((getBulletSoldierId(bullets[i])) != (j))){
                    setSoldierHealth(soldiers[j], getSoldierHealth(soldiers[j]) - getWeaponPower(getSoldierWeapon(soldiers[getBulletSoldierId(bullets[i])])));
                    if(getSoldierHealth(soldiers[j]) <= 0 && getSoldierDead(soldiers[j]) == 0){                        
                        setSoldierKills(soldiers[getBulletSoldierId(bullets[i])], getSoldierKills(soldiers[getBulletSoldierId(bullets[i])])+1);   // ökar antal kills
                        currentScore = getSoldierKills(soldiers[getBulletSoldierId(bullets[i])]);
                        printf("%s killed %s and has now %d kills\n", getSoldierName(soldiers[getBulletSoldierId(bullets[i])]), getSoldierName(soldiers[j]), getSoldierKills(soldiers[getBulletSoldierId(bullets[i])]));
                        playerKills[getBulletSoldierId(bullets[i])] = getSoldierKills(soldiers[getBulletSoldierId(bullets[i])]);
                    }
                    deleteBullet(amountOfBullets, bullets, i);
                }
            }   
        }
    }
}

PUBLIC void powersPlayerCollision(Soldier soldiers[], PowerUps powers){
    for (int j = 0; j < MAX_PLAYERS; j++){

        //Powerup Rectangle & adjusted soldier rectangle
        const SDL_Rect powerUpPos = getPowerUpsPosition(powers);
        const SDL_Rect soldierPos = {
            (getSoldierPositionX(soldiers[j])+8),
            (getSoldierPositionY(soldiers[j])+6),
            (getSoldierWidth()-8),
            (getSoldierHeight()-8)
        };
        
        //Intersect?
        if(SDL_HasIntersection(&powerUpPos, &soldierPos) == SDL_TRUE){
            powerUpTouched(soldiers[j], powers);
        }
    }
}