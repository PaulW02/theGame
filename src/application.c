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
#include "tile.h"
#include <unistd.h> //Behövs inte men kan användas för "sleep();"

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
PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[]);
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture, Bullet *bullet);
PRIVATE void update(Application theApp, double delta_time);
PRIVATE void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[32][32]);
//PRIVATE void draw(Application theApp);
PRIVATE void shootBullet(SDL_Renderer *gRenderer, int frame);
PRIVATE int deleteBullet(int *amountOfBullets, Bullet bullets[],int delete);
PRIVATE void checkPlayerOutOfBoundaries(Soldier s, SDL_Rect *playerPosition);
PRIVATE int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition);
PRIVATE int checkBulletAngle(int frame, SDL_RendererFlip *flip);
PRIVATE void collisionCheck(Soldier s, SDL_Rect *playerPosition, Tile tiles[32][32]);
PRIVATE void stepBack(Soldier s, SDL_Rect *playerPosition, int frame, int flip);
PRIVATE bool soldierWallCollision(Tile tiles[32][32], Soldier s, SDL_Rect *playerPosition, int frame, int flip);
PRIVATE void bulletWallCollision(Tile tiles[32][32], Bullet bullets[], int *amountOfBullets);
PRIVATE void bulletPlayerCollision(Bullet bullets[], int *amountOfBullets, Soldier s1, Soldier s2);
PRIVATE void teleportThingy(Soldier s, Tile tiles[32][32], int i, int j, SDL_Rect *playerPosition);




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
    Soldier soldier = createSoldier(10,10, 1);
    Soldier soldier1 = createSoldier(20,10, 2);
    SDL_Texture *mSoldier = NULL;
    SDL_Texture *mSoldier1 = NULL;
    SDL_Rect gSpriteClips[8];
    SDL_Rect playerPosition;
    SDL_Rect playerPosition1;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    playerPosition.y = getSoldierPositionY(soldier);
    playerPosition.x = getSoldierPositionX(soldier);
    playerPosition.h = 32;
    playerPosition.w = 32;
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
    int amountOfBullets = 0;
    int recvAmountOfBullets = 0;
    bool shotFired = true;

    int playerXPos, playerYPos, playerFrame, bulletXPosToSend, bulletYPosToSend;
    SDL_RendererFlip playerFlip; 

     // Background
    SDL_Texture *mTiles = NULL;
    SDL_Rect gTiles[16];
   
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

    loadMedia(gRenderer, &mSoldier, gSpriteClips, &mTiles, gTiles);
    loadMedia(gRenderer, &mSoldier1, gSpriteClips, &mTiles, gTiles);
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

                        setShooter(b, getSoldierNumber(soldier));
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
        
        //printf("UDP RECIEVE %d\n", SDLNet_UDP_Recv(sd, p2));
        if (SDLNet_UDP_Recv(sd, p2)){

            memcpy(&udpData2, (char * ) p2->data, sizeof(struct data)+1);
            secondFrame = udpData2.frame;
            secondFlip = udpData2.flip;
            recvAmountOfBullets = udpData2.amountOfBullets;
            for (int i = 0; i < recvAmountOfBullets; i++)
            {
                Bullet recvB = createBullet(udpData2.bulletPosition[i].x, udpData2.bulletPosition[i].y, 5);
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

        soldierWallCollision(tiles, soldier, &playerPosition, frame, flip);
        bulletWallCollision(tiles, bullets, &amountOfBullets);
        bulletPlayerCollision(bullets, &amountOfBullets, soldier, soldier1);

        SDL_RenderCopyEx(gRenderer, mSoldier, &gSpriteClips[frame],&playerPosition , 0, NULL, flip);
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
            bulletXPos = getBulletPositionX(bullets[i]);
            bulletYPos = getBulletPositionY(bullets[i]);

            move(&bulletPosition, bulletFrame, bulletFlip);
            
            SDL_RenderCopyEx(gRenderer, bulletTexture, &bullet,&bulletPosition, bulletAngle, NULL, bulletFlip);

            if(checkBulletOutOfBoundaries(b, bulletPosition))
            {
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

PRIVATE bool soldierWallCollision(Tile tiles[32][32], Soldier s, SDL_Rect *playerPosition, int frame, int flip){
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
                    teleportThingy(s,tiles, i, j, playerPosition);
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

PRIVATE void bulletPlayerCollision(Bullet bullets[], int *amountOfBullets, Soldier s1, Soldier s2){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    Soldier soldiers[2]={s1, s2};

    for (int i = 0; i < (*amountOfBullets); i++){
        for (int j = 0; j < 2; j++){
               //Rect Bullet
            leftA = getBulletPositionX(bullets[i]);
            rightA = (getBulletPositionX(bullets[i]) + getBulletWidth(bullets[i]));
            topA = getBulletPositionY(bullets[i]);
            bottomA = (getBulletPositionY(bullets[i]) + getBulletHeight(bullets[i]));

                //Rect Player
            leftB = (getSoldierPositionX(soldiers[j]));
            rightB = (getSoldierPositionX(soldiers[j]) + (getSoldierWidth()));
            topB = (getSoldierPositionY(soldiers[j]));
            bottomB = (getSoldierPositionY(soldiers[j]) + (getSoldierHeight()));

            printf("%d", getShooter(bullets[i]));

            if( ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB) )){
            }else{
                if(((getShooter(bullets[i])) != (j+1))){
                    deleteBullet(amountOfBullets, bullets, i);
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

PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[])
{
    SDL_Surface* gSpacemanSurface = IMG_Load("resources/Karaktarer/SKELETON/SKELETONrodBLUE.png");
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

PRIVATE void teleportThingy(Soldier s, Tile tiles[32][32], int i, int j, SDL_Rect *playerPosition){
   int newYPos, newXPos;
   
   if(getTileNumber(tiles[i][j])==0x0d){
       if((j==2)&&(i==12)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[25][26])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[25][26])));
            setSoldierPositionX(s, newXPos);
        }
        else if((j==26)&&(i==24)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[13][1])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[13][1])));
            setSoldierPositionX(s, newXPos);
        }        
       
   }
   else if(getTileNumber(tiles[i][j])==0x0a){
       if((j==24)&&(i==14)){   
            newYPos=(playerPosition->y=(getTilePositionY(tiles[19][8])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[19][8])));
            setSoldierPositionX(s, newXPos);
        }
        else if((j==9)&&(i==21)){
            newYPos=(playerPosition->y=(getTilePositionY(tiles[12][24])));
            setSoldierPositionY(s, newYPos);
            newXPos=(playerPosition->x=(getTilePositionX(tiles[12][24])));
            setSoldierPositionX(s, newXPos);
        }        
       
   }  
}




