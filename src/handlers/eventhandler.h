#ifndef eventhandler_h
#define eventhandler_h

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../player/soldier.h"

void movementInput(SDL_Event theApp, Soldier s);
void motion(Soldier s, int *pframe, SDL_Rect *healthBarPosition);

#endif