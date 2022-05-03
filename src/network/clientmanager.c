#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "clientmanager.h"
#include "../sounds/soundeffects.h"
#include "../player/soldier.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define CONN_PARAMS_LENGTH 20
#define MAX_PLAYERS 4

PUBLIC void clientPacketSender(Soldier soldiers[], int *soldierXPos, int *soldierYPos, int *oldX, int *oldY, int *playerId, int bulletsActive, UDPsocket sd, IPaddress srvadd, UDPpacket *p){
    *soldierXPos = getSoldierPositionX(soldiers[*playerId]);
    *soldierYPos = getSoldierPositionY(soldiers[*playerId]);
    if(*soldierXPos != *oldX || *soldierYPos != *oldY || bulletsActive){
        UDPPacketSender(soldiers[*playerId], sd, srvadd, p);
        *oldX = *soldierXPos;
        *oldY = *soldierYPos;
    }       
}

PUBLIC void UDPPacketSender(Soldier soldier, UDPsocket sd, IPaddress srvadd, UDPpacket *p){
    sprintf((char *)p->data, "%d %d %d %d %d\n",(int) getSoldierId(soldier), (int) getSoldierPositionX(soldier), (int) getSoldierPositionY(soldier), (int) getSoldierFrame(soldier), (int) getSoldierShotFired(soldier));    
    p->address.host = srvadd.host;	/* Set the destination host */
    p->address.port = srvadd.port;	/* And destination port */
    p->len = strlen((char *)p->data) + 1;
    SDLNet_UDP_Send(sd, -1, p); 
}

PUBLIC void UDPPacketReceiver(Soldier soldiers[], int *playerId, UDPsocket sd, UDPpacket *p2){
    int connParams[CONN_PARAMS_LENGTH];
    if (SDLNet_UDP_Recv(sd, p2)){
        sscanf((char * )p2->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", playerId, &connParams[0], &connParams[1], &connParams[2], &connParams[3], &connParams[4], &connParams[5], &connParams[6], &connParams[7], &connParams[8], &connParams[9], &connParams[10], &connParams[11], &connParams[12], &connParams[13], &connParams[14], &connParams[15], &connParams[16], &connParams[17], &connParams[18], &connParams[19]);
        insertValuesToAllPlayers(soldiers, connParams, *playerId);
    }
}

PUBLIC void insertValuesToAllPlayers(Soldier soldiers[], int connParams[], int playerId){
    int margin = 0;
    for(int i = 0; i < MAX_PLAYERS; i++) {
        for(int j = 0; j < 5; j++) {
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
                    case 4:
                        setSoldierShotFired(soldiers[i], connParams[j+margin]);
                        break;
                }
            }
        }
        margin += 5;
    }
}