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

//Window values
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define SCROLL_SPEED 5

//Menu options
#define TITLECARD 1
#define MAINMENU 2
#define CLOSEREQUSTED -1
#define HOWTOPLAYMENU 3
#define ONLINEMENU 4
#define PICKCHARACTERMENU 5

//Size defines
#define IPWORDLENGTH 16
#define NUMBEROFWEAPONS 5
#define NUMBEROFCHARACTERS 7
#define PATHLENGTH 64
#define WORDLENGTH 10

struct menu{
    SDL_Renderer *gRenderer;
    SDL_Event event;
    char ipAddress[IPWORDLENGTH];
    char pathToCharacter[PATHLENGTH];
    char gameModeType; //2v2,
    int character;
    int weapon; 
    char name[WORDLENGTH];
};

//Function declarations
PUBLIC int menuApplication(Menu m, int menuIndex);
PUBLIC void destroyMenu(Menu m);
PUBLIC void renderImage(SDL_Renderer *gRenderer, char *imageName, int posX, int posY, int scaleModifier,Uint8 alpha);
PRIVATE int startPage(Menu m);
PRIVATE int mainMenu(Menu m);
PRIVATE bool checkImageBoxForCursor(char *imageName, int imagePosX, int imagePosY, int *button);
PRIVATE int howToPlayMenu(Menu m);
PRIVATE int onlineMenuConfig(Menu m);
PUBLIC void renderText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size);
PRIVATE int pickCharacterMenu(Menu m);
PUBLIC void renderImageEx(SDL_Renderer *gRenderer, char *path,int posX,int posY, int flip, int directionFrame, Uint8 alpha);
PRIVATE void renderRect(Menu m, int x, int y, int w, int h);
PRIVATE void renderCharacter(Menu m, Uint32 sprite, int characterIndex, int weaponIndex);





PUBLIC Menu createMenu(SDL_Renderer *gRenderer)
{
    Menu m = malloc(sizeof(struct menu));
    SDL_Event windowEvent;

    //Initilizing TTF
    if(TTF_Init()<0) printf("Error with init!\n");
   
    //Declaring the values of the struct
    m->gRenderer = gRenderer;
    m->gameModeType = '\0';
    m->event = windowEvent;
    m->character=0;
    m->weapon=0;
    strcpy(m->ipAddress,"\0");
    strcpy(m->pathToCharacter,"\0");

    return m;
}

//menuIndex -Indicates which of the pages of the whole menu that you decide to start on.
//          menuIndex values:
//          - 0 Returns instantly
//          - 1 Title card
//          - 2 Main menu
//          - 3 How to play
//          - 4 Enter ip menu
//          - 5 Pick a character menu
PUBLIC int menuApplication(Menu m, int menuIndex)
{      
    int result = 1;
    result = menuIndex;

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

PUBLIC void destroyMenu(Menu m)
{
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

PUBLIC int getCharacter(Menu m)
{
    return m->character;
}

PUBLIC int getWeapon(Menu m)
{
    return m->weapon;
}

PUBLIC char* getPathToCharacter(Menu m)
{
    return m->pathToCharacter;
}

PUBLIC char* getPlayerName(Menu m)
{
    return m->name;
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
            renderImage(m->gRenderer,"pressAnyButton.png",-1,300,2,SDL_ALPHA_OPAQUE);
            SDL_RenderPresent(m->gRenderer);
            pressAnyButtonVisible=true;
        }
    }
    return CLOSEREQUSTED;

}

/*Can only render from the 'resource/menu/' path, will need
  modification if it's to be used elsewear */
PUBLIC void renderImage(SDL_Renderer *gRenderer, char *imageName,int posX,int posY, int scaleModifier, Uint8 alpha)
{
    char path[42] = "resources/menu/";
    strcat(path,imageName);
    SDL_Surface* s = IMG_Load(path);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(gRenderer,s);
    SDL_FreeSurface(s);
    SDL_SetTextureAlphaMod(tex,alpha);

    SDL_Rect dest;

    SDL_QueryTexture(tex, NULL,NULL,&dest.w,&dest.h);
    
    //Scales the image up or down
    dest.w/=scaleModifier;
    dest.h/=scaleModifier;

    //Defines the position of the images top-left corner
    if(posX==-1) dest.x=(WINDOW_WIDTH-dest.w)/2;
    else dest.x=posX;  
    
    dest.y=posY;

    SDL_RenderCopy(gRenderer,tex,NULL,&dest);
    SDL_DestroyTexture(tex);
    return;
}

