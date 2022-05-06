#ifndef packethandler_h
#define packethandler_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include "../player/soldier.h"
#include <stdio.h>
#include <stdbool.h>

void clientSendMovementPacket(Soldier soldier, UDPpacket *p);
void clientReceiveMovementPacket(int *playerId, Soldier soldiers[], UDPpacket *p2);
void clientSendShotFiredPacket(Soldier soldier, UDPpacket *p);
void clientReceiveShotFiredPacket(int *playerId, Soldier soldiers[], UDPpacket *p2);
void insertMovementValuesToAllPlayers(Soldier soldiers[], int connParams[], int playerId);
void insertShotFiredValuesToAllPlayers(Soldier soldiers[], int connParams[], int playerId);

#endif