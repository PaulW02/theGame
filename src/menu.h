#ifndef menu_h
#define menu_h
#include "SDL2/SDL.h"

typedef struct menu *Menu;


//Funcitons
Menu createMenu(SDL_Renderer *gRenderer, SDL_Window *window, SDL_Surface *window_surface);
int menuApplication(Menu m);

#endif