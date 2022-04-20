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
PRIVATE void renderImage(Menu m, char *imageName, int posY, int scaleModifier);



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

    SDL_Surface* titleSurface = IMG_Load("resources/menu/theGame.png");

    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(m->gRenderer,titleSurface);
    SDL_FreeSurface(titleSurface);

    SDL_Rect titleRect;
    SDL_QueryTexture(titleTex,NULL,NULL,&titleRect.w,&titleRect.h);
    titleRect.x=(WINDOW_WIDTH-titleRect.w)/2;
    titleRect.y=100;

    /*
    while(titleRect.y <= 100)
    {
        SDL_RenderClear(m->gRenderer);
        
        //Inte klart
        //SDL_RenderCopy(m->gRenderer,titleTex,NULL,&titleRect);


        //Time for a frame
        SDL_Delay(1/6000);
    }
    */
  
    SDL_RenderClear(m->gRenderer);
    SDL_RenderCopy(m->gRenderer,titleTex,NULL,&titleRect);
    
    SDL_RenderPresent(m->gRenderer);

    //Press any button appears
    
    renderImage(m,"pressAnyButton.png",300,2);
    SDL_RenderPresent(m->gRenderer);
    

    SDL_Delay(5000);

    free(m);
}


PRIVATE void renderImage(Menu m, char *imageName, int posY, int scaleModifier)
{
    char path[42] = "resources/menu/";
    strcat(path,imageName);
    SDL_Surface* s = IMG_Load(path);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m->gRenderer,s);
    SDL_FreeSurface(s);

    SDL_Rect dest;

    SDL_QueryTexture(tex, NULL,NULL,&dest.w,&dest.h);
    
    //Scales the image up or down
    dest.w/=scaleModifier;
    dest.h/=scaleModifier;

    //Defines the position of the images top-left corner
    dest.x=(WINDOW_WIDTH-dest.w)/2;
    dest.y=posY;


    SDL_RenderCopy(m->gRenderer,tex,NULL,&dest);

    return;
}