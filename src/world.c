

#include "world.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int tileGrid[32][32] = {    // 3=trappa, 4=torn, 8=horrisontel väg, 9= vertikal väg
    {0x01,0x01,0x01,0x00,0x04,0x06,0x05,0x05,0x05,0x05,0x03,0x04,0x05,0x05,0x05,0x05,0x04,0x04,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x01,0x09,0x06,0x05,0x05,0x05,0x05,0x06,0x09,0x05,0x05,0x05,0x05,0x07,0x02,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x01,0x09,0x06,0x05,0x05,0x05,0x05,0x07,0x03,0x05,0x05,0x05,0x05,0x05,0x07,0x02,0x02,0x02,0x01,0x01,0x01,0x03,0x08,0x08,0x08,0x04,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x03,0x06,0x05,0x06,0x04,0x05,0x07,0x07,0x05,0x05,0x05,0x05,0x03,0x03,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x05,0x06,0x05,0x07,0x09,0x05,0x07,0x07,0x05,0x05,0x05,0x05,0x07,0x09,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x05,0x05,0x05,0x07,0x09,0x05,0x07,0x07,0x05,0x05,0x05,0x05,0x07,0x09,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x05,0x05,0x05,0x07,0x09,0x05,0x07,0x07,0x05,0x05,0x05,0x05,0x07,0x09,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x05,0x05,0x07,0x04,0x08,0x08,0x08,0x08,0x04,0x05,0x05,0x07,0x04,0x08,0x04,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x00,0x05,0x07,0x09,0x05,0x05,0x05,0x05,0x06,0x05,0x05,0x07,0x07,0x07,0x09,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x03,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x00,0x05,0x07,0x04,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x00,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x00,0x00,0x00,0x00},
    {0x03,0x08,0x08,0x04,0x00,0x00,0x00,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x03,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x03,0x08,0x08,0x03},
    {0x01,0x01,0x0d,0x09,0x00,0x00,0x02,0x02,0x04,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x09,0x00,0x00,0x02,0x05,0x09,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x02,0x01,0x01,0x04,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x09,0x00,0x00,0x02,0x05,0x09,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x04,0x02,0x01,0x01,0x09,0x0a,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x09,0x00,0x00,0x02,0x05,0x09,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x02,0x01,0x01,0x03,0x08,0x08,0x08,0x08,0x03,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x03,0x00,0x00,0x02,0x05,0x09,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x00,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x00,0x02,0x05,0x03,0x08,0x04,0x06,0x05,0x05,0x03,0x08,0x08,0x08,0x08,0x03,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x00,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x05,0x05,0x07,0x07,0x05,0x09,0x06,0x05,0x07,0x07,0x07,0x07,0x07,0x07,0x09,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x04,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x04,0x05,0x05,0x05,0x05,0x09,0x06,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x00,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x09,0x05,0x05,0x05,0x05,0x09,0x06,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x09,0x05,0x05,0x05,0x0a,0x09,0x06,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x03,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x03,0x08,0x08,0x08,0x08,0x03,0x06,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x09,0x05,0x05,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x07,0x07,0x05,0x06,0x06,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x04,0x08,0x08,0x03,0x05,0x05,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x07,0x05,0x05,0x05,0x06,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x09,0x0d,0x05,0x05,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x09,0x05,0x05,0x05,0x05,0x00,0x00},
    {0x03,0x08,0x08,0x03,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x09,0x05,0x05,0x05,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x04,0x05,0x05,0x05,0x05,0x01,0x02,0x03,0x08,0x08,0x08,0x03,0x05,0x05,0x05,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x05,0x05,0x05,0x05,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x05,0x05,0x05,0x05,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x09,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x05,0x05,0x05,0x05,0x05,0x00,0x00},
    {0x01,0x01,0x01,0x02,0x00,0x02,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x03,0x05,0x05,0x05,0x05,0x01,0x02,0x01,0x01,0x01,0x01,0x05,0x05,0x05,0x05,0x05,0x00,0x00},
    
    
    
    
};


PRIVATE int TILE_ROWS = 32;
PRIVATE int TILE_COLUMNS = 32;
PRIVATE int TILE_WIDTH = 16;
PRIVATE int TILE_HEIGHT = 16;

PUBLIC int getTileRows(){
    return TILE_ROWS;
}

PUBLIC int getTileColumns(){
    return TILE_COLUMNS;
}

PUBLIC int getTileWidth(){
    return TILE_WIDTH;
}

PUBLIC int getTileHeight(){
    return TILE_HEIGHT;
}

PUBLIC int getTileGrid(int x, int y){
    return tileGrid[x][y];
}



