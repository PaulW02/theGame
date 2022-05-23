#ifndef lobby_h
#define lobby_h
#include "SDL2/SDL.h"


typedef struct lobby *Lobby;

//Functions
Lobby createLobby(SDL_Renderer *gRenderer);
int lobbyApplication(Lobby l);
void pushLobbyPlayer(Lobby l, char path[], char name[], int id);
int lobbyMenu(Lobby l);
void showCurrentLobbyPlayers(Lobby l);


#endif