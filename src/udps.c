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
};
typedef struct client Client;

PRIVATE int checkClient(UDPpacket *pRecieve, int i, Client clients[]);

int main(int argc, char **argv)
{
	UDPsocket udp_sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    char remote_ip_str[16]="";
    TCPsocket sd, csd;

    IPaddress ip, *remoteIP;
    int quit, quit2;
    
    Client clients[MAX_PLAYERS];

    Soldier soldiers[MAX_PLAYERS];

    int playerId = 0, checkClientBool = 0;
    

	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    if (SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

	/* Open a socket */
	if (!(udp_sd = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!(sd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
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
    }

    initPlayers(soldiers);
 
	/* Main loop */
	quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		/*if (SDLNet_UDP_Recv(sd, pRecive))
		{*/
        if ((csd = SDLNet_TCP_Accept(sd)))
        {

			//printf("UDP Packet incoming\n");
			//printf("\tData:    %s\n", (char *)pRecive->data);
			//printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
            if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
            {
                Uint32 remote_ip;
                unsigned char ch0, ch1, ch2, ch3;
                /* Print the address, converting in the host format */
                printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
                remote_ip = SDLNet_Read32(&remoteIP->host);
                ch0 = remote_ip>>24;
                ch1 = (remote_ip & 0x00ffffff)>>16;
                ch2 = (remote_ip & 0x0000ffff)>>8;
                ch3 = remote_ip & 0x000000ff;

                sprintf(remote_ip_str,"%d.%d.%d.%d", (int)ch0, (int)ch1, (int)ch2, (int)ch3);

                printf("Remote host IP in string form: %s\n", remote_ip_str);

            }
            else{
                fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
            }
            quit2 = 0;
            while (!quit2)
            {
                int allocated_index;
                
                if (SDLNet_TCP_Recv(csd, buffer, 512) > 0){
                }
                else{
                    fprintf(stderr, ">SDLNet_TCP_Recv: %s", SDLNet_GetError());
                    exit(EXIT_FAILURE);
                }

                { 
                    IPaddress client_address;
                    if(SDLNet_ResolveHost(&client_address, remote_ip_str, 2001) == -1)
                    {
                        fprintf(stderr, "Couldn't resolve client... %s\n", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }


                    while(SDLNet_TCP_Recv(csd,&csm_click_at_pdu,
                                sizeof(struct csm_click_at))>0)
                    {
                        int i; int rx, ry;

                        printf("%d %d\n", csm_click_at_pdu.x, csm_click_at_pdu.y);

                        scm_draw_square_at_pdu.x = csm_click_at_pdu.x;
                        scm_draw_square_at_pdu.y = csm_click_at_pdu.y;

                        //for(i=0;i<sizeof(struct scm_draw_square_at);i++)
                        //  p->data[i]=0;

                        p->address.host = client_address.host;
                        p->address.port = client_address.port;
                        p->len=12;

                        for(i=0;i<10;i++)
                        {
                        
                        rx = rand()%10; rand()%10 + rand()%10 - 15;
                        ry = rand()%10; rand()%10 + rand()%10 - 15;

                        sprintf((char *)(p->data),"%d %d %d", csm_click_at_pdu.x+rx, csm_click_at_pdu.y+ry, 1);
                        SDLNet_UDP_Send(udp_sd,-1,p); 
                        SDL_Delay(50);
                        }


                    }

                }
            }
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
                                sprintf((char *)pSent->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", j, getSoldierId(soldiers[0]), getSoldierPositionX(soldiers[0]), getSoldierPositionY(soldiers[0]), getSoldierFrame(soldiers[0]), getSoldierId(soldiers[1]), getSoldierPositionX(soldiers[1]), getSoldierPositionY(soldiers[1]), getSoldierFrame(soldiers[1]), getSoldierId(soldiers[2]), getSoldierPositionX(soldiers[2]), getSoldierPositionY(soldiers[2]), getSoldierFrame(soldiers[2]), getSoldierId(soldiers[3]), getSoldierPositionX(soldiers[3]), getSoldierPositionY(soldiers[3]), getSoldierFrame(soldiers[3]));
                                pSent->len = strlen((char *)pSent->data) + 1;
                            }else if(pRecive->len < 8){
                                int connParams[2];
                                sscanf((char * )pRecive->data, "%d %d\n", &connParams[0], &connParams[1]);
                                setSoldierId(soldiers[playerId], connParams[0]);
                                setSoldierShotFired(soldiers[playerId], connParams[1]);
                                sprintf((char *)pSent->data, "%d %d %d %d %d %d %d %d %d\n", j, getSoldierId(soldiers[0]), getSoldierShotFired(soldiers[0]), getSoldierId(soldiers[1]), getSoldierShotFired(soldiers[1]), getSoldierId(soldiers[2]), getSoldierShotFired(soldiers[2]), getSoldierId(soldiers[3]), getSoldierShotFired(soldiers[3]));
                                pSent->len = strlen((char *)pSent->data) + 1;                            
                            }
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