PRIVATE int mainMenu(Menu m)
{
    //Typsnitt: Showcard Gothic, Storlek 48

    bool windowCloseRequested = false, userInterfaceAppeard = false, hovering[3] = {false};
    int menuChoice = 4, button;
    Uint8 alpha[3] = {255,255,255};

    
    while(!windowCloseRequested)
    {
        while(SDL_PollEvent(&m->event))
        {
            if(m->event.type==SDL_QUIT)
                windowCloseRequested=true;
        }
                
        //Checks which option the mouse is hovering on.
        if(menuChoice<3) alpha[menuChoice]=255;
        if(hovering[0]=checkImageBoxForCursor("onlineOption.png",-1,225,&button)) menuChoice=0;
        if(hovering[1]=checkImageBoxForCursor("howToPlay.png",-1,325,&button)) menuChoice=1;
        if(hovering[2]=checkImageBoxForCursor("co_op.png",-1,425,&button)) menuChoice=2;
        if(!hovering[0] && !hovering[1] && !hovering[2]) alpha[menuChoice]=255;
        else alpha[menuChoice]=144;
        
        if(button)
        {
            switch (menuChoice)
            {
            case 0:
                m->gameModeType='O';
                return ONLINEMENU;
                break;
            
            case 1:
                return HOWTOPLAYMENU;
                break;
            
            case 2:
                m->gameModeType='C';
                return 0;
                break;

            default:
                break;
            }
        }

        SDL_RenderClear(m->gRenderer);

        renderImage(m->gRenderer,"theGame.png",-1,20,1,SDL_ALPHA_OPAQUE);
        
        renderImage(m->gRenderer,"onlineOption.png",-1,225,1,alpha[0]);
        renderImage(m->gRenderer,"howToPlay.png",-1,325,1,alpha[1]);
        renderImage(m->gRenderer,"co_op.png",-1,425,1,alpha[2]);

        SDL_RenderPresent(m->gRenderer);
    }
    return CLOSEREQUSTED;
}

/*Checks if the cursor is within the image paramater*/
/*Can only check from the 'resource/menu/' path, will need
  modification if it's to be used elsewear */
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

PRIVATE int howToPlayMenu(Menu m)
{
    bool windowCloseRequested = false, userInterfaceAppeard = false,firstLoop=true;
    SDL_Color color = {0xFF,0xFF,0xFF}; //White
    char c[2]={'\0'};
    int xDisplacement=0;
    int yDisplacement=0;


    FILE *fp = fopen("resources/menu/howToPlay.txt","r");
    if(fp==NULL)
    {
        printf("Can't open file!\n");
    }

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
                    {
                        fclose(fp);
                        return MAINMENU;
                    }
                    break;
                
                default:
                    break;
                }
        }
        if(firstLoop)
        {
            SDL_RenderClear(m->gRenderer);
            renderImage(m->gRenderer,"howToPlay.png",-1,100,1,SDL_ALPHA_OPAQUE);
            firstLoop=false;
        }
        if(!userInterfaceAppeard)
        {            
            //renderRect(m,(WINDOW_WIDTH-400)/2,200,400,200);
            if((c[0]=getc(fp)) != EOF)
            {
                if(c[0]=='\n')
                {
                    yDisplacement+=25;
                    xDisplacement=0;
                }
                else
                {
                    renderText(m->gRenderer,c,color,(WINDOW_WIDTH-400+xDisplacement)/2,200+yDisplacement,18);
                    xDisplacement+=25;
                    if(xDisplacement>650 && c[0]==' ')
                    {
                        xDisplacement=0;
                        yDisplacement+=25;
                    }
                    //printf("%c",c[0]);
                    SDL_RenderPresent(m->gRenderer);
                }
            }            
            SDL_RenderPresent(m->gRenderer);
        }

    }
    fclose(fp);
    return CLOSEREQUSTED;
}

