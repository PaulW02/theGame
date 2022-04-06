#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdbool.h>
#include "application.h"
#include "soldier.h"
#include "bullet.h"

#define PUBLIC /* empty */
#define PRIVATE static

struct application{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    window_event;

};

PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[]);
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Bullet *bullet);
PRIVATE void update(Application theApp, double delta_time);
//PRIVATE void draw(Application theApp);
PRIVATE void shootBullet(SDL_Renderer *gRenderer, int frame);


PUBLIC Application createApplication(){
    Application s = malloc(sizeof(struct application));
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
    }

    s->window= SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 480, SDL_WINDOW_SHOWN);

    if(!s->window)
    {
        printf("Failed to create window\n");
    }

    


   s->window_surface = SDL_GetWindowSurface(s->window);

    if(!s->window_surface)
    {
        printf("Failed to get the surface from the window\n"); 
    }
    
    

    return s;

}

PUBLIC void applicationUpdate(Application theApp){

    SDL_Renderer *gRenderer = NULL;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    Bullet b = NULL;
    SDL_Texture *bulletTexture = NULL;
    SDL_Rect bullet;
    SDL_Rect bulletPossition;
    SDL_Surface* bulletSurface = NULL;
    Bullet bullets[10];
    int bulletFrame = 0;
    SDL_RendererFlip bulletflip = SDL_FLIP_NONE;


    Soldier soldier = createSoldier(10,10);
    
    SDL_Texture *mSoldier = NULL;
    SDL_Rect gSpriteClips[8];
    SDL_Rect possition;
    possition.y = getSoldierPositionY(soldier);
    possition.x = getSoldierPositionX(soldier);
    possition.h = 16;
    possition.w = 16;

    SDL_Rect possition1;
    possition1.y = 10;
    possition1.x = 10;
    possition1.h = 10;
    possition1.w = 10;


    bulletPossition.y = possition.y;
    bulletPossition.x = possition.x;
    bulletPossition.h = possition.h;
    bulletPossition.w = possition.w;

    bullet.y = 100;
    bullet.x = possition.x;
    bullet.h = possition.h;
    bullet.w = possition.w;
    int frame = 6;
    int counter = 0;
    bool shotFired = false;


    
   gRenderer = SDL_CreateRenderer(theApp->window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);

    loadMedia(gRenderer, &mSoldier, gSpriteClips);

    bool keep_window_open = true;
    while(keep_window_open)
    {
        while(SDL_PollEvent(&theApp->window_event) > 0)
        {
            if(theApp->window_event.type == SDL_QUIT)
            {
                    keep_window_open = false;
                    break;
            }
            else if( theApp->window_event.type == SDL_KEYDOWN ){
                //Select surfaces based on key press
                switch(theApp->window_event.key.keysym.sym )
                {
                    case SDLK_UP:
                        // should be in game logic
                        possition.y -= 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 4)
                            frame = 5;
                        else
                            frame = 4;
                        break;
                    case SDLK_DOWN:
                        possition.y += 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 0)
                            frame = 1;
                        else
                            frame = 0;
                        break;
                    case SDLK_LEFT:
                        possition.x -= 2;
                        flip = SDL_FLIP_HORIZONTAL;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    case SDLK_RIGHT:
                        possition.x += 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    case SDLK_SPACE:
                        shotFired = true;
                        Bullet b = createBullet(possition.x, possition.y, 5);
                        setBulletFrame(b, frame);
                        setBulletPositionX(b, possition.x);
                        setBulletPositionY(b, possition.y);
                        setBulletHeight(b, 10);
                        setBulletWidth(b, 10);
                        setBulletFlip(b,flip);
                        bullets[counter] = b;
                        counter++;
                        loadBulletMedia(gRenderer, &bulletTexture, &b);
                    default:
                        
                        break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        SDL_RenderCopyEx(gRenderer, mSoldier, &gSpriteClips[frame],&possition , 0, NULL, flip);
        if(shotFired){
            for (int i = 0; i < counter; i++)
            {
                bulletPossition = getBulletPositionSDL(bullets[i]);
                bulletFrame = getBulletFrame(bullets[i]);
                bullet = getBulletSDL(bullets[i]);
                bulletflip = getBulletFlip(bullets[i]);
                //printf("%d %d %d %d ", bullet.x, bullet.y, bullet.h, bullet.w);
                move(&bulletPossition, bulletFrame, bulletflip);
                SDL_RenderCopyEx(gRenderer, bulletTexture, &bullet,&bulletPossition, 0, NULL, bulletflip);
                setBulletPositionX(bullets[i], bulletPossition.x);
                setBulletPositionY(bullets[i], bulletPossition.y);
            }
            
        }
        SDL_RenderPresent(gRenderer);
        update(theApp, 10.0/60.0);
    }
}



PRIVATE void update(Application theApp, double delta_time){
    //SDL_UpdateWindowSurface(theApp->window);
}

/*PRIVATE void draw(Application theApp)
{
    SDL_FillRect(theApp->window_surface, NULL, SDL_MapRGB(theApp->window_surface->format, 0, 0, 0));
    SDL_UpdateWindowSurface(theApp->window);
}
*/
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Bullet *bullet){
    SDL_Surface* bulletSurface = IMG_Load("resources/ALIEN.PNG");
    *bulletTexture = SDL_CreateTextureFromSurface(gRenderer, bulletSurface);

    setBulletSDLPos(*bullet, 0,0,10,10);
    
}

PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[]){
    SDL_Surface* gSpacemanSurface = IMG_Load("resources/SPACEMAN.PNG");
    *mSpaceman = SDL_CreateTextureFromSurface(gRenderer, gSpacemanSurface);
  
    
    gSpriteClips[ 0 ].x =   0;
    gSpriteClips[ 0 ].y =   0;
    gSpriteClips[ 0 ].w =  16;
    gSpriteClips[ 0 ].h = 16;
    
    gSpriteClips[ 1 ].x =  16;
    gSpriteClips[ 1 ].y =   0;
    gSpriteClips[ 1 ].w =  16;
    gSpriteClips[ 1 ].h = 16;
    
    gSpriteClips[ 2 ].x = 32;
    gSpriteClips[ 2 ].y =   0;
    gSpriteClips[ 2 ].w =  16;
    gSpriteClips[ 2 ].h = 16;
    
    gSpriteClips[ 3 ].x = 48;
    gSpriteClips[ 3 ].y =   0;
    gSpriteClips[ 3 ].w =  16;
    gSpriteClips[ 3 ].h = 16;
    
    gSpriteClips[ 4 ].x = 64;
    gSpriteClips[ 4 ].y =   0;
    gSpriteClips[ 4 ].w =  16;
    gSpriteClips[ 4 ].h = 16;
    
    gSpriteClips[ 5 ].x = 80;
    gSpriteClips[ 5 ].y =   0;
    gSpriteClips[ 5 ].w =  16;
    gSpriteClips[ 5 ].h = 16;
    
    gSpriteClips[ 6 ].x = 96;
    gSpriteClips[ 6 ].y =   0;
    gSpriteClips[ 6 ].w =  16;
    gSpriteClips[ 6 ].h = 16;
    
    gSpriteClips[ 7 ].x = 112;
    gSpriteClips[ 7 ].y =   0;
    gSpriteClips[ 7 ].w =  16;
    gSpriteClips[ 7 ].h = 16;
}

PRIVATE void shootBullet(SDL_Renderer *gRenderer, int frame)
{

}

PUBLIC void destoryApplication(Application theApp){
    SDL_FreeSurface(theApp->window_surface);
    SDL_DestroyWindow(theApp->window);
}
