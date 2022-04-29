#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_net.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "application.h"
#include "world.h"
#include "soldier.h"
#include "bullet.h"
#include "weapon.h"
#include "tile.h"
#include <unistd.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#define MAX_BULLETS 10

struct application{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    window_event;

};

struct data {
    int playerX;
    int playerY;
    int frame;
    SDL_RendererFlip flip;

    SDL_Rect bulletPosition[MAX_BULLETS];
    SDL_Rect bulletSDL[MAX_BULLETS];
    int bulletFrame[MAX_BULLETS];
    SDL_RendererFlip bulletFlip[MAX_BULLETS];
    int bulletAngle[MAX_BULLETS];
    int amountOfBullets;
};
typedef struct data Data;

PRIVATE bool init(SDL_Renderer **gRenderer);
PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[], Soldier s, SDL_Texture **mHealthBar, SDL_Rect gSpriteClipsHealth[]);
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Bullet *bullet);
PRIVATE void update(Application theApp, double delta_time);
PRIVATE void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[32][32]);
//PRIVATE void draw(Application theApp);
PRIVATE void shootBullet(SDL_Renderer *gRenderer, int frame);
PRIVATE int deleteBullet(int *amountOfBullets, Bullet bullets[],int delete);
PRIVATE void checkPlayerOutOfBoundaries(Soldier s, SDL_Rect *playerPosition);
PRIVATE int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition);
PRIVATE int checkBulletRangeMax(Bullet b, SDL_Rect bulletPosition, int maxRange, SDL_Rect playerPosition);
PRIVATE int checkBulletAngle(int frame, SDL_RendererFlip *flip);
PRIVATE void weaponChoiceHandler(Soldier soldier);
PRIVATE void collisionCheck(Soldier s, SDL_Rect *playerPosition, Tile tiles[32][32]);
PRIVATE void stepBack(Soldier s, SDL_Rect *playerPosition, int frame, int flip);
PRIVATE bool soldierWallCollision(Tile tiles[32][32], Soldier s, SDL_Rect *playerPosition, int frame, int flip, SDL_Rect *healthBarPosition);
PRIVATE void bulletWallCollision(Tile tiles[32][32], Bullet bullets[], int *counter);
PRIVATE void teleportThingy(Soldier s, Tile tiles[32][32], int i, int j, SDL_Rect *playerPosition, SDL_Rect *healthBarPosition);


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
    Soldier soldier1 = createSoldier(20,10);
    SDL_Texture *mSoldier = NULL;
    SDL_Texture *mSoldier1 = NULL;
    SDL_Texture *mHealthBar = NULL;
    SDL_Rect gSpriteClips[8];
    SDL_Rect gSpriteClipsHealth[11];
    SDL_Rect playerPosition;
    SDL_Rect healthBarPosition; // healthbar positionn above player
    SDL_Rect playerPosition1;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    playerPosition.y = getSoldierPositionY(soldier);
    playerPosition.x = getSoldierPositionX(soldier);
    playerPosition.h = 32;
    playerPosition.w = 32;
    healthBarPosition.y = getSoldierPositionY(soldier) - 12;
    healthBarPosition.x = getSoldierPositionX(soldier) - 4;
    healthBarPosition.h = 8;           // kolla här också!
    healthBarPosition.w = 36;
    
    int weaponSpeed;
    int maxRange;
    int oldX, oldY, soldierXPos, soldierYPos;

    //Second player data
    playerPosition1.y = getSoldierPositionY(soldier1);
    playerPosition1.x = getSoldierPositionX(soldier1);
    playerPosition1.h = 32;
    playerPosition1.w = 32;

    int secondFrame = 3;
    SDL_RendererFlip secondFlip = SDL_FLIP_NONE;

    Bullet b = NULL;
    SDL_Texture *bulletTexture = NULL;
    SDL_Texture *recvBulletTexture = NULL;
    
    SDL_Rect bullet;
    
    SDL_Rect bulletPosition;
    SDL_Rect recvBulletPosition;

    int oldBulletXPos, oldBulletYPos, bulletXPos, bulletYPos;

    Bullet bullets[MAX_BULLETS];
    Bullet recvBullets[MAX_BULLETS];
    
    int bulletFrame = 0;
    int bulletAngle = 0;
    int recvBulletAngle = 0;

    SDL_RendererFlip bulletFlip = SDL_FLIP_NONE;
    SDL_RendererFlip recvBulletFlip = SDL_FLIP_NONE;

    int frame = 3;
    int health = 100;
    int healthStatus=0;
    int amountOfBullets = 0;
    int recvAmountOfBullets = 0;
    bool shotFired = true;

    int playerXPos, playerYPos, playerFrame, bulletXPosToSend, bulletYPosToSend;
    SDL_RendererFlip playerFlip; 

     // Background
    SDL_Texture *mTiles = NULL;
    SDL_Rect gTiles[16];
   
    setSoldierFileName(soldier,"resources/Karaktarer/BOY/BOYpistol.png");
    weaponChoiceHandler(soldier);
    weaponSpeed = getWeaponSpeed(getSoldierWeapon(soldier));
    maxRange = getWeaponRange(getSoldierWeapon(soldier));


    Tile tiles[32][32];
    struct tilesWithCollision{
        int x;
        int y;
    };



    UDPsocket sd;
	IPaddress srvadd;
	UDPpacket *p;
    UDPpacket *p2;

    Data udpData;
    Data udpData2;

    if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!(sd = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    /* Resolve server name  */
	if (SDLNet_ResolveHost(&srvadd, "127.0.0.1", 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!((p = SDLNet_AllocPacket(1024))&& (p2 = SDLNet_AllocPacket(1024))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    

    gRenderer = SDL_CreateRenderer(theApp->window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);

    loadMedia(gRenderer, &mSoldier, gSpriteClips, &mTiles, gTiles, soldier, &mHealthBar, gSpriteClipsHealth);
    loadMedia(gRenderer, &mSoldier1, gSpriteClips, &mTiles, gTiles, soldier, &mHealthBar, gSpriteClipsHealth);
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
                        healthBarPosition.y = playerPosition.y - 12;
                        flip = SDL_FLIP_NONE;
                        if(frame == 4)
                            frame = 5;
                        else
                            frame = 4;
                        break;
                    case SDLK_DOWN:
                        playerPosition.y += 2;
                        setSoldierPositionY(soldier, playerPosition.y);
                        healthBarPosition.y = playerPosition.y - 12;
                        flip = SDL_FLIP_NONE;
                        if(frame == 0)
                            frame = 1;
                        else
                            frame = 0;
                        break;
                    case SDLK_LEFT:
                        playerPosition.x -= 2;
                        setSoldierPositionX(soldier, playerPosition.x);
                        healthBarPosition.x = playerPosition.x - 4;
                        flip = SDL_FLIP_HORIZONTAL;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    case SDLK_RIGHT:
                        playerPosition.x += 2;
                        setSoldierPositionX(soldier, playerPosition.x);
                        healthBarPosition.x = playerPosition.x - 4;
                        flip = SDL_FLIP_NONE;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    case SDLK_SPACE:
                        shotFired = true;
                        Bullet b = createBullet(playerPosition.x, playerPosition.y);
                        setBulletFrame(b, frame);
                        setBulletPositionX(b, playerPosition.x);
                        setBulletPositionY(b, playerPosition.y+14);
                        setBulletHeight(b, 5);
                        setBulletWidth(b, 10);
                        bulletAngle = checkBulletAngle(frame, &flip);
                        setBulletFlip(b,flip);
                        setBulletAngle(b,bulletAngle);
                        bullets[amountOfBullets] = b;
                        amountOfBullets++;
                        loadBulletMedia(gRenderer, &bulletTexture, &b);
                        loadBulletMedia(gRenderer, &recvBulletTexture, &b);
                    default:
                        
                        break;
                }
            }
        }
        
        soldierXPos = getSoldierPositionX(soldier);
        soldierYPos = getSoldierPositionY(soldier);



        // send and retrive positions  
        //if(oldX != soldierXPos || oldY != soldierYPos){   
            udpData.playerX = soldierXPos;
            udpData.playerY = soldierYPos;
            udpData.frame = frame;
            udpData.flip = flip;
            for (int i = 0; i < amountOfBullets; i++){
                udpData.bulletPosition[i].x = getBulletPositionX(bullets[i]);
                udpData.bulletPosition[i].y = getBulletPositionY(bullets[i]);
                udpData.bulletPosition[i].w = getBulletWidth(bullets[i]);
                udpData.bulletPosition[i].h = getBulletHeight(bullets[i]);
                udpData.bulletAngle[i] = getBulletAngle(bullets[i]);
                udpData.bulletFlip[i] = getBulletFlip(bullets[i]);
            }

            udpData.amountOfBullets = amountOfBullets;
            memcpy(p->data, &udpData, sizeof(struct data)+1);
            p->address.host = srvadd.host;	/* Set the destination host */
		    p->address.port = srvadd.port;	/* And destination port */
		    p->len = sizeof(struct data)+1;
            SDLNet_UDP_Send(sd, -1, p); 
            oldX = soldierXPos;
            oldY = soldierYPos;
            
        //}      
        
        //printf("UDP   %d\n", SDLNet_UDP_Recv(sd, p2));
        if (SDLNet_UDP_Recv(sd, p2)){

            memcpy(&udpData2, (char * ) p2->data, sizeof(struct data)+1);
            secondFrame = udpData2.frame;
            secondFlip = udpData2.flip;
            recvAmountOfBullets = udpData2.amountOfBullets;
            for (int i = 0; i < recvAmountOfBullets; i++)
            {
                Bullet recvB = createBullet(udpData2.bulletPosition[i].x, udpData2.bulletPosition[i].y);
                recvBullets[i] = recvB;
                setBulletPositionX(recvBullets[i], udpData2.bulletPosition[i].x);
                setBulletPositionY(recvBullets[i], udpData2.bulletPosition[i].y);
                setBulletWidth(recvBullets[i], udpData2.bulletPosition[i].w);
                setBulletHeight(recvBullets[i], udpData2.bulletPosition[i].h);
                setBulletAngle(recvBullets[i],udpData2.bulletAngle[i]);
                setBulletFlip(recvBullets[i],udpData2.bulletFlip[i]);
            }
            playerPosition1.x = udpData2.playerX;
            playerPosition1.y = udpData2.playerY;
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        renderBackground(gRenderer, mTiles, gTiles, tiles);
        checkPlayerOutOfBoundaries(soldier, &playerPosition);

        int checkPortalType;

        soldierWallCollision(tiles, soldier, &playerPosition, frame, flip, &healthBarPosition);
        
        bulletWallCollision(tiles, bullets, &amountOfBullets);
        SDL_RenderCopyEx(gRenderer, mSoldier, &gSpriteClips[frame],&playerPosition , 0, NULL, flip);
        SDL_RenderCopyEx(gRenderer, mHealthBar, &gSpriteClipsHealth[healthStatus],&healthBarPosition , 0, NULL, flip);
        SDL_RenderCopyEx(gRenderer, mSoldier1, &gSpriteClips[secondFrame],&playerPosition1 , 0, NULL, secondFlip);
        for (int i = 0; i < recvAmountOfBullets; i++){
            recvBulletPosition = getBulletPositionSDL(recvBullets[i]);
            recvBulletAngle = getBulletAngle(recvBullets[i]);
            recvBulletFlip = getBulletFlip(recvBullets[i]);
            SDL_RenderCopyEx(gRenderer, recvBulletTexture, &bullet,&recvBulletPosition, bulletAngle, NULL, recvBulletFlip);
            if(checkBulletOutOfBoundaries(recvBullets[i], recvBulletPosition)){
                free(recvBullets[i]);
                recvAmountOfBullets = deleteBullet(&recvAmountOfBullets,recvBullets, i);
            }
        }

        for (int i = 0; i < amountOfBullets; i++){
            bulletPosition = getBulletPositionSDL(bullets[i]);
            
            bulletFrame = getBulletFrame(bullets[i]);
            bullet = getBulletSDL(bullets[i]);
            bulletFlip = getBulletFlip(bullets[i]);
            
            bulletAngle = getBulletAngle(bullets[i]);
            move(&bulletPosition, bulletFrame, bulletFlip, weaponSpeed);
            SDL_RenderCopyEx(gRenderer, bulletTexture, &bullet,&bulletPosition, bulletAngle, NULL, bulletFlip);
            if(checkBulletOutOfBoundaries(b, bulletPosition) || checkBulletRangeMax(b,bulletPosition, maxRange, playerPosition))
            {
                //printf("%d RANGE\n", maxRange);
                free(bullets[i]);
                amountOfBullets = deleteBullet(&amountOfBullets,bullets, i);
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

PRIVATE int deleteBullet(int *amountOfBullets, Bullet bullets[],int delete)
{
    for (int i = delete; i < (*amountOfBullets-1); i++){
        bullets[i] = bullets[i+1];
    }
    (*amountOfBullets)--;
    return *amountOfBullets;
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

PRIVATE void stepBack(Soldier s, SDL_Rect *playerPosition, int frame, int flip){
    int newYPos, newXPos;
    if (frame == 0 || frame == 1){
        newYPos=(playerPosition->y-=2);
        setSoldierPositionY(s, newYPos);
    }
    if ((flip==0) && (frame == 2 || frame == 3)){
        newXPos=(playerPosition->x-=2);
        setSoldierPositionX(s, newXPos);
    }
    if ((flip==1) && (frame == 2 || frame == 3)){
        newXPos=(playerPosition->x+=2);
        setSoldierPositionX(s, newXPos);
    }
    if (frame == 4 || frame == 5){
        newYPos=(playerPosition->y+=2);
        setSoldierPositionY(s, newYPos);
    }
}

PRIVATE bool soldierWallCollision(Tile tiles[32][32], Soldier s, SDL_Rect *playerPosition, int frame, int flip, SDL_Rect *healthBarPosition){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            if(getTilePortal(tiles[i][j])==1){
                //Rect Player
                leftA = (getSoldierPositionX(s)+6);
                rightA = (getSoldierPositionX(s) + (getSoldierWidth()-8));
                topA = (getSoldierPositionY(s)+6);
                bottomA = (getSoldierPositionY(s) + (getSoldierHeight()-8));

                //Rect Tile
                leftB = getTilePositionX(tiles[i][j]);
                rightB = (getTilePositionX(tiles[i][j]) + getTileWidth());
                topB = getTilePositionY(tiles[i][j]);
                bottomB = (getTilePositionY(tiles[i][j]) + getTileHeight());

                if( (bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) ){
                }else{
                    teleportThingy(s,tiles, i, j, playerPosition, healthBarPosition);
                }    
            }
            if(getTileCollision(tiles[i][j])==1){
                
                //Rect Player
                leftA = (getSoldierPositionX(s)+4);
                rightA = (getSoldierPositionX(s) + (getSoldierWidth()-6));
                topA = (getSoldierPositionY(s)+4);
                bottomA = (getSoldierPositionY(s) + (getSoldierHeight()-6));

                //Rect Tile
                leftB = getTilePositionX(tiles[i][j]);
                rightB = (getTilePositionX(tiles[i][j]) + getTileWidth());
                topB = getTilePositionY(tiles[i][j]);
                bottomB = (getTilePositionY(tiles[i][j]) + getTileHeight());

                if( (bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) ){
                }else{
                    stepBack(s, playerPosition, frame, flip);
                }
            }   
        }
    }
    return false;
}

PRIVATE void bulletWallCollision(Tile tiles[32][32], Bullet bullets[], int *counter){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            if(getTileCollision(tiles[i][j])==1){
                for (int k = 0; k < (*counter); k++){

                    //Rect Bullet
                    leftA = getBulletPositionX(bullets[k]);
                    rightA = (getBulletPositionX(bullets[k]) + getBulletWidth(bullets[k]));
                    topA = getBulletPositionY(bullets[k]);
                    bottomA = (getBulletPositionY(bullets[k]) + getBulletHeight(bullets[k]));

                    //Rect Tile
                    leftB = getTilePositionX(tiles[i][j]);
                    rightB = (getTilePositionX(tiles[i][j]) + getTileWidth());
                    topB = getTilePositionY(tiles[i][j]);
                    bottomB = (getTilePositionY(tiles[i][j]) + getTileHeight());

                    if( (bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) ){
                    }else{
                        deleteBullet(counter, bullets, k);
                    }                    
                }
            }
        }
    }
}

PRIVATE int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition)
{
    if(bulletPosition.x >= WINDOW_WIDTH || bulletPosition.y >= WINDOW_HEIGHT || bulletPosition.x <= -10 || bulletPosition.y <= -10){
        return 1;
    }else{
        return 0;
    }
}

