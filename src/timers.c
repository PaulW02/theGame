#include "timers.h"

#define PUBLIC /* empty */
#define PRIVATE static

//Med avseende på att spelet kör i 60 fps så betyder de när man gör "set...Timer(1000)"
//så kommer det ta 1000/60 sekunder innan timern tar slut


PUBLIC void timerUpdate(Soldier s, PowerUps p){
    //Frame timer
    setSoldierFrameTimer(s, getSoldierFrameTimer(s) + 1);

    // Timer for powerUp
    setSoldierPowerUpTimer(s, getSoldierPowerUpTimer(s) - 1);
    setPowerUpTimer(p, getPowerUpTimer(p) - 1);
    if((getSoldierPowerUpTimer(s) < 1) && (getSoldierPowerUp(s) != 0)){
        powerUpTimedOut(s);
        setSoldierPowerUp(s, 0);
    }
    if(getPowerUpTimer(p) == 0){
        powerUpRespawn(p);
    }

    //Timer for respawn
    setSoldierRespawnTimer(s, getSoldierRespawnTimer(s) - 1);
}
