#ifndef clientmanager_h
#define clientmanager_h

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../sounds/soundeffects.h"
#include "../player/soldier.h"

void clientPacketSender(Soldier soldiers[], int *soldierXPos, int *soldierYPos, int *oldX, int *oldY, int *playerId, int bulletsActive, UDPsocket sd, IPaddress srvadd, UDPpacket *p);
void UDPPacketSender(Soldier soldier, UDPsocket sd, IPaddress srvadd, UDPpacket *p);
void UDPPacketReceiver(Soldier soldiers[], int *playerId, UDPsocket sd, UDPpacket *p2);
void insertValuesToAllPlayers(Soldier soldiers[], int connParams[], int playerId);

#endif