PRIVATE int checkBulletRangeMax(Bullet b, SDL_Rect bulletPosition, int maxRange, SDL_Rect playerPosition)
{
    if(bulletPosition.x == playerPosition.x+maxRange || bulletPosition.y == playerPosition.y+maxRange){
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
    SDL_Surface* bulletSurface = IMG_Load("resources/THEBULLET.png");
    *bulletTexture = SDL_CreateTextureFromSurface(gRenderer, bulletSurface);

    setBulletSDLPos(*bullet, 0,0,10,5);
}

PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[], Soldier s, SDL_Texture **mHealthBar, SDL_Rect gSpriteClipsHealth[])
{
    SDL_Surface* gHealthSurface = IMG_Load("resources/AllHealthStatuses6x6.PNG");
    *mHealthBar = SDL_CreateTextureFromSurface(gRenderer, gHealthSurface);

    gSpriteClipsHealth[ 0 ].x =0;
    gSpriteClipsHealth[ 0 ].y =0;
    gSpriteClipsHealth[ 0 ].w =36;
    gSpriteClipsHealth[ 0 ].h =8;

    gSpriteClipsHealth[ 1 ].x =36;
    gSpriteClipsHealth[ 1 ].y =0;
    gSpriteClipsHealth[ 1 ].w =36;
    gSpriteClipsHealth[ 1 ].h =8;

    gSpriteClipsHealth[ 2 ].x =72;
    gSpriteClipsHealth[ 2 ].y =0;
    gSpriteClipsHealth[ 2 ].w =36;
    gSpriteClipsHealth[ 2 ].h =8;

    gSpriteClipsHealth[ 3 ].x =108;
    gSpriteClipsHealth[ 3 ].y =0;
    gSpriteClipsHealth[ 3 ].w =36;
    gSpriteClipsHealth[ 3 ].h =8;

    gSpriteClipsHealth[ 4 ].x =144;
    gSpriteClipsHealth[ 4 ].y =0;
    gSpriteClipsHealth[ 4 ].w =36;
    gSpriteClipsHealth[ 4 ].h =8;

    gSpriteClipsHealth[ 5 ].x =180;
    gSpriteClipsHealth[ 5 ].y =0;
    gSpriteClipsHealth[ 5 ].w =36;
    gSpriteClipsHealth[ 5 ].h =8;

    gSpriteClipsHealth[ 6 ].x =216;
    gSpriteClipsHealth[ 6 ].y =0;
    gSpriteClipsHealth[ 6 ].w =36;
    gSpriteClipsHealth[ 6 ].h =8;

    gSpriteClipsHealth[ 7 ].x =252;
    gSpriteClipsHealth[ 7 ].y =0;
    gSpriteClipsHealth[ 7 ].w =36;
    gSpriteClipsHealth[ 7 ].h =8;

    gSpriteClipsHealth[ 8 ].x =288;
    gSpriteClipsHealth[ 8 ].y =0;
    gSpriteClipsHealth[ 8 ].w =36;
    gSpriteClipsHealth[ 8 ].h =8;

    gSpriteClipsHealth[ 9 ].x =324;
    gSpriteClipsHealth[ 9 ].y =0;
    gSpriteClipsHealth[ 9 ].w =36;
    gSpriteClipsHealth[ 9 ].h =8;

    gSpriteClipsHealth[ 10 ].x =360;
    gSpriteClipsHealth[ 10 ].y =0;
    gSpriteClipsHealth[ 10 ].w =36;
    gSpriteClipsHealth[ 10 ].h =8;


    SDL_Surface* gSpacemanSurface = IMG_Load(getSoldierFileName(s));
    *mSpaceman = SDL_CreateTextureFromSurface(gRenderer, gSpacemanSurface);
    
     
    gSpriteClips[ 0 ].x =   0;
    gSpriteClips[ 0 ].y =   0;
    gSpriteClips[ 0 ].w =  32;
    gSpriteClips[ 0 ].h = 32;
    
    gSpriteClips[ 1 ].x =  66;
    gSpriteClips[ 1 ].y =   0;
    gSpriteClips[ 1 ].w =  32;
    gSpriteClips[ 1 ].h = 32;
    
    gSpriteClips[ 2 ].x = 0;
    gSpriteClips[ 2 ].y =   64;
    gSpriteClips[ 2 ].w =  32;
    gSpriteClips[ 2 ].h = 32;
    
    gSpriteClips[ 3 ].x = 66;
    gSpriteClips[ 3 ].y =   64;
    gSpriteClips[ 3 ].w =  32;
    gSpriteClips[ 3 ].h = 32;
    
    gSpriteClips[ 4 ].x = 0;
    gSpriteClips[ 4 ].y =   96;
    gSpriteClips[ 4 ].w =  32;
    gSpriteClips[ 4 ].h = 32;
    
    gSpriteClips[ 5 ].x = 66;
    gSpriteClips[ 5 ].y =   96;
    gSpriteClips[ 5 ].w =  32;
    gSpriteClips[ 5 ].h = 32;

    SDL_Surface* gTilesSurface = IMG_Load("resources/GrassTowersPortals.PNG");
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

PRIVATE void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[32][32]){
    
    SDL_Rect possition;

    int height=0, width=0, number=0;
    height = getTileHeight();
    width = getTileWidth();

    possition.y = 0;
    possition.x = 0;
    possition.h = getTileHeight();
    possition.w = getTileWidth();

    for (int i = 0; i<getTileColumns(); i++){
        for (int j = 0; j<getTileRows(); j++){
            number = getTileGrid(i,j);
            Tile tile=createTile(possition.x, possition.y, number);
            setTileSDLRec(tile, (j*width), (i*height), width, height);
            possition=getTileSDLRec(tile);
            if(number==0x03 || number==0x04 || number==0x08||number==0x09){
                setTileCollision(tile, 1);
            }else if(number == 0x0d || number==0x0a){
                setTilePortal(tile, 1);
            }
            else{
                setTileCollision(tile, 0);
                setTilePortal(tile, 0);
            }
            tiles[i][j]=tile;

            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i,j)],&possition , 0, NULL, SDL_FLIP_NONE);
        }
    }
}