PRIVATE int onlineMenuConfig(Menu m)
{
    bool windowCloseRequested = false, hovering = false;
    char ipPlaceholder[16] = "\0";
    SDL_Color color = {0xFF,0xFF,0xFF}; //White
    Uint8 alpha = 255;
    int button;

    
    SDL_StartTextInput();

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
                            }
                            break;
                        
                        case SDL_SCANCODE_RETURN:
                            strcpy(m->ipAddress,ipPlaceholder);
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
                            //printf("%c",m->event.text.text[0]);
                        }
                    }
                    break;
                
                default:
                    break;
            }
        }
        SDL_RenderClear(m->gRenderer);

        //Render title   
        renderImage(m->gRenderer,"onlineOption.png",-1,100,1,SDL_ALPHA_OPAQUE);

        //Renders a centered rectangle with white border with text in it
        renderRect(m,(WINDOW_WIDTH-300)/2,250,300,50);
        renderText(m->gRenderer,ipPlaceholder,color,(WINDOW_WIDTH-300)/2,250,30);

        //Renders "Continue" and changes ALPHA
        if(hovering=checkImageBoxForCursor("continue.png",-1,330,&button)) alpha=144;
        else alpha=255;
        renderImage(m->gRenderer,"continue.png",-1,330,1,alpha);

        if(hovering && button)
        {
            strcpy(m->ipAddress,ipPlaceholder);
            SDL_StopTextInput();
            return PICKCHARACTERMENU;  
        }

        SDL_RenderPresent(m->gRenderer);        

    }
    SDL_StopTextInput();
    return CLOSEREQUSTED;
}

/*Renders a rectangle*/
PRIVATE void renderRect(Menu m, int x, int y, int w, int h)
{
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderFillRect(m->gRenderer,&rect);
    SDL_SetRenderDrawColor(m->gRenderer,0xFF,0xFF,0xFF,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(m->gRenderer,&rect);
    SDL_SetRenderDrawColor(m->gRenderer,0x00,0x00,0x00,SDL_ALPHA_OPAQUE);
    return;
}

/*Renders the text you put in as 'char *textToRender'
  Color:      The color, could be ex 'SDL_Color color = {0xFF,0xFF,0xFF}' for white
  X and Y:    The upper left most corner
  Size:       The size of the text*/
PUBLIC void renderText(SDL_Renderer *gRenderer, char *textToRender, SDL_Color color, int x, int y, int size)
{        
    TTF_Font* font = TTF_OpenFont("resources/fonts/8bitOperatorPlus-Regular.ttf",size);
    SDL_Surface* text = TTF_RenderText_Solid(font,textToRender,color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer,text);
 
    SDL_Rect rect;
    SDL_QueryTexture(textTexture,NULL,NULL,&rect.w,&rect.h);

    if(x==-1) rect.x=(WINDOW_WIDTH-rect.w)/2;
    else rect.x=x;
    rect.y=y;

    SDL_RenderCopy(gRenderer,textTexture,NULL,&rect);
            
    //Cleans up resources
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);

    return;
}

