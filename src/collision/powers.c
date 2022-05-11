

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
    int powerUpTimer;
    
};

PUBLIC PowerUps createPowerUps(int x, int y){
    PowerUps p = malloc(sizeof(struct powers));
    
    p->position.x = x; 
    p->position.y = y;
    p->position.h = 16;
    p->position.w = 16;
    p->powerUpTimer = 0;
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

PUBLIC void setPowerUpTimer(PowerUps p, int powerUpTimer){
    p->powerUpTimer = powerUpTimer;
}

PUBLIC int getPowerUpTimer(PowerUps p){
    return p->powerUpTimer;
}

PUBLIC void powerUpTouched(Soldier s, PowerUps p){
    setSoldierPowerUp(s, 1);
    setSoldierPowerUpTimer(s, 700);
    setSoldierSpeed(s,3);
    setPowerUpsPositionX(p, 600);
    setPowerUpsPositionY(p, 600);
    setPowerUpTimer(p, 400);
    
}

PUBLIC void powerUpTimedOut(Soldier s){
    setSoldierSpeed(s, 2);
}

PUBLIC void powerUpRespawn(PowerUps p){
    setPowerUpsPositionX(p, 220);
    setPowerUpsPositionY(p, 220);

}