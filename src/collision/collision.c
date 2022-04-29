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

#define AMOUNT_TILES 32
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

// Collision detection functions
PUBLIC bool soldierWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Soldier s, SDL_Rect *playerPosition, int frame){
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
                    teleportSoldier(s,tiles, i, j, playerPosition);
                }    
            }

            //För väggar
            if(getTileCollision(tiles[i][j])==1){
                
                //Rect Player
                leftA = (getSoldierPositionX(s)+4);
                rightA = (getSoldierPositionX(s) + (getSoldierWidth()-6));
                topA = (getSoldierPositionY(s)+4);
                bottomA = (getSoldierPositionY(s) + (getSoldierHeight()-6));

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
    if (getSoldierSpeedY(s)>0){
        newYPos=(playerPosition->y-=2);
        setSoldierPositionY(s, newYPos);
    }
    if (getSoldierSpeedX(s)>0){
        newXPos=(playerPosition->x-=2);
        setSoldierPositionX(s, newXPos);
    }
    if (getSoldierSpeedX(s)<0){
        newXPos=(playerPosition->x+=2);
        setSoldierPositionX(s, newXPos);
    }
    if (getSoldierSpeedY(s)<0){
        newYPos=(playerPosition->y+=2);
        setSoldierPositionY(s, newYPos);
    }
}

PUBLIC void teleportSoldier(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int i, int j, SDL_Rect *playerPosition){
   int newYPos, newXPos;
   
   if(getTileNumber(tiles[i][j])==0x0d){
       if((j==2)&&(i==12)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[25][26])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[25][26])));
            setSoldierPositionX(s, newXPos);
        }
        else if((j==26)&&(i==24)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[13][1])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[13][1])));
            setSoldierPositionX(s, newXPos);
        }        
       
   }
   else if(getTileNumber(tiles[i][j])==0x0a){
       if((j==24)&&(i==14)){   
            newYPos=(playerPosition->y=(getTilePositionY(tiles[19][8])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[19][8])));
            setSoldierPositionX(s, newXPos);
        }
        else if((j==9)&&(i==21)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[12][24])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[12][24])));
            setSoldierPositionX(s, newXPos);
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
    if (getSoldierPositionX(s) > WINDOW_WIDTH-16){
        setSoldierPositionX(s, WINDOW_WIDTH-16);
    }else if(getSoldierPositionX(s) < 0){
        setSoldierPositionX(s, 0);
    }
    
    if(getSoldierPositionY(s) > WINDOW_HEIGHT-16){
        setSoldierPositionY(s, WINDOW_WIDTH-16);
    }else if(getSoldierPositionY(s) < 0){
        setSoldierPositionX(s, 0);
    }
}