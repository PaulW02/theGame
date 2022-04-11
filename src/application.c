#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdbool.h>
#include "application.h"
#include "world.h"
#include "soldier.h"
#include "bullet.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

struct application{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    window_event;

};

PRIVATE bool init(SDL_Renderer **gRenderer);
PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[]);
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Bullet *bullet);
PRIVATE void update(Application theApp, double delta_time);
PRIVATE void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTile, SDL_Rect gTiles[]);
//PRIVATE void draw(Application theApp);
PRIVATE void shootBullet(SDL_Renderer *gRenderer, int frame);
PRIVATE int deleteBullet(int *counter, Bullet bullets[],int delete);
PRIVATE void checkPlayerOutOfBoundaries(Soldier s, SDL_Rect *playerPosition);
PRIVATE int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition);
PRIVATE int checkBulletAngle(int frame, SDL_RendererFlip *flip);


PUBLIC Application createApplication(){
    Application s = malloc(sizeof(struct application));
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
    }

    s->window= SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

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

    //Create player and set start position
    Soldier soldier = createSoldier(10,10);
    SDL_Texture *mSoldier = NULL;
    SDL_Rect gSpriteClips[8];
    SDL_Rect playerPosition;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    playerPosition.y = getSoldierPositionY(soldier);
    playerPosition.x = getSoldierPositionX(soldier);
    playerPosition.h = 24;
    playerPosition.w = 24;

    Bullet b = NULL;
    SDL_Texture *bulletTexture = NULL;
    SDL_Rect bullet;
    SDL_Rect bulletPosition;
    SDL_Surface* bulletSurface = NULL;
    Bullet bullets[10];
    int bulletFrame = 0;
    int bulletAngle = 0;
    SDL_RendererFlip bulletflip = SDL_FLIP_NONE;

    int frame = 3;
    int counter = 0;
    bool shotFired = true;

     // Background
    SDL_Texture *mTiles = NULL;
    SDL_Rect gTiles[16];
   
    

    gRenderer = SDL_CreateRenderer(theApp->window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);

    loadMedia(gRenderer, &mSoldier, gSpriteClips, &mTiles, gTiles);

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
                        playerPosition.y -= 2;
                        setSoldierPositionY(soldier, playerPosition.y);
                        flip = SDL_FLIP_NONE;
                        if(frame == 4)
                            frame = 5;
                        else
                            frame = 4;
                        break;
                    case SDLK_DOWN:
                        playerPosition.y += 2;
                        setSoldierPositionY(soldier, playerPosition.y);
                        flip = SDL_FLIP_NONE;
                        if(frame == 0)
                            frame = 1;
                        else
                            frame = 0;
                        break;
                    case SDLK_LEFT:
                        playerPosition.x -= 2;
                        setSoldierPositionX(soldier, playerPosition.x);
                        flip = SDL_FLIP_HORIZONTAL;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    case SDLK_RIGHT:
                        playerPosition.x += 2;
                        setSoldierPositionX(soldier, playerPosition.x);
                        flip = SDL_FLIP_NONE;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    case SDLK_SPACE:
                        shotFired = true;
                        Bullet b = createBullet(playerPosition.x, playerPosition.y, 5);
                        setBulletFrame(b, frame);
                        setBulletPositionX(b, playerPosition.x);
                        setBulletPositionY(b, playerPosition.y+14);
                        setBulletHeight(b, 5);
                        setBulletWidth(b, 10);
                        bulletAngle = checkBulletAngle(frame, &flip);
                        setBulletFlip(b,flip);
                        setBulletAngle(b,bulletAngle);
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
        renderBackground(gRenderer, mTiles, gTiles);
        checkPlayerOutOfBoundaries(soldier, &playerPosition);
        SDL_RenderCopyEx(gRenderer, mSoldier, &gSpriteClips[frame],&playerPosition , 0, NULL, flip);

        for (int i = 0; i < counter; i++)
        {
            bulletPosition = getBulletPositionSDL(bullets[i]);
            bulletFrame = getBulletFrame(bullets[i]);
            bullet = getBulletSDL(bullets[i]);
            bulletflip = getBulletFlip(bullets[i]);
            bulletAngle = getBulletAngle(bullets[i]);
            move(&bulletPosition, bulletFrame, bulletflip);
            SDL_RenderCopyEx(gRenderer, bulletTexture, &bullet,&bulletPosition, bulletAngle, NULL, bulletflip);
            if(checkBulletOutOfBoundaries(b, bulletPosition))
            {
                free(bullets[i]);
                counter = deleteBullet(&counter,bullets, i);
            }else{
                setBulletPositionX(bullets[i], bulletPosition.x);
                setBulletPositionY(bullets[i], bulletPosition.y);
            }
        }
        SDL_RenderPresent(gRenderer);
        update(theApp, 10.0/60.0);
    }
}

