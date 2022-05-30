#ifndef lobby_h
#define lobby_h
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"

#define MAX_NAME 16
#define PATHLENGTH 64

typedef struct lobby *Lobby;

//Functions
Lobby createLobby(SDL_Renderer *gRenderer);
void pushLobbyPlayer(Lobby l, char path[], char name[], int id);
void renderCharacterText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size);
void destroyLobby(Lobby l);

#endif