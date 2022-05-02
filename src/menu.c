#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "menu.h"
#include "application.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_ttf.h"


#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define SCROLL_SPEED 5
#define TITLECARD 1
#define MAINMENU 2
#define CLOSEREQUSTED -1
#define HOWTOPLAYMENU 3
#define ONLINEMENU 4
#define IPWORDLENGTH 16


struct menu{
    SDL_Renderer *gRenderer;
    SDL_Event event;
    TTF_Font* font;
    char ipAddress[IPWORDLENGTH];
    char gameModeType; //2v2, 
    //Implement type of gamemode, ex 2v2 or free for all
};

//Function declarations
PUBLIC int menuApplication(Menu m);
PUBLIC void destroyMenu(Menu m);
PRIVATE void renderImage(Menu m, char *imageName, int posX, int posY, int scaleModifier);
PRIVATE int startPage(Menu m);
PRIVATE int mainMenu(Menu m);
PRIVATE bool checkImageBoxForCursor(char *imageName, int imagePosX, int imagePosY, int *button);
PRIVATE void resetMainMenu(Menu m);
PRIVATE int howToPlayMenu(Menu m);
PRIVATE int onlineMenuConfig(Menu m);
PRIVATE void renderText(Menu m, char *textToRender, SDL_Color color, int x, int y, int w, int h);



PUBLIC Menu createMenu(SDL_Renderer *gRenderer)
{
    Menu m = malloc(sizeof(struct menu));
    SDL_Event windowEvent;

    //Initilizing TTF
    if(TTF_Init()<0) printf("Error with init!\n");
    m->font = TTF_OpenFont("resources/fonts/8bitOperatorPlus-Regular.ttf",30);
    m->gRenderer = gRenderer;
    m->gameModeType = '\0';
    m->event = windowEvent;
    strcpy(m->ipAddress,"\0");
    return m;
}

PUBLIC int menuApplication(Menu m)
{      
    int result = 1;
    bool closeRequested = false;
    while(!closeRequested)
    {
        switch (result)
        {
        case CLOSEREQUSTED:
            destroyMenu(m);
            closeRequested=true;
            break;

        case TITLECARD:
            result = startPage(m);
            break;
        
        case MAINMENU:
            result = mainMenu(m);
            break;

        case HOWTOPLAYMENU:
            result = howToPlayMenu(m);
            break;
        
        case ONLINEMENU:
            result = onlineMenuConfig(m);
            break;

        default:
            return 0;
            break;
        }
    }
    return result;

}

//Isn't implemented yet
PUBLIC void destroyMenu(Menu m)
{
    TTF_CloseFont(m->font);
    SDL_DestroyRenderer(m->gRenderer);
    TTF_Quit();
    free(m);
}

PUBLIC char* getIpAddress(Menu m)
{
    return m->ipAddress;
}

PUBLIC char getGameType(Menu m)
{
    return m->gameModeType;
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
        //Returns 1 if there was an event
        while (SDL_PollEvent(&m->event))
        {
            switch(m->event.type)
            {
                case SDL_QUIT:
                    windowCloseRequest=true;
                    break;
                
                case SDL_KEYDOWN:
                    return MAINMENU;
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
            renderImage(m,"pressAnyButton.png",-1,300,2);
            SDL_RenderPresent(m->gRenderer);
            pressAnyButtonVisible=true;
        }
    }
    return CLOSEREQUSTED;

}

PRIVATE void renderImage(Menu m, char *imageName,int posX,int posY, int scaleModifier)
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
    if(posX==-1) dest.x=(WINDOW_WIDTH-dest.w)/2;
    else dest.x=posX;  
    
    dest.y=posY;

    SDL_RenderCopy(m->gRenderer,tex,NULL,&dest);
    SDL_DestroyTexture(tex);
    return;
}

PRIVATE int mainMenu(Menu m)
{
    //Typsnitt: Showcard Gothic, Storlek 48

    bool windowCloseRequested = false, userInterfaceAppeard = false;
    int menuChoice = 0, button;
    
    while(!windowCloseRequested)
    {
        while(SDL_PollEvent(&m->event))
        {
            if(m->event.type==SDL_QUIT)
                windowCloseRequested=true;
        }
        
        if(!userInterfaceAppeard)
        {
            SDL_RenderClear(m->gRenderer);
            renderImage(m,"theGame.png",-1,20,1);
            resetMainMenu(m);
            SDL_RenderPresent(m->gRenderer);
            userInterfaceAppeard=true;
        }
        
        //Checks which option the mouse is hovering on.
        if(checkImageBoxForCursor("onlineOption.png",-1,225,&button)) menuChoice=0;
        if(checkImageBoxForCursor("howToPlay.png",-1,325,&button)) menuChoice=1;
        if(checkImageBoxForCursor("co_op.png",-1,425,&button)) menuChoice=2;

        switch (menuChoice)
        {
        case 0:
            resetMainMenu(m);
            renderImage(m,"onlineOptionGray.png",-1,225,1);
            if(button)
            {
                m->gameModeType='O';
                return ONLINEMENU;
            }
            break;
        
        case 1:
            resetMainMenu(m);
            renderImage(m,"howToPlayGray.png",-1,325,1);
            if(button)
            {
                m->gameModeType='H';
                return HOWTOPLAYMENU;
            }
            break;
        
        case 2:
            resetMainMenu(m);
            renderImage(m,"co_opGray.png",-1,425,1);
            if(button)
            {
                m->gameModeType='C';
                return 0;
            }
            break;

        default:
            break;
        }
        SDL_RenderPresent(m->gRenderer);
    }
    return CLOSEREQUSTED;
}

