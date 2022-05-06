#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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

#define CONN_PARAMS_LENGTH 20

struct application{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    window_event;

};

struct playersData{
    int id;
	int x;
	int y;
    int frame;
    int shotFired;
};
typedef struct playersData PlayersData;

struct gameInfo{
    PlayersData playersData[MAX_PLAYERS];
    Soldier soldiers[MAX_PLAYERS];
    TCPsocket tcp_sd;
    int id;
};
typedef struct gameInfo GameInfo;

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

    SDL_Renderer *gRenderer = NULL;

    //Create player and set start position
    GameInfo *gameInfo = (struct gameInfo *)malloc(sizeof(struct gameInfo));
    initPlayers(gameInfo->soldiers);
    

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

    int packetType = 0;

     // Background
    SDL_Texture *mTiles = NULL;
    SDL_Rect gTiles[16];
   


    Tile tiles[AMOUNT_TILES][AMOUNT_TILES];

    UDPsocket sd;
    
	IPaddress srvadd;
	UDPpacket *p;
    UDPpacket *p2;
    gRenderer = SDL_CreateRenderer(theApp->window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    Menu m = createMenu(gRenderer);
    if(menuApplication(m) == -1) return;
    initSoundEffects();
    initConnection(&sd, &gameInfo->tcp_sd, &srvadd, &p, &p2, m);  
    pthread_t networkThread;


    
    bool keep_window_open = true;

    pthread_create(&networkThread, NULL, handleNetwork, (void *)gameInfo);
    weaponSpeed = getWeaponSpeed(getSoldierWeapon(gameInfo->soldiers[gameInfo->id]));
    maxRange = getWeaponRange(getSoldierWeapon(gameInfo->soldiers[gameInfo->id]));
    loadSoldierMedia(gRenderer, &mSoldier, gSpriteClips, gameInfo->soldiers[gameInfo->id]);
    loadBulletMedia(gRenderer, &bulletTexture, getSoldierWeapon(gameInfo->soldiers[gameInfo->id]));
    loadTiles(gRenderer, &mTiles, gTiles);
    while(keep_window_open)
    {
        while(SDL_PollEvent(&theApp->window_event))
        {
            if(theApp->window_event.type == SDL_QUIT){
                keep_window_open = false;
                break;
            }else if( theApp->window_event.type == SDL_KEYUP){
                setSoldierShotFired(gameInfo->soldiers[gameInfo->id], 0);
            }
            movementInput(theApp->window_event, gameInfo->soldiers[gameInfo->id]);
        }  
        frame = getSoldierFrame(gameInfo->soldiers[gameInfo->id]);
        motion(gameInfo->soldiers[gameInfo->id], &frame);

        // Send and retrive information UDP
        //clientPacketSender(soldiers, &soldierXPos, &soldierYPos, &oldX, &oldY, &playerId, bulletsActive, sd, srvadd, p, &packetType);
        //UDPPacketReceiver(soldiers, &playerId, sd, p2, packetType);
   
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        renderBackground(gRenderer, mTiles, gTiles, tiles);

        createAllCurrentBullets(gameInfo->soldiers, bullets, &amountOfBullets, &bulletsActive);
        manageFireRateAndAmmo(gameInfo->soldiers);    //Manages firerate and reload for all soldiers
        bulletPlayerCollision(bullets, gameInfo->soldiers, &amountOfBullets);
        bulletWallCollision(tiles, bullets, &amountOfBullets);

        renderPlayers(gRenderer, gameInfo->soldiers, mSoldier, gSpriteClips, tiles);
        bulletsRenderer(gRenderer, bullets, &bulletTexture, &amountOfBullets, weaponSpeed, &bulletsActive);
        SDL_RenderPresent(gRenderer);
        timerUpdate(gameInfo->soldiers[gameInfo->id]);
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

    int connParams[6];
    SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, connParams, sizeof(connParams));
    setSoldierId(((GameInfo *)ptr)->soldiers[connParams[0]], connParams[0]);
    setSoldierFrame(((GameInfo *)ptr)->soldiers[connParams[0]], connParams[1]);
    setSoldierPositionX(((GameInfo *)ptr)->soldiers[connParams[0]], connParams[2]);
    setSoldierPositionY(((GameInfo *)ptr)->soldiers[connParams[0]], connParams[3]);
    setSoldierConnected(((GameInfo *)ptr)->soldiers[connParams[0]], connParams[4]);
    setSoldierPosition(((GameInfo *)ptr)->soldiers[connParams[0]],getSoldierPositionX(((GameInfo *)ptr)->soldiers[connParams[0]]), getSoldierPositionY(((GameInfo *)ptr)->soldiers[connParams[0]]), 32, 32);
    setSoldierFileName(((GameInfo *)ptr)->soldiers[connParams[0]], "resources/Karaktarer/BOY/BOYpistol.png");
    setSoldierShotFired(((GameInfo *)ptr)->soldiers[connParams[0]], connParams[5]);
    ((GameInfo *)ptr)->id = connParams[0];

    int gameOver = 0;
    
    while (!gameOver)
    {
        clientPlayersData.id = connParams[0];
        clientPlayersData.x = getSoldierPositionX(((GameInfo *)ptr)->soldiers[connParams[0]]);
        clientPlayersData.y = getSoldierPositionY(((GameInfo *)ptr)->soldiers[connParams[0]]);
        clientPlayersData.frame = getSoldierFrame(((GameInfo *)ptr)->soldiers[connParams[0]]);
        clientPlayersData.shotFired = getSoldierShotFired(((GameInfo *)ptr)->soldiers[connParams[0]]);

		if (SDLNet_TCP_Send(((GameInfo *)ptr)->tcp_sd, &clientPlayersData, sizeof(struct playersData)) < sizeof(struct playersData))
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

        SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, ((GameInfo *)ptr)->playersData, 4*sizeof(struct playersData));
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if(i != clientPlayersData.id){
                setSoldierId(((GameInfo *)ptr)->soldiers[i],((GameInfo *)ptr)->playersData[i].id);
                setSoldierPositionX(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].x);
                setSoldierPositionY(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].y);
                setSoldierFrame(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].frame);
                setSoldierShotFired(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].shotFired);
                usleep(1);
            }
        }
        usleep(1000);
    }
}