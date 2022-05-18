#include "timers.h"
#include <stdio.h>
#include <time.h>

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

PUBLIC int countDown(){	 
	 
	unsigned int x_hours=0;
	unsigned int x_minutes=0;
	unsigned int x_seconds=0;
	unsigned int x_milliseconds=0;
	unsigned int totaltime=0,count_down_time_in_secs=0,time_left=0;

	clock_t x_startTime,x_countTime;
	count_down_time_in_secs=20;  // 1 minute is 60, 1 hour is 3600

 
    x_startTime=clock();  // start clock
    time_left=count_down_time_in_secs-x_seconds;   // update timer

	while (time_left>0) 
	{
		x_countTime=clock(); // update timer difference
		x_milliseconds=x_countTime-x_startTime;
		x_seconds=(x_milliseconds/(CLOCKS_PER_SEC))-(x_minutes*60);
		x_minutes=(x_milliseconds/(CLOCKS_PER_SEC))/60;
		x_hours=x_minutes/60;

		time_left=count_down_time_in_secs-x_seconds; // subtract to get difference 


		printf( "\nYou have %d seconds left ( %d ) count down timer by TopCoder",time_left,count_down_time_in_secs);
	}

    return 0;
}
