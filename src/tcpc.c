/*#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` tcpc.c -o tcpc `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL2/SDL_net.h"
 
struct movement{
    int id;
	int x;
	int y;
};
typedef struct movement Movement;

struct playerConnDetails{
    TCPsocket sock;
	int id;
};
typedef struct playerConnDetails PlayerConnDetails;

int main(int argc, char **argv)
{
	IPaddress ip;		/* Server address 
	TCPsocket sd;		/* Socket descriptor 
	int quit, len;
	char buffer[512];
    Movement clientMovement;
    Movement playersMovement[4];
 
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	Resolve the host we are connecting to 
	if (SDLNet_ResolveHost(&ip, "127.0.0.1", 1234) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	 Open a connection with the IP provided (listen on the host's port) 
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	 Send messages 
	quit = 0;
           clientMovement.id = -1;
        clientMovement.x = 50;
        clientMovement.y = 100;
	while (!quit)
	{

 
		if (SDLNet_TCP_Send(sd, &clientMovement, sizeof(struct movement)) < sizeof(struct movement))
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

        printf("In the middle of TCP Send and Recv");

        SDLNet_TCP_Recv(sd, playersMovement, 4*sizeof(struct movement));
        for (int i = 0; i < 2; i++)
        {
            printf("PlayerId: %d %d %d\n", playersMovement[i].id, playersMovement[i].x, playersMovement[i].y);
        }
        

	}
 
	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;
} 
*/