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
//#include "bullet.h"


struct bullet{
    int xPos;
    int yPos;
    int speed;
    SDL_Rect bulletPosition;
    SDL_Rect bulletSDL;
    int bulletFrame;
    SDL_RendererFlip bulletflip;
    int bulletAngle;
};
typedef struct bullet LocalBullet;


struct data {
    int playerX;
    int playerY;
    int frame;
    SDL_RendererFlip flip;
    //Bullet bullets[10];
    SDL_Rect bulletPosition[10];
    SDL_Rect bulletSDL[10];
    int bulletFrame[10];
    SDL_RendererFlip bulletFlip[10];
    int bulletAngle[10];
    int amountOfBullets;
};
typedef struct data Data;


 
int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    Uint32 IPclient1=0; 
    Uint32 IPclient2=0;
    Uint32 portClient1=0; 
    Uint32 portClient2=0;
    int quit, a, b;
    Data udpData;
 
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
 
	/* Main loop */
	quit = 0;
	while (!quit)
	{
        //printf("TEST\n");
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			//printf("UDP Packet incoming\n");
			//printf("\tData:    %s\n", (char *)pRecive->data);
			//printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
            if(IPclient1 == 0 && portClient1 == 0){
                printf("Client 1\n");
                IPclient1 = pRecive->address.host;
                portClient1 = pRecive->address.port;
            }else if(pRecive->address.port != portClient1  && IPclient2 == 0){
                printf("Client 2\n");
                IPclient2 = pRecive->address.host;
                portClient2 = pRecive->address.port;
            }else{
                if (pRecive->address.port == portClient1){
                    printf("Recived data\n");
                    if(IPclient2 != 0){
                        printf("Send to Client 2\n");
                        pSent->address.host = IPclient2;	/* Set the destination host */
		                pSent->address.port = portClient2;
                        //sscanf((char * )pRecive->data, "%d %d\n", &a, &b);
                        //printf("%d %d\n", a, b);
                        //sprintf((char *)pSent->data, "%d %d\n", a,  b);
                        memcpy(&udpData, (char * ) pRecive->data, sizeof(struct data));               
						memcpy((char *)pSent->data, &udpData , sizeof(struct data)+1);
                        pSent->len = sizeof(struct data)+1;
                        SDLNet_UDP_Send(sd, -1, pSent);
                    }
                } else if (pRecive->address.port == portClient2){
                    printf("Send to Client 1\n");    
                    pSent->address.host = IPclient1;	/* Set the destination host */
		            pSent->address.port = portClient1;
                    //sscanf((char * )pRecive->data, "%d %d\n", &a, &b);
                    //printf("%d %d\n", a, b);
                    //sprintf((char *)pSent->data, "%d %d\n", a, b);
                    memcpy(&udpData, (char * ) pRecive->data, sizeof(struct data));
                    printf("UDP Packet data %d %d\n", udpData.playerX, udpData.playerY);
                    memcpy((char *)pSent->data, &udpData , sizeof(struct data)+1);
                    pSent->len = sizeof(struct data)+1;
                    SDLNet_UDP_Send(sd, -1, pSent);
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