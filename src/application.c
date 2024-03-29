#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "menu.h"
#include "endScreen.h"
#include "lobby.h"
#include "collision/collision.h"
#include "timers.h"
#include "collision/powers.h"

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

struct player{
    char playerPath[PATHLENGTH];
    char playerName[MAX_NAME];
};
typedef struct player Player;

struct lobby{
    SDL_Renderer *gRenderer;
    SDL_Event windowEvent;
    int numberOfPlayers;
    Player players[MAX_PLAYERS];
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
    SDL_Texture *mReloadDisplay = NULL;
    SDL_Texture *mBulletType = NULL;

    SDL_Texture *mPowers = NULL;
    SDL_Rect powersClips[1];
    SDL_Rect powersPosition;

    SDL_Texture *timeTexture = NULL;
    SDL_Rect timePos;
    Uint32 nextSecond, passedTime;
    int currentTime = 0;

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
    int playerInfoCounter = 0;
    // Background
    SDL_Texture *mTiles = NULL;
    SDL_Rect gTiles[16];
    
    PowerUps powers;
    powers = createPowerUps(220,220);
    
    Tile tiles[AMOUNT_TILES][AMOUNT_TILES];

	IPaddress srvadd;

    gameInfo->gRenderer = SDL_CreateRenderer(theApp->window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    
    //Menu
    Menu m = createMenu(gameInfo->gRenderer);
    if(menuApplication(m,1) == -1) return;

    initSoundEffects();
    initConnection(&gameInfo->tcp_sd, &srvadd, m); 
    //Lobby
    gameInfo->l = createLobby(gameInfo->gRenderer);
    pushLobbyPlayer(gameInfo->l, getPathToCharacter(m), getPlayerName(m), 0);
 
    pthread_t networkThread;

    //Setting current player image, name and weapon
    setSoldierFileName(gameInfo->soldiers[gameInfo->id], getPathToCharacter(m));
    setSoldierName(gameInfo->soldiers[gameInfo->id], getPlayerName(m));
    weaponChoiceHandler(gameInfo->soldiers[gameInfo->id]);

    // Send to lobby character
    char soldierImagePath[PATHLENGTH];
    char soldierName[MAX_NAME];
    int currentPlayers = 0;
    strcpy(soldierImagePath, getSoldierFileName(gameInfo->soldiers[gameInfo->id]));
    strcpy(soldierName, getSoldierName(gameInfo->soldiers[gameInfo->id]));
    SDLNet_TCP_Send(gameInfo->tcp_sd, soldierImagePath, PATHLENGTH+1);
    SDLNet_TCP_Send(gameInfo->tcp_sd, soldierName, MAX_NAME+1);

    pthread_create(&networkThread, NULL, handleNetwork, (void *)gameInfo);


    //Lobby starts here
    bool closeRequested = false;
    int oldAmountOfPlayersConnected = -1;
    SDL_Color colorWhite = {0xFF,0xFF,0xFF}; //White
    SDL_Color colors[] = {{0xFF,0x00,0x00},  //Red
                          {0x00,0xFF,0x00},  //Green
                          {0x00,0x00,0xFF},  //Blue
                          {0xFF,0x00,0xFF}}; //Pink
    int posX = 0, padding = 70, countdown = 30;
    int posY[4] = {275,300,300,275};

    while(!closeRequested)
    {
       if (gameInfo->amountOfPlayersConnected == MAX_PLAYERS)
       {
           for (int i = 0; i < MAX_PLAYERS; i++)
           {
               pushLobbyPlayer(gameInfo->l, gameInfo->playerLobbyInformation[i].soldierImagePath, gameInfo->playerLobbyInformation[i].soldierName, i);
           }
           
           closeRequested = true;
       }

        while(SDL_PollEvent(&gameInfo->l->windowEvent))
        {
            if(gameInfo->l->windowEvent.type == SDL_QUIT){
                closeRequested = true;
            }
        }
        if(gameInfo->l->players[0].playerPath[0] != '\0')
        {
            SDL_RenderClear(gameInfo->l->gRenderer);
            renderImage(gameInfo->l->gRenderer,"lobby.png",-1,150,1,255);

            for(int i = 0; i < gameInfo->l->numberOfPlayers; i++)
            {
                posX = (WINDOW_WIDTH/2)-(WINDOW_WIDTH/5)*2 + (WINDOW_WIDTH/5)*i;

                renderImageEx(gameInfo->l->gRenderer,gameInfo->l->players[i].playerPath,posX,300,SDL_FLIP_NONE,0,SDL_ALPHA_OPAQUE);
                renderCharacterText(gameInfo->l->gRenderer,gameInfo->l->players[i].playerName,colors[i],posX,posY[i]-50,24);
            }
            SDL_RenderPresent(gameInfo->l->gRenderer);
        }
        if(gameInfo->amountOfPlayersConnected == MAX_PLAYERS){
            gameInfo->gameState = 2;
            usleep(5000000);
            
        }
    }
    usleep(10000000);
    
    //Loading images for health, tiles and powerups
    loadHealthMedia(gameInfo->gRenderer, &mHealthBar, healthClips);
    loadTiles(gameInfo->gRenderer, &mTiles, gTiles);
    loadPowers(gameInfo->gRenderer, &mPowers, powersClips);

    //Loads time display texture, position, font etc. and sets value based on current time
    updateTimeDisplay(gameInfo->gRenderer, &timeTexture, &timePos, currentTime);

    Uint32 startTime = SDL_GetTicks()/1000;

    bool keep_window_open = true;

    //Drawing and event handling loop
    while(gameInfo->gameState == 2)
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
        manageFireRateAndAmmo(gameInfo->soldiers);
        bulletPlayerCollision(bullets, gameInfo->soldiers, &amountOfBullets, gameInfo->playerKills);
        bulletWallCollision(tiles, bullets, &amountOfBullets);
        powersPlayerCollision(gameInfo->soldiers, powers);
        
        renderPlayers(gameInfo->gRenderer, gameInfo->soldiers, gameInfo->id, gameInfo->mSoldier, gameInfo->gSpriteClips, tiles, mHealthBar, healthClips, healthBarPositions, gameInfo->mAmmoCounter, gameInfo->ammoClips, ammoPosition, gameInfo->mBulletType, gameInfo->mReloadDisplay, powersPosition, mPowers, powersClips, powers);
        bulletsRenderer(gameInfo->gRenderer, gameInfo->soldiers, bullets, gameInfo->bulletTexture, &amountOfBullets, &bulletsActive);
        
        //Display for Game Timer
        passedTime = SDL_GetTicks()/1000 - startTime;
        currentTime = 180 - passedTime;
        //Texture for game timer is loaded once every second to prevent lag
        if(passedTime >= nextSecond)
        {
            updateTimeDisplay(gameInfo->gRenderer, &timeTexture, &timePos, currentTime);
            nextSecond = passedTime + 1;
        }
        SDL_RenderCopy(gameInfo->gRenderer,timeTexture,NULL,&timePos);
        
        SDL_RenderPresent(gameInfo->gRenderer);
        timerUpdate(gameInfo->soldiers[gameInfo->id], powers);

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / ((float) SDL_GetPerformanceFrequency() * 1000.0f);
        SDL_Delay(floor(16.666f - elapsedMS));
    }

