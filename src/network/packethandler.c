#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include "packethandler.h"
#include "../player/soldier.h"
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define MAX_PLAYERS 4

PUBLIC void clientSendMovementPacket(Soldier soldier, UDPpacket *p){
    sprintf((char *)p->data, "%d %d %d %d\n",(int) getSoldierId(soldier), (int) getSoldierPositionX(soldier), (int) getSoldierPositionY(soldier), (int) getSoldierFrame(soldier));
}

PUBLIC void clientReceiveMovementPacket(int *playerId, Soldier soldiers[], UDPpacket *p2){
    int connParams[16];
    sscanf((char * )p2->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", playerId, &connParams[0], &connParams[1], &connParams[2], &connParams[3], &connParams[4], &connParams[5], &connParams[6], &connParams[7], &connParams[8], &connParams[9], &connParams[10], &connParams[11], &connParams[12], &connParams[13], &connParams[14], &connParams[15]); 
    insertMovementValuesToAllPlayers(soldiers, connParams, *playerId);
}

PUBLIC void clientSendShotFiredPacket(Soldier soldier, UDPpacket *p){
    sprintf((char *)p->data, "%d %d\n", (int) getSoldierId(soldier), (int) getSoldierShotFired(soldier));
}

PUBLIC void clientReceiveShotFiredPacket(int *playerId, Soldier soldiers[], UDPpacket *p2){
    int connParams[8];
    sscanf((char * )p2->data, "%d %d %d %d %d %d %d %d %d\n", playerId, &connParams[0], &connParams[1], &connParams[2], &connParams[3], &connParams[4], &connParams[5], &connParams[6], &connParams[7]);
    insertShotFiredValuesToAllPlayers(soldiers, connParams, *playerId);
    printf("TRY AGAIN\n");
}

PUBLIC void insertMovementValuesToAllPlayers(Soldier soldiers[], int connParams[], int playerId){
    int margin = 0;
    for(int i = 0; i < MAX_PLAYERS; i++) {
        for(int j = 0; j < 4; j++) {
            if(playerId != i) {
                switch(j) {
                    case 0:
                        setSoldierId(soldiers[i],connParams[j+margin]);
                        break;
                    case 1:
                        setSoldierPositionX(soldiers[i], connParams[j+margin]);
                        break;
                    case 2:
                        setSoldierPositionY(soldiers[i], connParams[j+margin]);
                        break;
                    case 3:
                        setSoldierFrame(soldiers[i], connParams[j+margin]);
                        break;
                }
            }
        }
        margin += 4;
    }
}

PUBLIC void insertShotFiredValuesToAllPlayers(Soldier soldiers[], int connParams[], int playerId){
    int margin = 0;
    for(int i = 0; i < MAX_PLAYERS; i++) {
        for(int j = 0; j < 2; j++) {
            if(playerId != i) {
                switch(j) {
                    case 0:
                        setSoldierId(soldiers[i],connParams[j+margin]);
                        break;
                    case 1:
                        setSoldierShotFired(soldiers[i], connParams[j+margin]);
                        break;
                }
            }
        }
        margin += 2;
    }
}