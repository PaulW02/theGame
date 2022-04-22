#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "menu.h"
#include "application.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define SCROLL_SPEED 5


struct menu{
    SDL_Renderer *gRenderer;
    char ipAdress[16];
    int gameModeType; //2v2, 
    //Implement type of gamemode, ex 2v2 or free for all
};

//Function declarations
PUBLIC int menuApplication(Menu m);
PUBLIC void destroyMenu(Menu m);
PRIVATE void renderImage(Menu m, char *imageName, int posY, int scaleModifier);
PRIVATE int startPage(Menu m);

PUBLIC Menu createMenu(SDL_Renderer *gRenderer)
{
    Menu m = malloc(sizeof(struct menu));
    m->gRenderer = gRenderer;
    m->gameModeType = 0;
    strcpy(m->ipAdress,"123.123.123.123");
    return m;
}

PUBLIC int menuApplication(Menu m)
{      
    int result = 0;
    
    result = startPage(m);
    
    
    
    if(result==-1) return result;

}

//Isn't implemented yet
PUBLIC void destroyMenu(Menu m)
{
    SDL_DestroyRenderer(m->gRenderer);
    free(m);
}

PRIVATE int startPage(Menu m)
{

    SDL_Surface* titleSurface = IMG_Load("resources/menu/theGame.png");

    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(m->gRenderer,titleSurface);
    SDL_FreeSurface(titleSurface);

    SDL_Rect titleRect;
    SDL_QueryTexture(titleTex,NULL,NULL,&titleRect.w,&titleRect.h);
    titleRect.x=(WINDOW_WIDTH-titleRect.w)/2;
    titleRect.y=WINDOW_HEIGHT;

    
    bool windowCloseRequest = false, titleAppearing = true, pressAnyButtonVisible = false;

    while(!windowCloseRequest)
    {
        SDL_Event event;
        //Returns 1 if there was an event
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    windowCloseRequest=true;
                    break;
                
                case SDL_KEYDOWN:
                    return 0;
                    break;
                
                default:
                    break;
            }
        }
        if(titleAppearing)
        {
            //Animation happends here
            SDL_RenderClear(m->gRenderer);
            SDL_RenderCopy(m->gRenderer,titleTex,NULL,&titleRect);
            SDL_RenderPresent(m->gRenderer);

            //Moves the game title upwards
            titleRect.y-=SCROLL_SPEED;
            
            //Checks if the title has reached its goal
            if(titleRect.y<=100) titleAppearing=false;

            //Time for a frame
            SDL_Delay(1000/60);
        }
        else if(!titleAppearing && !pressAnyButtonVisible)
        {
            renderImage(m,"pressAnyButton.png",300,2);
            SDL_RenderPresent(m->gRenderer);
            pressAnyButtonVisible=true;
        }
    }
    destroyMenu(m);
    return -1;

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

PUBLIC char* getIpAdress(Menu m)
{
    return m->ipAdress;
}