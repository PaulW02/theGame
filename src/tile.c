#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tile.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define PUBLIC /* empty */
#define PRIVATE static

struct tile{
    int tileXPos;
    int tileYPos;
    int number;
    int worldTileWidth;
    int worldTileHeight;
    int tileCollision;
    int tilePortal;
    SDL_Rect tilePosition;
};


PUBLIC Tile createTile(int x, int y, int num){
    Tile t = malloc(sizeof(struct tile));
    t->tileXPos = x;
    t->tileYPos = y;
    t->number = num;
    return t;
}

PUBLIC void setTilePositionX(Tile t, int positionX){
    t->tilePosition.x = positionX;
}

PUBLIC int getTilePositionX(Tile t){
    return t->tilePosition.x;
}

PUBLIC void setTilePositionY(Tile t, int positionY){
    t->tilePosition.y = positionY;
}

PUBLIC int getTilePositionY(Tile t){
    return t->tilePosition.y;
}

PUBLIC void setTilePositionXY(Tile t, int positionX, int positionY){
    t->tilePosition.x = positionX;
    t->tilePosition.y = positionY;
}


PUBLIC void setWorldTileHeight(Tile t, int height){
    t->tilePosition.h = height;
}

PUBLIC int getWorldTileHeight(Tile t){
    return t->tilePosition.h;
}

PUBLIC void setWorldTileWidth(Tile t, int width){
    t->tilePosition.w = width;
}

PUBLIC int getWorldTiletWidth(Tile t){
    return t->tilePosition.w;
}

PUBLIC void setTileNumber(Tile t, int num){
    t->number = num;
}

PUBLIC int getTileNumber(Tile t){
    return t->number;
}

PUBLIC void setTileSDLRec(Tile t, int x, int y, int w, int h){
    t->tilePosition.x = x;
    t->tilePosition.y = y;
    t->tilePosition.w = w;
    t->tilePosition.h = h;
}

PUBLIC SDL_Rect getTileSDLRec(Tile t){
    return t->tilePosition;
}

PUBLIC void setTileCollision(Tile t, int collision){
    t->tileCollision = collision;
}

PUBLIC int getTileCollision(Tile t){
    return t->tileCollision;
}

PUBLIC void setTilePortal(Tile t, int portal){
    t->tilePortal = portal;
}

PUBLIC int getTilePortal(Tile t){
    return t->tilePortal; 
}