    for (int i = 0; i < MAX_PLAYERS; i++){
        setSoldierKills(gameInfo->soldiers[i], gameInfo->playerKills[i]);
    }
    
	//End Screen	
    EndScreen es = createEndScreen(gameInfo->gRenderer, gameInfo->soldiers);	
    if(endScreenApplication(es) == -1) return;

    pthread_join(networkThread, NULL);
    
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

    //Receiving from server total amount of connected players and their player image and name
    SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, &((GameInfo *)ptr)->amountOfPlayersConnected, sizeof(((GameInfo *)ptr)->amountOfPlayersConnected));
    SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, ((GameInfo *)ptr)->playerLobbyInformation, sizeof(((GameInfo *)ptr)->playerLobbyInformation));

    //Setting up player and their weapon
    setupPlayerAndWeapon(((GameInfo *)ptr));

    //Receiving current player info and setting them
    SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, connParams, sizeof(connParams));
    ((GameInfo *)ptr)->id = connParams[0];

    setReceivedValuesForCurrentPlayer(((GameInfo *)ptr), connParams);

    //Loading images for reload and ammo to ONLY the player that the user is using, to see distinction between the player and other players
    loadReloadMedia(((GameInfo *)ptr)->gRenderer, getSoldierWeapon(((GameInfo *)ptr)->soldiers[connParams[0]]), &((GameInfo *)ptr)->mReloadDisplay[connParams[0]]);
    loadAmmoMedia(((GameInfo *)ptr)->gRenderer, getSoldierWeapon(((GameInfo *)ptr)->soldiers[connParams[0]]), &((GameInfo *)ptr)->mAmmoCounter[connParams[0]], ((GameInfo *)ptr)->ammoClips[connParams[0]], &((GameInfo *)ptr)->mBulletType[connParams[0]]);
    
    while (((GameInfo *)ptr)->gameState == 2)
    {   
        //Set current player info and sending it to the server
        setCurrentPlayerInfo(((GameInfo *)ptr), &clientPlayersData, connParams[0]);

		if (SDLNet_TCP_Send(((GameInfo *)ptr)->tcp_sd, &clientPlayersData, sizeof(struct playersData)) < sizeof(struct playersData))
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

        //Receiving all players data and setting them
        SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, ((GameInfo *)ptr)->playersData, 4*sizeof(struct playersData));
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if(i != clientPlayersData.id){
                setSoldierId(((GameInfo *)ptr)->soldiers[i],((GameInfo *)ptr)->playersData[i].id);
                setSoldierPositionX(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].x);
                setSoldierPositionY(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].y);
                setSoldierFrame(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].frame);
                setSoldierShotFired(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].shotFired);
                setSoldierHealth(((GameInfo *)ptr)->soldiers[i], ((GameInfo *)ptr)->playersData[i].health);
            }
        }

        if((((GameInfo *)ptr)->id) == 0){
            SDLNet_TCP_Send(((GameInfo *)ptr)->tcp_sd, ((GameInfo *)ptr)->playerKills, sizeof(((GameInfo *)ptr)->playerKills));
        }else{
            SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, ((GameInfo *)ptr)->playerKills, sizeof(((GameInfo *)ptr)->playerKills));
        }

        SDLNet_TCP_Recv(((GameInfo *)ptr)->tcp_sd, &((GameInfo *)ptr)->gameState, sizeof(((GameInfo *)ptr)->gameState));

    }
}