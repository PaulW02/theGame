#ifndef initnetwork_h
#define initnetwork_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include "../menu.h"
#include <stdio.h>
#include <stdbool.h>


void initConnection(TCPsocket *tcp_sd, IPaddress *srvadd, Menu m);

#endif