#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "menu.h"
#include "collision/collision.h"
#include "timers.h"

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
#include <pthread.h>

#define PUBLIC /* empty */
#define PRIVATE static

struct application{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    window_event;

};



PUBLIC void *handleNetwork(void *ptr);

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


    //Create player and set start position
    GameInfo *gameInfo = (struct gameInfo *)malloc(sizeof(struct gameInfo));
    initPlayers(gameInfo->soldiers);
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        gameInfo->playersData[i].x = -50;
        gameInfo->playersData[i].y = -50;
    }

    gameInfo->gRenderer = NULL;
    for (int i = 0; i < MAX_PLAYERS; i++){
        gameInfo->mSoldier[i] = NULL;
    }
    
    

    SDL_Rect playerPosition;
    
    SDL_Texture *mHealthBar = NULL;
    SDL_Rect healthClips[11];
    SDL_Rect healthBarPositions[MAX_PLAYERS];

    SDL_Texture *mAmmoCounter = NULL;
    SDL_Rect ammoClips[11];
    SDL_Rect ammoPosition;

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

    int packetType = 0;

     // Background
    SDL_Texture *mTiles = NULL;
    SDL_Rect gTiles[16];
   


    Tile tiles[AMOUNT_TILES][AMOUNT_TILES];

	IPaddress srvadd;

    gameInfo->gRenderer = SDL_CreateRenderer(theApp->window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    Menu m = createMenu(gameInfo->gRenderer);
    if(menuApplication(m) == -1) return;
    initSoundEffects();
    initConnection(&gameInfo->tcp_sd, &srvadd, m);  
    pthread_t networkThread;


    
    bool keep_window_open = true;
    pthread_create(&networkThread, NULL, handleNetwork, (void *)gameInfo);
    
    setSoldierFileName(gameInfo->soldiers[gameInfo->id], getPathToCharacter(m));
    weaponChoiceHandler(gameInfo->soldiers[gameInfo->id]);
   
   // Send to lobby character
   char soldierImagePath[PATHLENGTH];
   strcpy(soldierImagePath, getSoldierFileName(gameInfo->soldiers[gameInfo->id]));
   SDLNet_TCP_Send(gameInfo->tcp_sd, soldierImagePath, PATHLENGTH+1);

    

    loadHealthMedia(gameInfo->gRenderer, &mHealthBar, healthClips);
    loadAmmoMedia(gameInfo->gRenderer, &mAmmoCounter, ammoClips);
    loadTiles(gameInfo->gRenderer, &mTiles, gTiles);
    while(keep_window_open)
    {
        Uint64 start = SDL_GetPerformanceCounter();  

        while(SDL_PollEvent(&theApp->window_event))
        {
            if(theApp->window_event.type == SDL_QUIT){
                keep_window_open = false;
                break;
            }
            movementInput(theApp->window_event, gameInfo->soldiers[gameInfo->id]);
        }  
        frame = getSoldierFrame(gameInfo->soldiers[gameInfo->id]);
        motion(gameInfo->soldiers[gameInfo->id], &frame, &healthBarPositions[gameInfo->id], &ammoPosition);
        SDL_SetRenderDrawColor(gameInfo->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gameInfo->gRenderer);
        renderBackground(gameInfo->gRenderer, mTiles, gTiles, tiles);
        createAllCurrentBullets(gameInfo->soldiers, bullets, &amountOfBullets, &bulletsActive);
        manageFireRateAndAmmo(gameInfo->soldiers);    //Manages firerate and reload for all soldiers
        bulletPlayerCollision(bullets, gameInfo->soldiers, &amountOfBullets);
        bulletWallCollision(tiles, bullets, &amountOfBullets);

        renderPlayers(gameInfo->gRenderer, gameInfo->soldiers, gameInfo->id, gameInfo->mSoldier, gameInfo->gSpriteClips, tiles, mHealthBar, healthClips, healthBarPositions, mAmmoCounter, ammoClips, ammoPosition);
        bulletsRenderer(gameInfo->gRenderer, gameInfo->soldiers, bullets, gameInfo->bulletTexture, &amountOfBullets, &bulletsActive);
        SDL_RenderPresent(gameInfo->gRenderer);
        timerUpdate(gameInfo->soldiers[gameInfo->id]);

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / ((float) SDL_GetPerformanceFrequency() * 1000.0f);
        SDL_Delay(floor(16.666f - elapsedMS));
    }
    SDLNet_TCP_Close(gameInfo->tcp_sd);
}

PUBLIC void destoryApplication(Application theApp){
    SDL_FreeSurface(theApp->window_surface);
    SDL_DestroyWindow(theApp->window);
    //Mix_CloseAudio();
}

PUBLIC void *handleNetwork(void *ptr) {

    PlayersData clientPlayersData;
    int connParams[7];
    

    SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, ((GameInfo *)ptr)->soldierImagePaths, 4*(PATHLENGTH+1));
    SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, &((GameInfo *)ptr)->amountOfPlayersConnected, sizeof(((GameInfo *)ptr)->amountOfPlayersConnected));
    setupPlayerAndWeapon(((GameInfo *)ptr));

    SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, connParams, sizeof(connParams));
    setReceivedValuesForCurrentPlayer(((GameInfo *)ptr), connParams);
    int gameOver = 0;
    
    while (!gameOver)
    {   
        getCurrentPlayerInfo(((GameInfo *)ptr), &clientPlayersData, connParams[0]);

		if (SDLNet_TCP_Send(((GameInfo *)ptr)->tcp_sd, &clientPlayersData, sizeof(struct playersData)) < sizeof(struct playersData))
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

        SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, ((GameInfo *)ptr)->playersData, 4*sizeof(struct playersData));
        for (int i = 0; i < ((GameInfo *)ptr)->amountOfPlayersConnected; i++)
        {
            if(i != clientPlayersData.id){
                setSoldierId(((GameInfo *)ptr)->soldiers[i],((GameInfo *)ptr)->playersData[i].id);
                setSoldierPositionX(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].x);
                setSoldierPositionY(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].y);
                setSoldierFrame(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].frame);
                setSoldierShotFired(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].shotFired);
                setSoldierHealth(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].health);
                //setWeaponMagazine(getSoldierWeapon(((GameInfo *)ptr)->soldiers[i]), ((GameInfo *)ptr)->playersData[i].magazine);
            }
        }
    }
}