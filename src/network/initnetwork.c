#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include "initnetwork.h"
#include "../menu.h"
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void initConnection(UDPsocket *sd, TCPsocket *tcp_sd, IPaddress *srvadd, UDPpacket **p, UDPpacket **p2, Menu m){
    if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	char *ip = getIpAddress(m);
    /* Resolve server name  */
	if (SDLNet_ResolveHost(srvadd, ip, 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
/*
    if (!(*sd = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
*/
    if (!(*tcp_sd = SDLNet_TCP_Open(srvadd)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
/*
    if (!((*p = SDLNet_AllocPacket(1024))&& (*p2 = SDLNet_AllocPacket(1024))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
*/
}
