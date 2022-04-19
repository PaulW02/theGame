#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "menu.h"
#include "application.h"
#include <stdio.h>
#include <string.h>


#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512


struct menu{
    SDL_Renderer *gRenderer;
    char ipAdress[16];
    //Implement type of gamemode, ex 2v2 or free for all
};

//Function declarations
PRIVATE void StartPage(Menu m);
PRIVATE void renderImage(Menu m, char *imageName, int posY);



PUBLIC Menu createMenu(SDL_Renderer *gRenderer)
{
    Menu m = malloc(sizeof(struct menu));
    m->gRenderer = gRenderer;
    //strcpy(m->ipAdress,"123.123.123.123"); Ex
    return m;
}

PUBLIC void MenuApplication(Menu m)
{
    PRIVATE int page = 0;

    //printf("You made it here!\n");
    //getchar();
    StartPage(m);
}

PRIVATE void StartPage(Menu m)
{

    //Clears the memory
    SDL_RenderClear(m->gRenderer);
        
    renderImage(m,"theGame.png",100);
    renderImage(m,"pressAnyButton.png",200);
    SDL_RenderPresent(m->gRenderer);

    SDL_Delay(5000);

    free(m);
}


PRIVATE void renderImage(Menu m, char *imageName, int posY)
{
    char path[42] = "resources/menu/";
    strcat(path,imageName);
    SDL_Surface* s = IMG_Load(path);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m->gRenderer,s);
    SDL_FreeSurface(s);

    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL,NULL,&dest.w,&dest.h);
    dest.x=(WINDOW_WIDTH-dest.w)/2;
    dest.y=posY;

    SDL_RenderCopy(m->gRenderer,tex,NULL,&dest);

    return;
}