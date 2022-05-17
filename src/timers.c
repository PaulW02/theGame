#include "timers.h"

#define PUBLIC /* empty */
#define PRIVATE static


PUBLIC void timerUpdate(Soldier s, PowerUps p){
    //Frame timer
    setSoldierFrameTimer(s, getSoldierFrameTimer(s) + 1);
    // Timer for powerUp
    setSoldierPowerUpTimer(s, getSoldierPowerUpTimer(s) - 1);
    setPowerUpTimer(p, getPowerUpTimer(p)-1);
    if(getSoldierPowerUpTimer(s)<1 && getSoldierPowerUp(s)!=0){
        powerUpTimedOut(s);
        setSoldierPowerUp(s,0);
    }
    if(getPowerUpTimer(p)==0){
        powerUpRespawn(p);
    }
}