PRIVATE int checkBulletAngle(int frame, SDL_RendererFlip *flip)
{
    if(frame == 0 || frame == 1)
    {
        return 90;
    }else if( frame == 2 || frame == 3)
    {
        return 0;
    }else
    {   
        *flip = SDL_FLIP_HORIZONTAL;
        return 90;
    }
}

PRIVATE int deleteBullet(int *counter, Bullet bullets[],int delete)
{
    for (int i = delete; i < (*counter-1); i++){
        bullets[i] = bullets[i+1];
    }
    (*counter)--;
    return *counter;
}

PRIVATE void checkPlayerOutOfBoundaries(Soldier s, SDL_Rect *playerPosition)
{
    if (getSoldierPositionX(s) > WINDOW_WIDTH-16){
        playerPosition->x = WINDOW_WIDTH-16;
    }else if(getSoldierPositionX(s) < 0){
        playerPosition->x = 0;
    }
    
    if(getSoldierPositionY(s) > WINDOW_HEIGHT-16){
        playerPosition->y = WINDOW_HEIGHT-16;
    }else if(getSoldierPositionY(s) < 0){
        playerPosition->y = 0;
    }
}

PRIVATE int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition)
{
    if(bulletPosition.x == WINDOW_WIDTH || bulletPosition.y == WINDOW_HEIGHT || bulletPosition.x == -10 || bulletPosition.y == -10){
        return 1;
    }else{
        return 0;
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
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Bullet *bullet)
{
    SDL_Surface* bulletSurface = IMG_Load("resources/BULLETS.png");
    *bulletTexture = SDL_CreateTextureFromSurface(gRenderer, bulletSurface);

    setBulletSDLPos(*bullet, 0,0,10,5);
}

PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[])
{
    SDL_Surface* gSpacemanSurface = IMG_Load("resources/MALE.png");
    *mSpaceman = SDL_CreateTextureFromSurface(gRenderer, gSpacemanSurface);
    
     
    gSpriteClips[ 0 ].x =   0;
    gSpriteClips[ 0 ].y =   0;
    gSpriteClips[ 0 ].w =  25;
    gSpriteClips[ 0 ].h = 30;
    
    gSpriteClips[ 1 ].x =  26;
    gSpriteClips[ 1 ].y =   0;
    gSpriteClips[ 1 ].w =  25;
    gSpriteClips[ 1 ].h = 30;
    
    gSpriteClips[ 2 ].x = 51;
    gSpriteClips[ 2 ].y =   0;
    gSpriteClips[ 2 ].w =  23;
    gSpriteClips[ 2 ].h = 30;
    
    gSpriteClips[ 3 ].x = 76;
    gSpriteClips[ 3 ].y =   0;
    gSpriteClips[ 3 ].w =  21;
    gSpriteClips[ 3 ].h = 30;
    
    gSpriteClips[ 4 ].x = 105;
    gSpriteClips[ 4 ].y =   0;
    gSpriteClips[ 4 ].w =  25;
    gSpriteClips[ 4 ].h = 30;
    
    gSpriteClips[ 5 ].x = 130;
    gSpriteClips[ 5 ].y =   0;
    gSpriteClips[ 5 ].w =  25;
    gSpriteClips[ 5 ].h = 30;

    SDL_Surface* gTilesSurface = IMG_Load("resources/GrassAndTowersStraight.PNG");
    *mTiles = SDL_CreateTextureFromSurface(gRenderer, gTilesSurface);
    for (int i = 0; i < 16; i++) {
        gTiles[i].x = i*getTileWidth();
        gTiles[i].y = 0;
        gTiles[i].w = getTileWidth();
        gTiles[i].h = getTileHeight();
    }
}

PUBLIC void destoryApplication(Application theApp){
    SDL_FreeSurface(theApp->window_surface);
    SDL_DestroyWindow(theApp->window);
}

PRIVATE void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]){
    
    SDL_Rect possition;
    possition.y = 0;
    possition.x = 0;
    possition.h = getTileHeight();
    possition.w = getTileWidth();
    
    for (int i = 0; i<getTileColumns(); i++) {
        for (int j = 0; j<getTileRows(); j++) {
            possition.y = i*getTileHeight();
            possition.x = j*getTileWidth();
            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i,j)],&possition , 0, NULL, SDL_FLIP_NONE);
        }
    }
    
}

