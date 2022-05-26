#ifndef lobby_h
#define lobby_h
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"

typedef struct lobby *Lobby;

//Functions
Lobby createLobby(SDL_Renderer *gRenderer);
int lobbyApplication(Lobby l, int lobbyState);
void pushLobbyPlayer(Lobby l, char path[], char name[]);
int lobbyMenu(Lobby l, int lobbyState);
void destroyLobby(Lobby l);
void showCurrentLobbyPlayers(Lobby l);


#endif