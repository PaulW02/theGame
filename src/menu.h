#ifndef menu_h
#define menu_h
#include "SDL2/SDL.h"

typedef struct menu *Menu;


//Init
Menu createMenu(SDL_Renderer *gRenderer);
int menuApplication(Menu m, int menuIndex);

//Functions
//void renderImage(Menu m, char *imageName, int posX, int posY, int scaleModifier,Uint8 alpha);
//void renderText(Menu m, char *textToRender, SDL_Color color, int x, int y, int size);

//Get and set for the struct values
char* getIpAddress(Menu m);
char getGameType(Menu m);
int getCharacter(Menu m);
int getWeapon(Menu m);
char* getPathToCharacter(Menu m);
char* getPlayerName(Menu m);


void renderText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size);
void renderImage(SDL_Renderer *gRenderer, char *imageName, int posX, int posY, int scaleModifier,Uint8 alpha);
void renderImageEx(SDL_Renderer *gRenderer, char *path,int posX,int posY, int flip, int directionFrame, Uint8 alpha);


#endif