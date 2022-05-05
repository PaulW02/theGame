#include "timers.h"
#include "player/powerup.h"

#define PUBLIC /* empty */
#define PRIVATE static


PUBLIC void timerUpdate(Soldier s, int *currentTime, int *lastTime){
    (*currentTime) = SDL_GetTicks();
    if ((*currentTime) > ((*lastTime) + 1000)) {
        printf("hello");
        (*lastTime) = (*currentTime);
    }    
    //Movement timer
    setSoldierMovementTimer(s, getSoldierMovementTimer(s) + 1);

    //Frame timer
    setSoldierFrameTimer(s, getSoldierFrameTimer(s) + 1);

    //Powerup timers
    if(getSoldierSpeedUpTimer(s)>0){
        setSoldierSpeedUpTimer(s, getSoldierSpeedUpTimer(s) - 1);
    }else{
        powerupTimedOut(s);
    }
}
