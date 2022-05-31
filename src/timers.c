#include "timers.h"
#include <stdio.h>
#include <time.h>

#define PUBLIC /* empty */
#define PRIVATE static

//Med avseende på att spelet kör i 60 fps så betyder de när man gör "set...Timer(1000)"
//så kommer det ta 1000/60 sekunder innan timern tar slut


PUBLIC void timerUpdate(Soldier s, PowerUps p){
    //Frame timer
	if(getSoldierFrameTimer(s)<3){
		setSoldierFrameTimer(s, getSoldierFrameTimer(s) + 1);
	}

    // Timer for powerUp
	if(getSoldierPowerUpTimer>0){
		setSoldierPowerUpTimer(s, getSoldierPowerUpTimer(s) - 1);
	}
    if((getSoldierPowerUpTimer(s) == 0) && (getSoldierPowerUp(s) != 0)){
        powerUpTimedOut(s);
        setSoldierPowerUp(s, 0);
    }

	if(getPowerUpTimer(p) > 0){
		setPowerUpTimer(p, getPowerUpTimer(p) - 1);
	}else if(getPowerUpTimer(p) == 0){
        setPowerUpTimer(p, -1);
		powerUpRespawn(p);
    }

    //Timer for respawn
	if((getSoldierDead(s) == 1) && (getSoldierRespawnTimer(s) > 0)){
		setSoldierRespawnTimer(s, getSoldierRespawnTimer(s) - 1);
	}else if(getSoldierRespawnTimer(s) == 0){
		setSoldierDead(s, 0);
		setSoldierRespawnTimer(s, -1);
		respawnPlayer(s);
	}
}