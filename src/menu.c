#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "menu.h"
#include "application.h"
#include <stdio.h>
#include <string.h>


#define PUBLIC /* empty */
#define PRIVATE static

struct menu{
    char ipAdress[16];
    SDL_Renderer *gRenderer;
    //Implement type of gamemode, ex 2v2 or free for all
};

PUBLIC Menu createMenu(SDL_Renderer *gRenderer)
{
    Menu m = malloc(sizeof(struct menu));
    m->gRenderer = gRenderer;
    //strcpy(m->ipAdress,"123.123.123.123"); Ex
    return m;
}

void startMenu(Menu m)
{
    printf("You made it here!\n");
    printf("Saved string: %s", m->ipAdress);
    getchar();
}
