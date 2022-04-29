#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "collision/collision.h"

#include "sounds/soundeffects.h"

#include "map/world.h"
#include "map/tile.h"

#include "player/soldier.h"
#include "player/bullet.h"
#include "player/weapon.h"

#include "network/initnetwork.h"
#include "network/clientmanager.h"

#include "handlers/playerhandler.h"
#include "handlers/bullethandler.h"
#include "handlers/eventhandler.h"

#include "draw/render.h"
#include "draw/media.h"

#include "application.h"
#include <unistd.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#define MAX_BULLETS 100
#define MAX_PLAYERS 4
#define AMOUNT_TILES 32
#define CONN_PARAMS_LENGTH 20

struct application{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    window_event;

};

PUBLIC Application createApplication(){
    Application s = malloc(sizeof(struct application));

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
    }

    SDL_Init(SDL_INIT_AUDIO);
    
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
    Soldier soldiers[MAX_PLAYERS];

    initPlayers(soldiers);
    

    SDL_Texture *mSoldier = NULL;
    SDL_Rect gSpriteClips[8];
    SDL_Rect playerPosition;
    
    int weaponSpeed;
    int maxRange;
    int oldX, oldY, soldierXPos, soldierYPos;

    int playerId = 0;
    int playerFrame = 3;

    Bullet b = NULL;
    SDL_Texture *bulletTexture = NULL;
    SDL_Rect bullet;
    SDL_Rect bulletPosition;

    Bullet bullets[MAX_BULLETS];
    
    int bulletFrame = 0;
    int bulletAngle = 0;

    int frame = 3;
    int amountOfBullets = 0;
    int shotFired = 0;
    int bulletsActive = 0;

    int checkPortalType;

     // Background
    SDL_Texture *mTiles = NULL;
    SDL_Rect gTiles[16];
   
    Tile tiles[AMOUNT_TILES][AMOUNT_TILES];

    UDPsocket sd;
	IPaddress srvadd;
	UDPpacket *p;
    UDPpacket *p2;

    initSoundEffects();
    initConnection(&sd, &srvadd, &p, &p2);  

    weaponSpeed = getWeaponSpeed(getSoldierWeapon(soldiers[playerId]));
    maxRange = getWeaponRange(getSoldierWeapon(soldiers[playerId]));

    gRenderer = SDL_CreateRenderer(theApp->window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);

    

    loadSoldierMedia(gRenderer, &mSoldier, gSpriteClips, soldiers[playerId]);
    loadBulletMedia(gRenderer, &bulletTexture);
    loadTiles(gRenderer, &mTiles, gTiles);
    
    bool keep_window_open = true;

    while(keep_window_open)
    {
        while(SDL_PollEvent(&theApp->window_event))
        {
            if(theApp->window_event.type == SDL_QUIT){
                keep_window_open = false;
                break;
            }else if( theApp->window_event.type == SDL_KEYUP){
                setSoldierShotFired(soldiers[playerId], 0);
            }
            movementInput(theApp->window_event, soldiers[playerId], &frame, &amountOfBullets);
        }  
        motion(soldiers[playerId], &frame);

        // Send and retrive information  
        clientPacketSender(soldiers, &soldierXPos, &soldierYPos, &oldX, &oldY, &playerId, bulletsActive, sd, srvadd, p);
        UDPPacketReceiver(soldiers, &playerId, sd, p2);

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        renderBackground(gRenderer, mTiles, gTiles, tiles);
        createAllCurrentBullets(soldiers, bullets, &amountOfBullets, &bulletsActive);
        

        bulletWallCollision(tiles, bullets, &amountOfBullets);

        renderPlayers(gRenderer, soldiers, mSoldier, gSpriteClips, tiles);

        bulletsRenderer(gRenderer, bullets, &bulletTexture, &amountOfBullets, weaponSpeed, &bulletsActive);
        SDL_RenderPresent(gRenderer);
    }
}

PUBLIC void destoryApplication(Application theApp){
    SDL_FreeSurface(theApp->window_surface);
    SDL_DestroyWindow(theApp->window);
    Mix_CloseAudio();
}
