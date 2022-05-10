

#include <string.h>
#include "powers.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"



#define PUBLIC /* empty */
#define PRIVATE static
#define MAXCHAR 100

PRIVATE int WIDTH = 32;
PRIVATE int HEIGHT = 32;

struct powers{
    SDL_Rect position;
    
};

PUBLIC PowerUps createPowerUps(int x, int y){
    PowerUps p = malloc(sizeof(struct powers));
    
    p->position.x = x; 
    p->position.y = y;
    p->position.h = 16;
    p->position.w = 16;
   
    return p;
}

PUBLIC int getPowerUpsHeight(){
    return HEIGHT;
}

PUBLIC int getPowerUpsWidth(){
    return WIDTH;
}

PUBLIC void setPowerUpsPositionX(PowerUps p, int x){
    p->position.x = x;
}

PUBLIC int getPowerUpsPositionX(PowerUps p){
    return p->position.x;
}

PUBLIC void setPowerUpsPositionY(PowerUps p, int y){
    p->position.y = y;
}

PUBLIC int getPowerUpsPositionY(PowerUps p){
    return p->position.y;
}

PUBLIC void setPowerUpsPosition(PowerUps p, int x, int y, int w, int h){
    p->position.x = x;
    p->position.y = y;
    p->position.w = w;
    p->position.h = h;
}

PUBLIC SDL_Rect getPowerUpsPosition(PowerUps p){
    return p->position;
}


