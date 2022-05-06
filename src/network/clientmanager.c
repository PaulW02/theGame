#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"

#include "clientmanager.h"
#include "packethandler.h"
#include "../player/soldier.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define CONN_PARAMS_LENGTH 16
#define MAX_PLAYERS 4

PUBLIC void sendStarterPacket(Soldier soldier, TCPsocket tcp_sd){
    
    if(getSoldierId(soldier) == 0){
        setSoldierFileName(soldier, "resources/Karaktarer/FEMALEwizard/FEMALEbow.png");
    }else if(getSoldierId(soldier) == 1){
        setSoldierFileName(soldier, "resources/Karaktarer/BOY/BOYpistol.png");
    }else if(getSoldierId(soldier) == 2){
        setSoldierFileName(soldier, "resources/Karaktarer/MALEwarrior/MALEbow.png");
    }else{
        setSoldierFileName(soldier, "resources/Karaktarer/SKELETON/SKELETONbow.png");
    }
    
    SDLNet_TCP_Send(tcp_sd, soldier, sizeof(soldier));

}

PUBLIC void clientPacketSender(Soldier soldiers[], int *soldierXPos, int *soldierYPos, int *oldX, int *oldY, int *playerId, int bulletsActive, UDPsocket sd, IPaddress srvadd, UDPpacket *p, int *packetType){
    *soldierXPos = getSoldierPositionX(soldiers[*playerId]);
    *soldierYPos = getSoldierPositionY(soldiers[*playerId]);
    
    if(*soldierXPos != *oldX || *soldierYPos != *oldY){
        *packetType = 1;
        UDPPacketSender(soldiers[*playerId], sd, srvadd, p, *packetType);
        *oldX = *soldierXPos;
        *oldY = *soldierYPos;
    }
    if(bulletsActive || getSoldierShotFired(soldiers[*playerId])){
        *packetType = 2;
        UDPPacketSender(soldiers[*playerId], sd, srvadd, p, *packetType);
    }       
}

PUBLIC void UDPPacketSender(Soldier soldier, UDPsocket sd, IPaddress srvadd, UDPpacket *p, int packetType){
    if (packetType == 1){
        clientSendMovementPacket(soldier, p);
    }else if(packetType == 2){
        clientSendShotFiredPacket(soldier, p);
    }
    
    p->address.host = srvadd.host;	/* Set the destination host */
    p->address.port = srvadd.port;	/* And destination port */
    p->len = strlen((char *)p->data) + 1;
    
    SDLNet_UDP_Send(sd, -1, p); 
    
}

PUBLIC void UDPPacketReceiver(Soldier soldiers[], int *playerId, UDPsocket sd, UDPpacket *p2, int packetType){
    printf("PACKETTYPE %d\n", packetType);
    if (SDLNet_UDP_Recv(sd, p2)){
        if (packetType == 1){
            clientReceiveMovementPacket(playerId, soldiers, p2);
            printf("RECEIVED 1\n");
        }else if(packetType == 2){
            printf("TESTAR 2\n");
            clientReceiveShotFiredPacket(playerId, soldiers, p2);
            printf("RECEIVED 2\n");
        }
    }
}