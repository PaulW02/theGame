#ifndef menu_h
#define menu_h
#include "SDL2/SDL.h"

typedef struct menu *Menu;


//Funcitons
Menu createMenu(SDL_Renderer *gRenderer);
void startMenu(Menu m);

#endif