PRIVATE int pickCharacterMenu(Menu m)
{
    bool windowCloseRequested = false, spin = true;
    int i = 0, button, option = 5, mouseButtonDown=0, characterIndex=0, weaponIndex=0;
    Uint32 ticks, seconds, sprite;
    Uint8 alpha[5] = {255,255,255,255,255};
    SDL_Color color = {0xFF,0xFF,0xFF}; //White
    
    bool hovering[5]={false};

    //Names & Weapons
    PRIVATE char characters[7][10] = {"Boy","Female","Ghost","Male","Pumpkin","Skeleton","Wizard"};
    PRIVATE char weapons[5][11] = {"Bow","Pistol","Blue rod", "Red rod", "Spear"};
    char namePlaceholder[WORDLENGTH] = "\0";

    SDL_StartTextInput();

    while(!windowCloseRequested)
    {

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
                            SDL_StopTextInput();
                            return ONLINEMENU;
                            break;
                        
                        case SDL_SCANCODE_BACKSPACE:
                            if(strlen(namePlaceholder)!=0)
                            {
                                namePlaceholder[strlen(namePlaceholder)-1]='\0';
                            }
                            break;
                        
                        case SDL_SCANCODE_RETURN:
                            m->character=characterIndex;
                            m->weapon=weaponIndex;
                            strcpy(m->name,namePlaceholder);
                            SDL_StopTextInput();
                            return 0;
                            break;

                        default:
                            break;
                    }
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    mouseButtonDown=1;
                    break;

                case SDL_TEXTINPUT:
                    if(strlen(namePlaceholder)<WORDLENGTH-2)
                    {
                        strncat(namePlaceholder,m->event.text.text,1);
                    }
                    break;

                default:
                    break;
            }
        }
        if(spin)
        {
            SDL_RenderClear(m->gRenderer);

            //Title
            renderImage(m->gRenderer,"pickCharacter.png",-1,20,1,SDL_ALPHA_OPAQUE);
            
            //Name input
            renderRect(m,(WINDOW_WIDTH-275)/2,125,280,35);
            renderText(m->gRenderer,namePlaceholder,color,-1,125,24);
            
            //Animation
            renderCharacter(m,sprite, characterIndex,weaponIndex);

            //Checks what the user is hovering and highlights it
            if(option<5) alpha[option]=255;
            if(hovering[0]=checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)-100,350,&button)) option=0;
            if(hovering[1]=checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)+100,350,&button)) option=1;
            if(hovering[2]=checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)-100,400,&button)) option=2;
            if(hovering[3]=checkImageBoxForCursor("arrow.png",((WINDOW_WIDTH-16)/2)+100,400,&button)) option=3;
            if(hovering[4]=checkImageBoxForCursor("continue.png",-1,450,&button)) option=4;
            if(!hovering[0] && !hovering[1] && !hovering[2] && !hovering[3] && !hovering[4]) alpha[option]=255;
            else alpha[option]=144;

            //Arrow options
            renderImageEx(m->gRenderer,"resources/menu/arrow.png",((WINDOW_WIDTH-16)/2)-100,350,SDL_FLIP_HORIZONTAL,-1, alpha[0]);
            if(button==1 && hovering[0] && mouseButtonDown && characterIndex>0)
            {
                characterIndex--;
                mouseButtonDown=0;
            }
            renderImageEx(m->gRenderer,"resources/menu/arrow.png",((WINDOW_WIDTH-16)/2)+100,350,SDL_FLIP_NONE,-1,alpha[1]);
            if(button==1 && hovering[1] && mouseButtonDown && characterIndex<NUMBEROFCHARACTERS-1)
            {
                characterIndex++;
                mouseButtonDown=0;
            }
            renderImageEx(m->gRenderer,"resources/menu/arrow.png",((WINDOW_WIDTH-16)/2)-100,400,SDL_FLIP_HORIZONTAL,-1,alpha[2]);
            if(button==1 && hovering[2] && mouseButtonDown && weaponIndex>0)
            {
                weaponIndex--;
                mouseButtonDown=0;
            }
            renderImageEx(m->gRenderer,"resources/menu/arrow.png",((WINDOW_WIDTH-16)/2)+100,400,SDL_FLIP_NONE,-1,alpha[3]);
            if(button==1 && hovering[3] && mouseButtonDown && weaponIndex<NUMBEROFWEAPONS-1)
            {
                weaponIndex++;
                mouseButtonDown=0;
            }
            renderImage(m->gRenderer,"continue.png",-1,450,1,alpha[4]);
            if(button==1 && hovering[4] && mouseButtonDown)
            {
                m->character=characterIndex;
                m->weapon=weaponIndex;
                strcpy(m->name,namePlaceholder);
                mouseButtonDown=0;
                SDL_StopTextInput();
                return 0;
            }

            renderText(m->gRenderer,characters[characterIndex],color,-1,340,30);
            renderText(m->gRenderer,weapons[weaponIndex],color,-1,390,30);

            SDL_RenderPresent(m->gRenderer);
        }
    }
    SDL_StopTextInput();
    return CLOSEREQUSTED;
}

PUBLIC void renderImageEx(SDL_Renderer *gRenderer, char *path,int posX,int posY, int flip, int directionFrame, Uint8 alpha)
{
    SDL_Surface* s = IMG_Load(path);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(gRenderer,s);
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
        SDL_RenderCopyEx(gRenderer,tex,NULL,&dest,0,NULL,flip);
    else
        SDL_RenderCopyEx(gRenderer,tex,&sourceDest[directionFrame],&dest,0,NULL,flip);

    SDL_DestroyTexture(tex);
    return;  
}

PRIVATE void renderCharacter(Menu m, Uint32 sprite, int characterIndex, int weaponIndex)
{
    char path[64] = "resources/Karaktarer/";
    static char characterPaths[7][13] = {"BOY","FEMALE","GHOST","MALE","PUMPKIN","SKELETON","WIZARD"};
    static char weaponPaths[5][12] = {"bow.png","pistol.png","rodBLUE.png","rodRED.png","spear.png"};

    strcat(path,characterPaths[characterIndex]);
    strcat(path,"/");
    strcat(path,characterPaths[characterIndex]);
    strcat(path,weaponPaths[weaponIndex]);

    renderImageEx(m->gRenderer,path,-1,175,SDL_FLIP_NONE,sprite,SDL_ALPHA_OPAQUE);
    strcpy(m->pathToCharacter,path);
    return;
}