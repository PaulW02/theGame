#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "lobby.h"
#include "menu.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define CHARACTERWIDTH 96
#define WORDLENGTH 16
#define PATHLENGTH 64

#define MAXNUMBEROFPLAYERS 4

#define CLOSEREQUSTED -1
#define LOBBYMENU 1


//Function declarations
PUBLIC int lobbyMenu(Lobby l, int lobbyState);
PUBLIC void destroyLobby(Lobby l);
PUBLIC void renderCharacterText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size);


struct player{
    char playerPath[PATHLENGTH];
    char playerName[WORDLENGTH];
};
typedef struct player Player;

struct lobby{
    SDL_Renderer *gRenderer;
    SDL_Event windowEvent;
    int numberOfPlayers;
    Player players[MAXNUMBEROFPLAYERS];
};


PUBLIC Lobby createLobby(SDL_Renderer *gRenderer)
{
    Lobby l = malloc(sizeof(struct lobby));
    SDL_Event windowEvent;
    l->numberOfPlayers=0;
    l->windowEvent=windowEvent;
    l->gRenderer=gRenderer;
}

/*
 Adds a player struct to an array of players with the parts 'path' and 'name'.
    - Path should be something in the style of :'resources/Karaktarer/SKELETON/SKELETONpistol.png'.
    - Name should be below 16 character which it will be if code is executed correctly
*/
PUBLIC void pushLobbyPlayer(Lobby l, char path[], char name[], int id)
{
    if(id<MAXNUMBEROFPLAYERS)
    {
        strcpy(l->players[id].playerPath,path);
        strcpy(l->players[id].playerName,name);
        if(id == l->numberOfPlayers){
            l->numberOfPlayers++;
        }
        return;
    }
}

PUBLIC void renderCharacterText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size)
{
    TTF_Font* font = TTF_OpenFont("resources/fonts/8bitOperatorPlus-Regular.ttf",size);
    SDL_Surface* text = TTF_RenderText_Solid(font,textToRender,color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer,text);
 
    SDL_Rect rect;
    SDL_QueryTexture(textTexture,NULL,NULL,&rect.w,&rect.h);

    rect.x=((CHARACTERWIDTH-rect.w)/2)+x;
    rect.y=y;

    SDL_RenderCopy(gRenderer,textTexture,NULL,&rect);
            
    //Cleans up resources
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);

    return;
}

PUBLIC void destroyLobby(Lobby l)
{
        free(l);
}