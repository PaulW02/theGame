#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"
#include "soldier.h"
//#include "bullet.h"

#define MAX_PLAYERS 4
#define MAX_BULLETS 100
#define CONN_PARAMS_LENGTH 5

#define PUBLIC
#define PRIVATE static

struct client {
    Uint32 IPclient;
    Uint32 portClient;
};
typedef struct client Client;

PRIVATE int checkClient(UDPpacket *pRecieve, int i, Client clients[]);

int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    int quit;
    
    Client clients[MAX_PLAYERS];

    Soldier soldiers[MAX_PLAYERS];

    int playerId = 0, checkClientBool = 0;
    

	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(2000)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Make space for the packet */
	if (!((pSent = SDLNet_AllocPacket(1024))&&(pRecive = SDLNet_AllocPacket(1024))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        clients[i].IPclient = 0;
        clients[i].portClient = 0;
        soldiers[i] = createSoldier(0,0);
        setSoldierId(soldiers[i], i);
        setSoldierPositionX(soldiers[i], 0);
        setSoldierPositionY(soldiers[i], 0);
        setSoldierFrame(soldiers[i], 0);
        setSoldierShotFired(soldiers[i], 0);
    }
 
	/* Main loop */
	quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			//printf("UDP Packet incoming\n");
			//printf("\tData:    %s\n", (char *)pRecive->data);
			//printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
            
            for (int i = 0; i < MAX_PLAYERS; i++){
                //printf("%d TEST \n", checkClient(pRecive, i, clients));
                checkClientBool = checkClient(pRecive, i, clients);
                //for (int j = 0; j < MAX_PLAYERS; j++){
                
                    if (checkClientBool == (MAX_PLAYERS-1) && clients[i].IPclient == 0)
                    {
                        printf("Client %d", i+1);
                        clients[i].IPclient = pRecive->address.host;
                        clients[i].portClient = pRecive->address.port;
                    }
                //}
            }
            for (int i = 0; i < MAX_PLAYERS; i++){        
                for (int j = 0; j < MAX_PLAYERS; j++){
                    if (pRecive->address.port == clients[i].portClient){
                        if(clients[j].IPclient != 0 && j != i){
                            //printf("Recived data\n");
                            printf("Send to Client %d\n", j+1);
                            pSent->address.host = clients[j].IPclient;	/* Set the destination host */
                            pSent->address.port = clients[j].portClient;
                            int connParams[CONN_PARAMS_LENGTH];
                            sscanf((char * )pRecive->data, "%d %d %d %d %d %d\n", &connParams[0], &connParams[1], &connParams[2], &connParams[3], &connParams[4], &connParams[5]);
                            playerId = i;
                            setSoldierId(soldiers[playerId], playerId);
                            setSoldierPositionX(soldiers[playerId], connParams[1]);
                            setSoldierPositionY(soldiers[playerId], connParams[2]);
                            setSoldierFrame(soldiers[playerId], connParams[3]);
                            setSoldierShotFired(soldiers[playerId], connParams[4]);

                            sprintf((char *)pSent->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", j, getSoldierId(soldiers[0]), getSoldierPositionX(soldiers[0]), getSoldierPositionY(soldiers[0]), getSoldierFrame(soldiers[0]), getSoldierShotFired(soldiers[0]), getSoldierId(soldiers[1]), getSoldierPositionX(soldiers[1]), getSoldierPositionY(soldiers[1]), getSoldierFrame(soldiers[1]), getSoldierShotFired(soldiers[1]), getSoldierId(soldiers[2]), getSoldierPositionX(soldiers[2]), getSoldierPositionY(soldiers[2]), getSoldierFrame(soldiers[2]), getSoldierShotFired(soldiers[2]), getSoldierId(soldiers[3]), getSoldierPositionX(soldiers[3]), getSoldierPositionY(soldiers[3]), getSoldierFrame(soldiers[3]), getSoldierShotFired(soldiers[3]));
                            pSent->len = strlen((char *)pSent->data) + 1;
                            SDLNet_UDP_Send(sd, -1, pSent);
                        }
                    }
                }
            }
            

			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}		
	}
 
	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pRecive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
} 

PRIVATE int checkClient(UDPpacket *pRecieve, int i, Client clients[]){
    int counter = 0;
    for (int elem = 0; elem < MAX_PLAYERS; elem++)
    {
        if(pRecieve->address.port != clients[elem].portClient && elem != i){
            counter++;
        }
    }
    return counter;
}