PRIVATE bool checkImageBoxForCursor(char *imageName, int imagePosX, int imagePosY, int *button)
{
    int cursorX, cursorY;
    char path[42] = "resources/menu/";
    
    //Collecting neccesary values
    strcat(path,imageName);
    SDL_Surface* s = IMG_Load(path);

    //PumpEvent, updates the event queue
    SDL_PumpEvents();
    *button = SDL_GetMouseState(&cursorX,&cursorY);

    if(imagePosX==-1) imagePosX=(WINDOW_WIDTH-(s->w))/2;

    
    if(cursorX>imagePosX && cursorX<imagePosX+s->w && cursorY>imagePosY && cursorY<imagePosY+s->h)
    {
        return true;
    } 
    return false;   
}

PRIVATE void resetMainMenu(Menu m)
{
    renderImage(m,"onlineOption.png",-1,225,1);
    renderImage(m,"howToPlay.png",-1,325,1);
    renderImage(m,"co_op.png",-1,425,1);
    return;
}

PRIVATE int howToPlayMenu(Menu m)
{
    bool windowCloseRequested = false, userInterfaceAppeard = false;
    while(!windowCloseRequested)
    {
        while(SDL_PollEvent(&m->event))
        {
            switch (m->event.type)
            {
            case SDL_QUIT:
                windowCloseRequested=true;
                break;
            case SDL_KEYDOWN:
                if(m->event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return MAINMENU;
                break;
            
            default:
                break;
            }
        }
        if(!userInterfaceAppeard)
        {
            SDL_RenderClear(m->gRenderer);
            renderImage(m,"howToPlay.png",-1,100,1);
            SDL_RenderPresent(m->gRenderer);

            //Add text here for how the game is to be played!

        }

    }
    return CLOSEREQUSTED;
}

PRIVATE int onlineMenuConfig(Menu m)
{
    bool windowCloseRequested = false, userInterfaceAppeard = false;
    char ipPlaceholder[16] = "\0";
    SDL_Color color = {0xFF,0xFF,0xFF}; //White

    while(!windowCloseRequested)
    {
        while(SDL_PollEvent(&m->event))
        {
            switch (m->event.type)
            {
                case SDL_QUIT:
                    windowCloseRequested=true;
                    break;
                case SDL_KEYDOWN:
                    switch(m->event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            SDL_StopTextInput();
                            return MAINMENU;
                            break;

                        case SDL_SCANCODE_BACKSPACE:
                            if(strlen(ipPlaceholder)!=0)
                            {
                                ipPlaceholder[strlen(ipPlaceholder)-1]='\0';
                                renderText(m,ipPlaceholder,color,(WINDOW_WIDTH-300)/2,200,300,50);
                            }
                            break;
                        
                        case SDL_SCANCODE_RETURN:
                            //printf("You pressed enter!\n");
                            strcpy(m->ipAddress,ipPlaceholder);
                            //printf("You copied: %s \n",m->ipAddress);
                            SDL_StopTextInput();
                            return 0;
                            break;

                        default:
                            break;
                    }
                    break;
                
                case SDL_TEXTINPUT:
                    if(m->event.text.text[0]>=48 && m->event.text.text[0]<=57 || m->event.text.text[0]==46)
                    {
                        if(strlen(ipPlaceholder)<IPWORDLENGTH-1)
                        {
                            strncat(ipPlaceholder,m->event.text.text,1);
                            renderText(m,ipPlaceholder,color,(WINDOW_WIDTH-300)/2,200,300,50);
                            //printf("%c",m->event.text.text[0]);
                        }
                    }
                    break;
                
                default:
                    break;
            }
        }
        if(!userInterfaceAppeard)
        {
            SDL_StartTextInput();
            SDL_RenderClear(m->gRenderer);
            
            renderImage(m,"onlineOption.png",-1,50,1);

            //Renders a centered rectangle with white border
            SDL_Rect ipInputRect = {(WINDOW_WIDTH-300)/2,200,300,50}; //x,y,w,h
            SDL_SetRenderDrawColor(m->gRenderer,0xFF,0xFF,0xFF,SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(m->gRenderer,&ipInputRect);

            SDL_RenderPresent(m->gRenderer);
            //Restores the drawcolor to its original state 
            SDL_SetRenderDrawColor(m->gRenderer,0x00,0x00,0x00,SDL_ALPHA_OPAQUE);
            userInterfaceAppeard=true;
        }
        SDL_RenderPresent(m->gRenderer);

    }
    SDL_StopTextInput();
    return CLOSEREQUSTED;
}

PRIVATE void renderText(Menu m, char *textToRender, SDL_Color color, int x, int y, int w, int h)
{        
    SDL_Rect ipInputBox = {x,y,w,h};
    SDL_RenderFillRect(m->gRenderer,&ipInputBox);
    SDL_SetRenderDrawColor(m->gRenderer,0xFF,0xFF,0xFF,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(m->gRenderer,&ipInputBox);
    SDL_SetRenderDrawColor(m->gRenderer,0x00,0x00,0x00,SDL_ALPHA_OPAQUE);

    SDL_Surface* text = TTF_RenderText_Solid(m->font,textToRender,color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m->gRenderer,text);
 
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    SDL_QueryTexture(textTexture,NULL,NULL,&rect.w,&rect.h);
    SDL_RenderCopy(m->gRenderer,textTexture,NULL,&rect);
            
    //Cleans up resources
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(text);

    return;
}