PRIVATE void weaponChoiceHandler(Soldier soldier)
{

    Weapon pistol = createWeapon(200,10,6);
    Weapon bow = createWeapon(5,6,7);
    Weapon spear = createWeapon(5,6,7);
    Weapon rodBlue = createWeapon(5,6,7);
    Weapon rodRed = createWeapon(5,6,7);
    
    

    if (strstr(getSoldierFileName(soldier),"pistol"))
    {
        setSoldierWeapon(soldier,pistol);
    }
    if (strstr(getSoldierFileName(soldier),"bow"))
    {
        setSoldierWeapon(soldier,bow);
    }
    if (strstr(getSoldierFileName(soldier),"spear"))
    {
        setSoldierWeapon(soldier,spear);
    }
    if (strstr(getSoldierFileName(soldier),"rodBLUE"))
    {
        setSoldierWeapon(soldier,pistol);
    }
    if (strstr(getSoldierFileName(soldier),"rodRED"))
    {
        setSoldierWeapon(soldier,pistol);
    }
    printf("%d", getWeaponRange(getSoldierWeapon(soldier)));
}

PRIVATE void teleportThingy(Soldier s, Tile tiles[32][32], int i, int j, SDL_Rect *playerPosition, SDL_Rect *healthBarPosition){
   int newYPos, newXPos;
   
   if(getTileNumber(tiles[i][j])==0x0d){
       if((j==2)&&(i==12)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[25][26])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[25][26])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }
        else if((j==26)&&(i==24)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[13][1])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[13][1])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }        
       
   }
   else if(getTileNumber(tiles[i][j])==0x0a){
       if((j==24)&&(i==14)){   
            newYPos=(playerPosition->y=(getTilePositionY(tiles[19][8])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[19][8])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }
        else if((j==9)&&(i==21)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[12][24])));
            setSoldierPositionY(s, newYPos);
            healthBarPosition->y = playerPosition->y - 12;
            newXPos=(playerPosition->x=(getTilePositionX(tiles[12][24])));
            setSoldierPositionX(s, newXPos);
            healthBarPosition->x = playerPosition->x - 4;
        }        
       
   }  
}




