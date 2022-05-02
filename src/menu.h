#ifndef menu_h
#define menu_h
#include "SDL2/SDL.h"

typedef struct menu *Menu;


//Funcitons
Menu createMenu(SDL_Renderer *gRenderer);
int menuApplication(Menu m);
char* getIpAddress(Menu m);
char getGameType(Menu m);

#endif