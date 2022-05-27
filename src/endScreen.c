#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "menu.h"
#include "lobby.h"
#include "endScreen.h"
#include "handlers/playerhandler.h"
#include "player/soldier.h"

#define CLOSEREQUSTED -1
#define SCOREBOARDMENU 1

#define PUBLIC /* empty */
#define PRIVATE static

//Function declarations
PUBLIC EndScreen createEndScreen(SDL_Renderer *gRenderer, Soldier soldiers[]);
PRIVATE void destroyEndScreen(EndScreen es);
PRIVATE int endScreenMenu(EndScreen es);

struct endScreen{
    SDL_Renderer *gRenderer;
    SDL_Event windowEvent;
    Soldier soldiers[4];
};

PUBLIC EndScreen createEndScreen(SDL_Renderer *gRenderer, Soldier soldiers[])
{
    EndScreen es = malloc(sizeof(struct endScreen));
    SDL_Event windowEvent;
    es->windowEvent=windowEvent;
    es->gRenderer=gRenderer;
    es->soldiers[0]=soldiers[0];
    es->soldiers[1]=soldiers[1];
    es->soldiers[2]=soldiers[2];
    es->soldiers[3]=soldiers[3];
}

PUBLIC int endScreenApplication(EndScreen es)
{
    int result = 1;
    bool closeRequested = false;
    while(!closeRequested)
    {
        switch (result)
        {
            case CLOSEREQUSTED:
                closeRequested=true;
                destroyEndScreen(es);
                break;
            
            case SCOREBOARDMENU:
                result = endScreenMenu(es);
                break;
            
            default:
                break;
        }
    }
    return result;
}

PRIVATE void destroyEndScreen(EndScreen es){
    free(es);
}
PRIVATE int endScreenMenu(EndScreen es){
    bool closeRequested = false;
    bool interfaceAppeared = false;
    while(!closeRequested){
        while(SDL_PollEvent(&es->windowEvent)){
            switch(es->windowEvent.type){
                case SDL_QUIT:
                    closeRequested=true;
                    break;
                default:
                    break;
            }
        }
        if(!interfaceAppeared){
            SDL_SetRenderDrawColor(es->gRenderer, 0x00,0x00,0x00,SDL_ALPHA_OPAQUE);
            SDL_RenderClear(es->gRenderer);
            scoreBoard(es->soldiers, es->gRenderer);
            SDL_RenderPresent(es->gRenderer);
        }
    }
    return CLOSEREQUSTED;
}