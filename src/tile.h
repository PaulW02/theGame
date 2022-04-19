
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef tile_h
#define tile_h

typedef struct tile *Tile;

Tile createTile(int x, int y, int num);
void setTilePositionX(Tile t, int positionX);
int getTilePositionX(Tile t);
void setTilePositionY(Tile t, int positionY);
int getTilePositionY(Tile t);
void setTilePositionXY(Tile t, int positionX, int positionY);
void setWorldTileHeight(Tile t, int height);
int getWorldTileHeight(Tile t);
void setWorldTileWidth(Tile t, int width);
int getWorldTiletWidth(Tile t);
void setTileNumber(Tile t, int num);
int getTileNumber(Tile t);
void setTileSDLRec(Tile t, int x, int y, int w, int h);
SDL_Rect getTileSDLRec(Tile t);
void setTileCollision(Tile t, int collision);
int getTileCollision(Tile t);
void setTilePortal(Tile t, int portal);
int getTilePortal(Tile t);

#endif /* tile_h */