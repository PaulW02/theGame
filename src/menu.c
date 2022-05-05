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
#define PICKCHARACTERMENU 5
#define IPWORDLENGTH 16
#define PATH_TO_ARROW "resources/menu/arrow.png"


struct menu{
    SDL_Renderer *gRenderer;
    SDL_Event event;
    TTF_Font* font;
    char ipAddress[IPWORDLENGTH];
    char characterModel[64];
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
PRIVATE void renderTextInputBox(Menu m, int x, int y, int w, int h);
PRIVATE void renderText(Menu m, char *textToRender, SDL_Color color, int x, int y);
PRIVATE int pickCharacterMenu(Menu m);
PRIVATE void renderImageEx(Menu m, char *path,int posX,int posY, int flip, int directionFrame, Uint8 alpha);
PRIVATE void renderCharacterMenuUI(Menu m);




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
            
            case PICKCHARACTERMENU:
                result = pickCharacterMenu(m);
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
                                renderTextInputBox(m,(WINDOW_WIDTH-300)/2,200,300,50);
                                renderText(m,ipPlaceholder,color,(WINDOW_WIDTH-300)/2,200);
                            }
                            break;
                        
                        case SDL_SCANCODE_RETURN:
                            //printf("You pressed enter!\n");
                            strcpy(m->ipAddress,ipPlaceholder);
                            //printf("You copied: %s \n",m->ipAddress);
                            SDL_StopTextInput();
                            return PICKCHARACTERMENU;
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
                            renderTextInputBox(m,(WINDOW_WIDTH-300)/2,200,300,50);
                            renderText(m,ipPlaceholder,color,(WINDOW_WIDTH-300)/2,200);
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

PRIVATE void renderTextInputBox(Menu m, int x, int y, int w, int h)
{
    SDL_Rect ipInputBox = {x,y,w,h};
    SDL_RenderFillRect(m->gRenderer,&ipInputBox);
    SDL_SetRenderDrawColor(m->gRenderer,0xFF,0xFF,0xFF,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(m->gRenderer,&ipInputBox);
    SDL_SetRenderDrawColor(m->gRenderer,0x00,0x00,0x00,SDL_ALPHA_OPAQUE);

}

PRIVATE void renderText(Menu m, char *textToRender, SDL_Color color, int x, int y)
{        
    SDL_Surface* text = TTF_RenderText_Solid(m->font,textToRender,color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m->gRenderer,text);
 
    SDL_Rect rect;
    

    SDL_QueryTexture(textTexture,NULL,NULL,&rect.w,&rect.h);

    if(x==-1) rect.x=(WINDOW_WIDTH-rect.w)/2;
    else rect.x=x;
    rect.y=y;

    SDL_RenderCopy(m->gRenderer,textTexture,NULL,&rect);
            
    //Cleans up resources
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(text);

    return;
}

PRIVATE int pickCharacterMenu(Menu m)
{
    bool windowCloseRequested = false, spin = true;
    int button=0, hoveringOption = 5, characterIndex = 0, weaponIndex = 0, mouseButtonDown = 0;
    int alpha[]={255,255,255,255};
    int repeat = 0;
    Uint32 ticks, seconds, sprite;

    char characterName [7][10] = {"Boy", "Female", "Ghost", "Male", "Pumpkin", "Skeleton", "Wizard"};
    //char characterWeapon [5][9] = {"Bow", "Pistol", "Blue rod", "Red rod", "Spear"};
    SDL_Color color = {0xFF,0xFF,0xFF}; 


    while(!windowCloseRequested)
    {
        //SDL_Delay(1000/60) ger 60 frames per sekund

        ticks = SDL_GetTicks();
        seconds = ticks/300;
        sprite = seconds % 8;
        
        while(SDL_PollEvent(&m->event))
        {
            switch (m->event.type)
            {
                case SDL_QUIT:
                    windowCloseRequested=true;
                    break;
                
                case SDL_KEYDOWN:
                    switch (m->event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            return ONLINEMENU;
                            break;
                        
                        default:
                            return 0;
                            break;
                    }
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    mouseButtonDown=1;
                    break;

                default:
                    break;
            }
        }
        if(spin)
        {

            SDL_RenderClear(m->gRenderer);

            //The animation 
            renderImageEx(m,"resources/Karaktarer/BOY/BOYbow.png",-1,200,SDL_FLIP_NONE,sprite,SDL_ALPHA_OPAQUE);

            //Arrow options
            if(hoveringOption<4) alpha[hoveringOption]=SDL_ALPHA_OPAQUE;
            if(checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)-70,350,&button)) hoveringOption=0;    
            if(checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)+60,350,&button)) hoveringOption=1;
            if(checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)-70,400,&button)) hoveringOption=2;
            if(checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)+60,400,&button)) hoveringOption=3;
            alpha[hoveringOption]=144;

            //renderCharacterMenuUI(m);
            renderImage(m,"pickCharacter.png",-1,20,1);

            //Top buttons
            renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)-100,350,SDL_FLIP_HORIZONTAL,-1,alpha[0]);
            /*
            if(hoveringOption==0 && button==1 && characterIndex>0 && mouseButtonDown) 
            {
                characterIndex--;
                mouseButtonDown=0;
            }*/

            renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)+100,350,SDL_FLIP_NONE,-1,alpha[1]);
            /*
            if(hoveringOption==1 && button==1 && characterIndex<6 && mouseButtonDown) 
            {
                characterIndex++;
                mouseButtonDown=0;
            }*/

            //Lower buttons
            renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)-100,400,SDL_FLIP_HORIZONTAL,-1,alpha[2]);
            /*
            if(hoveringOption==2 && button && weaponIndex>0 && mouseButtonDown)
            {
                weaponIndex--;
                mouseButtonDown=0;
            }
            */

            renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)+100,400,SDL_FLIP_NONE,-1,alpha[3]);
            /*
            if(hoveringOption==3 && button && weaponIndex<4 && mouseButtonDown)
            {
                weaponIndex++;
                mouseButtonDown=0;
            }
            */

            //Renders character name and weapon
            //renderText(m,characterName[characterIndex],color,-1,340);
            //renderText(m,characterWeapon[weaponIndex],color,-1,390);

            //printf("Index: %s\n", characterName[characterIndex]);
            SDL_RenderPresent(m->gRenderer);
        }
    }
    return CLOSEREQUSTED;
}

