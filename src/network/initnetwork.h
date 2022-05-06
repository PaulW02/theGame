#ifndef initnetwork_h
#define initnetwork_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>


void initConnection(UDPsocket *sd, TCPsocket *tcp_sd, IPaddress *srvadd, UDPpacket **p, UDPpacket **p2);

#endif