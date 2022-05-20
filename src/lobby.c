#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "lobby.h"
#include "menu.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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
PUBLIC Lobby createLobby(SDL_Renderer *gRenderer);
PUBLIC int lobbyApplication(Lobby l);
PRIVATE int lobbyMenu(Lobby l);
PRIVATE void destroyLobby(Lobby l);
PRIVATE void renderCharacterText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size);


PUBLIC void pushLobbyPlayer(Lobby l, char path[], char name[]);


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


//Function declarations
PUBLIC Lobby createLobby(SDL_Renderer *gRenderer);
PUBLIC int lobbyApplication(Lobby l);
PRIVATE int lobbyMenu(Lobby l);
PRIVATE void destroyLobby(Lobby l);
PRIVATE void renderCharacterText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size);

PUBLIC void pushLobbyPlayer(Lobby l, char path[], char name[]);


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
PUBLIC void pushLobbyPlayer(Lobby l, char path[], char name[])
{
    if(l->numberOfPlayers<MAXNUMBEROFPLAYERS)
    {
        strcpy(l->players[l->numberOfPlayers].playerPath,path);
        strcpy(l->players[l->numberOfPlayers].playerName,name);
        l->numberOfPlayers++;
        return;
    }
}

PUBLIC int lobbyApplication(Lobby l)
{
    int result = 1;
    bool closeRequested = false;
    while(!closeRequested)
    {
        switch (result)
        {
            case CLOSEREQUSTED:
                closeRequested=true;
                destroyLobby(l);
                break;
            
            case LOBBYMENU:
                result = lobbyMenu(l);
                break;
            
            default:
                return 0;
                break;
        }
    }
    return result;
}

PRIVATE int lobbyMenu(Lobby l)
{
    bool closeRequested = false;
    SDL_Color colorWhite = {0xFF,0xFF,0xFF}; //White
    SDL_Color colors[] = {{0xFF,0x00,0x00},  //Red
                          {0x00,0xFF,0x00},  //Green
                          {0x00,0x00,0xFF},  //Blue
                          {0xFF,0x00,0xFF}}; //Pink
    int posX = 0, padding = 70;
    int posY[4] = {275,300,300,275};

    //Countdown [WIP]
    //Uint32 ticks, seconds, startTimeValue;
    //char countdownNumber[3];
    //int countdown = 30;
    
    //Countdown [WIP]
    //startTimeValue = SDL_GetTicks() / 1000;

    while(!closeRequested)
    {
        /*
        //Countdown [WIP]
        ticks = SDL_GetTicks();
        seconds = (ticks/1000)% 2 + 1;
        */

        while(SDL_PollEvent(&l->windowEvent))
        {
            switch(l->windowEvent.type)
            {
                case SDL_QUIT:
                    closeRequested=true;
                    break;
                case SDL_KEYDOWN:
                    switch (l->windowEvent.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_RETURN:
                        return 0; 
                        break;
                    
                    default:
                        break;
                    }
                    break;
            }

            if(l->players[0].playerPath[0] != '\0')
            {
                SDL_RenderClear(l->gRenderer);
                renderImage(l->gRenderer,"lobby.png",-1,150,1,255);
                
                /*
                //Countdown [WIP]
                sprintf(countdownNumber,"%d",countdown);
                renderText(l->gRenderer,countdownNumber,colorWhite,-1,50,24);
                */

                for(int i = 0; i < l->numberOfPlayers; i++)
                {
                    posX = (WINDOW_WIDTH/2)-(WINDOW_WIDTH/5)*2 + (WINDOW_WIDTH/5)*i;

                    renderImageEx(l->gRenderer,l->players[i].playerPath,posX,300,SDL_FLIP_NONE,0,SDL_ALPHA_OPAQUE);
                    renderCharacterText(l->gRenderer,l->players[i].playerName,colors[i],posX,posY[i]-50,24);
                }
                SDL_RenderPresent(l->gRenderer);

            }
        }
    }
    return CLOSEREQUSTED;
}

PRIVATE void renderCharacterText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size)
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

PRIVATE void destroyLobby(Lobby l)
{
        free(l);
}