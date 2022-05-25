#ifndef endScreen_h
#define endScreen_h
#include "SDL2/SDL.h"
#include "player/soldier.h"
typedef struct endScreen *EndScreen;

EndScreen createEndScreen(SDL_Renderer *gRenderer, Soldier soldiers[]);
int endScreenApplication(EndScreen es);

#endif