PRIVATE void renderImageEx(Menu m, char *path,int posX,int posY, int flip, int directionFrame, Uint8 alpha)
{
    SDL_Surface* s = IMG_Load(path);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m->gRenderer,s);
    SDL_FreeSurface(s);
    SDL_SetTextureAlphaMod(tex,alpha);

    SDL_Rect dest;
    static SDL_Rect sourceDest[] = {{0,0,32,32},
                                    {66,0,32,32},
                                    {0,64,32,32},
                                    {66,64,32,32},
                                    {0,96,32,32},
                                    {66,96,32,32},
                                    {0,32,32,32},
                                    {66,32,32,32}};

    SDL_QueryTexture(tex, NULL,NULL,&dest.w,&dest.h);
    
    //Defines the position of the images top-left corner
    if(posX==-1) dest.x=(WINDOW_WIDTH-dest.w)/2;
    else dest.x=posX;  
    
    dest.y=posY;
    if(directionFrame==-1)
        SDL_RenderCopyEx(m->gRenderer,tex,NULL,&dest,0,NULL,flip);
    else
        SDL_RenderCopyEx(m->gRenderer,tex,&sourceDest[directionFrame],&dest,0,NULL,flip);

    SDL_DestroyTexture(tex);
    return;  
}

PRIVATE void renderCharacterMenuUI(Menu m)
{
    static int i = 0;
    renderImage(m,"pickCharacter.png",-1,20,1);
    
    //The animation
    renderImageEx(m,"resources/Karaktarer/BOY/BOYbow.png",-1,200,SDL_FLIP_NONE,i,SDL_ALPHA_OPAQUE);
    if(i!=7)i++;
    else i=0;  
    
    //The arrows
    renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)+60,350,SDL_FLIP_NONE,-1,SDL_ALPHA_OPAQUE);
    renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)-70,350,SDL_FLIP_HORIZONTAL,-1,SDL_ALPHA_OPAQUE);
    renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)+60,400,SDL_FLIP_NONE,-1,SDL_ALPHA_OPAQUE);
    renderImageEx(m,PATH_TO_ARROW,((WINDOW_WIDTH-16)/2)-70,400,SDL_FLIP_HORIZONTAL,-1,SDL_ALPHA_OPAQUE);

    return;
}