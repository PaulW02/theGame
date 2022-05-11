/*#if 0
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
#include "handlers/playerhandler.h"
#include "player/soldier.h"
#include "handlers/playerhandler.h"


#define MAX_PLAYERS 4
#define MAX_BULLETS 100
#define CONN_PARAMS_LENGTH 5

#define PUBLIC
#define PRIVATE static

struct client {
    Uint32 IPclient;
    Uint32 portClient;
    TCPsocket sock;
};
typedef struct client Client;

struct serverPlayer {
    int id;
    TCPsocket sock;
    char imageFileName[50];
    int startX;
    int startY;
};
typedef struct serverPlayer ServerPlayer;

PRIVATE int checkClient(UDPpacket *pRecieve, int i, Client clients[]);

int main(int argc, char **argv)
{
	UDPsocket udp_sd;       /* Socket descriptor 
	UDPpacket *pRecive;       /* Pointer to packet memory 
	UDPpacket *pSent;
    IPaddress ip;
    TCPsocket tcp_sd, sd;
    TCPsocket client_sockets[MAX_PLAYERS];
    SDLNet_SocketSet socketSet;
    int quit;
    
    Client clients[MAX_PLAYERS];

    Soldier soldiers[MAX_PLAYERS];

    int playerId = 0, checkClientBool = 0, currentAmountOfPlayers = 0;
    

	/* Initialize SDL_net 
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    /* Resolve server name  
	if (SDLNet_ResolveHost(&ip, NULL, 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    socketSet=SDLNet_AllocSocketSet(6);
    if(!socketSet) {
        printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
        exit(1); //most of the time this is a major error, but do what you want.
    }

	/* Open a socket 
	if (!(udp_sd = SDLNet_UDP_Open(2000)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (SDLNet_UDP_AddSocket(socketSet, udp_sd) == -1)
    {
        fprintf(stderr, "Failed to add socket to socket set (UDP): %s", SDLNet_GetError());
    }

	if (!(tcp_sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
    if (SDLNet_TCP_AddSocket(socketSet, tcp_sd) == -1)
    {
        fprintf(stderr,"Failed to add socket to socket set (TCP): %s", SDLNet_GetError());
    }

	/* Make space for the packet 
	if (!((pSent = SDLNet_AllocPacket(1024))&&(pRecive = SDLNet_AllocPacket(1024))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    for (int i = 0; i < MAX_PLAYERS; i++)
    {   
        clients[i].IPclient = 0;
        clients[i].portClient = 0;
    }

    initPlayers(soldiers);
 
	/* Main loop 
	quit = 0;
	while (!quit)
	{
        for (int i = 0; i < MAX_PLAYERS; i++)  
        {  
            //socket descriptor 
            sd = client_sockets[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd){  
                SDLNet_TCP_AddSocket(socketSet, sd); 
                printf("TEST %d\n", i);
            } 
        }  

        TCPsocket newClientSock = SDLNet_TCP_Accept(tcp_sd);
        printf("TESTar");
        if (newClientSock)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)  
            {  
                //if position is empty 
                if(client_sockets[i] == NULL)  
                {  
                    Soldier soldier;
                    client_sockets[i] = newClientSock;  
                    printf("Adding to list of sockets as %d\n" , i);  
                    SDLNet_TCP_Recv(sd, soldier, sizeof(soldier));
                    printf("%d TEST", getSoldierId(soldier));
                            
                    break;  
                }  
            } 
        }

        for (int i = 0; i < MAX_PLAYERS; i++)  
        {  
            sd = client_sockets[i];  
                 
            if (sd)  
            {  
                char message[30];
                strcpy(message, "GOt IT now im sending it back");

                SDLNet_TCP_Send(sd, message, strlen(message)+1);  
            }  
        }

		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming 
		if (SDLNet_UDP_Recv(udp_sd, pRecive))
		{
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
                    if (pRecive->address.port = clients[i].portClient){
                        if(clients[i].IPclient != 0){
                            //printf("Recived data\n");
                            printf("Send to Client %d\n", i+1);
                            pSent->address.host = clients[i].IPclient;	/* Set the destination host 
                            pSent->address.port = clients[i].portClient;
                            //int connParams[CONN_PARAMS_LENGTH];
                            playerId = i;
                            printf("%d LENGTH\n", pRecive->len);
                            if(pRecive->len > 8){
                                int connParams[4];
                                sscanf((char * )pRecive->data, "%d %d %d %d\n", &connParams[0], &connParams[1], &connParams[2], &connParams[3]);
                                setSoldierId(soldiers[playerId], playerId);
                                setSoldierPositionX(soldiers[playerId], connParams[1]);
                                setSoldierPositionY(soldiers[playerId], connParams[2]);
                                setSoldierFrame(soldiers[playerId], connParams[3]);
                                sprintf((char *)pSent->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", i, getSoldierId(soldiers[0]), getSoldierPositionX(soldiers[0]), getSoldierPositionY(soldiers[0]), getSoldierFrame(soldiers[0]), getSoldierId(soldiers[1]), getSoldierPositionX(soldiers[1]), getSoldierPositionY(soldiers[1]), getSoldierFrame(soldiers[1]), getSoldierId(soldiers[2]), getSoldierPositionX(soldiers[2]), getSoldierPositionY(soldiers[2]), getSoldierFrame(soldiers[2]), getSoldierId(soldiers[3]), getSoldierPositionX(soldiers[3]), getSoldierPositionY(soldiers[3]), getSoldierFrame(soldiers[3]));
                                pSent->len = strlen((char *)pSent->data) + 1;
                            }else if(pRecive->len < 8){
                                int connParams1[2];
                                sscanf((char * )pRecive->data, "%d %d\n", &connParams1[0], &connParams1[1]);
                                setSoldierId(soldiers[playerId], playerId);
                                setSoldierShotFired(soldiers[playerId], connParams1[1]);
                                sprintf((char *)pSent->data, "%d %d %d %d %d %d %d %d %d\n", i, getSoldierId(soldiers[0]), getSoldierShotFired(soldiers[0]), getSoldierId(soldiers[1]), getSoldierShotFired(soldiers[1]), getSoldierId(soldiers[2]), getSoldierShotFired(soldiers[2]), getSoldierId(soldiers[3]), getSoldierShotFired(soldiers[3]));
                                pSent->len = strlen((char *)pSent->data) + 1;                            
                            }
                            SDLNet_UDP_Send(udp_sd, -1, pSent);
                            
                        }
                }
            }
            

			/* Quit if packet contains "quit" 
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}		
	}
 
	/* Clean and exit 
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
}*/