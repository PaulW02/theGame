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

#define MAX_BULLETS 100
#define MAX_PLAYERS 4
#define AMOUNT_TILES 32
#define CONN_PARAMS_LENGTH 20

struct application{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    window_event;

};

PRIVATE bool init(SDL_Renderer **gRenderer);
PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[], Soldier s);
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture);
PRIVATE void update(Application theApp, double delta_time);
PRIVATE void eventKeyHandler(Soldier soldier, Bullet bullets[MAX_BULLETS], int *frame);
PRIVATE void dataPackageHandler(Soldier soldiers[], int amountOfBullets, int *soldierXPos, int *soldierYPos, int *oldX, int *oldY, Bullet bullets[MAX_BULLETS], int *playerId, int bulletsActive, UDPsocket sd, IPaddress srvadd, UDPpacket *p, UDPpacket *p2);
PRIVATE void bulletsRenderer(SDL_Renderer *gRenderer, Bullet bullets[MAX_BULLETS], SDL_Texture **bulletTexture, int *amountOfBullets, int weaponSpeed, int *bulletsActive);
PRIVATE void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES]);
PRIVATE void shootBullet(SDL_Renderer *gRenderer, int frame);
PRIVATE int deleteBullet(int *amountOfBullets, Bullet bullets[],int delete);
PRIVATE void checkPlayerOutOfBoundaries(Soldier s, SDL_Rect *playerPosition);
PRIVATE int checkBulletOutOfBoundaries(Bullet b, SDL_Rect bulletPosition);
PRIVATE int checkBulletRangeMax(Bullet b, SDL_Rect bulletPosition, int maxRange, SDL_Rect playerPosition);
PRIVATE int checkBulletAngle(int frame);
PRIVATE void weaponChoiceHandler(Soldier soldier);
PRIVATE void collisionCheck(Soldier s, SDL_Rect *playerPosition, Tile tiles[AMOUNT_TILES][AMOUNT_TILES]);
PRIVATE void stepBack(Soldier s, SDL_Rect *playerPosition, int frame);
PRIVATE bool soldierWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Soldier s, SDL_Rect *playerPosition, int frame);
PRIVATE void bulletWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Bullet bullets[], int *counter);
PRIVATE void teleportThingy(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int i, int j, SDL_Rect *playerPosition);
PRIVATE void createAllCurrentBullets(Soldier soldiers[], Bullet bullets[], int *amountOfBullets, int *bulletsActive);
PRIVATE int checkShotFired(Soldier soldiers[]);


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
    Soldier soldiers[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        soldiers[i] = createSoldier(100*i, 100*i);
        setSoldierId(soldiers[i], i);
        setSoldierPosition(soldiers[i], getSoldierPositionX(soldiers[i]), getSoldierPositionY(soldiers[i]), 32, 32);
        setSoldierFrame(soldiers[i], 0);
    }
    

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
   
    setSoldierFileName(soldiers[playerId],"resources/Karaktarer/BOY/BOYpistol.png");
    weaponChoiceHandler(soldiers[playerId]);
    weaponSpeed = getWeaponSpeed(getSoldierWeapon(soldiers[playerId]));
    maxRange = getWeaponRange(getSoldierWeapon(soldiers[playerId]));


    Tile tiles[AMOUNT_TILES][AMOUNT_TILES];

    UDPsocket sd;
	IPaddress srvadd;
	UDPpacket *p;
    UDPpacket *p2;


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

    loadMedia(gRenderer, &mSoldier, gSpriteClips, &mTiles, gTiles, soldiers[playerId]);
    loadBulletMedia(gRenderer, &bulletTexture);
    
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
                eventKeyHandler(soldiers[playerId], bullets, &frame);
            }else if( theApp->window_event.type == SDL_KEYUP){
                setSoldierShotFired(soldiers[playerId], 0);
            }
        }

        // Send and retrive information  
        dataPackageHandler(soldiers, amountOfBullets, &soldierXPos, &soldierYPos, &oldX, &oldY, bullets, &playerId, bulletsActive, sd, srvadd, p, p2);  

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        renderBackground(gRenderer, mTiles, gTiles, tiles);
        createAllCurrentBullets(soldiers, bullets, &amountOfBullets, &bulletsActive);

        

        
        
        bulletWallCollision(tiles, bullets, &amountOfBullets);
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            playerPosition = getSoldierPosition(soldiers[i]);
            checkPlayerOutOfBoundaries(soldiers[i], &playerPosition);
            soldierWallCollision(tiles, soldiers[i], &playerPosition, getSoldierFrame(soldiers[i]));
            SDL_RenderCopyEx(gRenderer, mSoldier, &gSpriteClips[getSoldierFrame(soldiers[i])],&playerPosition, 0, NULL, SDL_FLIP_NONE);
        }


        bulletsRenderer(gRenderer, bullets, &bulletTexture, &amountOfBullets, weaponSpeed, &bulletsActive);
        SDL_RenderPresent(gRenderer);
        update(theApp, 10.0/60.0);

    }
}

// Handles all key events
PRIVATE void eventKeyHandler(Soldier soldier, Bullet bullets[MAX_BULLETS], int *frame){
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_UP]||keystate[SDL_SCANCODE_W]){
        setSoldierPositionY(soldier, getSoldierPositionY(soldier)-2);
        if(*frame == 4){
            *frame = 5;
        }else{
            *frame = 4;
        }
    }
    if(keystate[SDL_SCANCODE_DOWN]||keystate[SDL_SCANCODE_S]){
        setSoldierPositionY(soldier, getSoldierPositionY(soldier)+2);
        if(*frame == 0){
            *frame = 1;
        }else{
            *frame = 0;
        }
    }
    if(keystate[SDL_SCANCODE_LEFT]||keystate[SDL_SCANCODE_A]){
        setSoldierPositionX(soldier, getSoldierPositionX(soldier)-2);
        if(*frame == 6){
            *frame = 7;
        }else{
            *frame = 6;
        }
    }
    if(keystate[SDL_SCANCODE_RIGHT]||keystate[SDL_SCANCODE_D]){
        setSoldierPositionX(soldier, getSoldierPositionX(soldier)+2);
        if(*frame == 2){
            *frame = 3;
        }else{
            *frame = 2;
        }
    }
    if(keystate[SDL_SCANCODE_SPACE]){
        setSoldierShotFired(soldier, 1);
    }
    setSoldierFrame(soldier, *frame);
    

}
 
// Sends and recieves data packages
PRIVATE void dataPackageHandler(Soldier soldiers[], int amountOfBullets, int *soldierXPos, int *soldierYPos, int *oldX, int *oldY, Bullet bullets[MAX_BULLETS], int *playerId, int bulletsActive, UDPsocket sd, IPaddress srvadd, UDPpacket *p, UDPpacket *p2){
    int connParams[CONN_PARAMS_LENGTH], margin = 0;
    *soldierXPos = getSoldierPositionX(soldiers[*playerId]);
    *soldierYPos = getSoldierPositionY(soldiers[*playerId]);
    if(*soldierXPos != *oldX || *soldierYPos != *oldY || bulletsActive){
        sprintf((char *)p->data, "%d %d %d %d %d\n",(int) getSoldierId(soldiers[*playerId]), (int) getSoldierPositionX(soldiers[*playerId]), (int) getSoldierPositionY(soldiers[*playerId]), (int) getSoldierFrame(soldiers[*playerId]), (int) getSoldierShotFired(soldiers[*playerId]));    
        p->address.host = srvadd.host;	/* Set the destination host */
        p->address.port = srvadd.port;	/* And destination port */
        p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(sd, -1, p); 
        *oldX = *soldierXPos;
        *oldY = *soldierYPos;
    }       

    if (SDLNet_UDP_Recv(sd, p2)){
        sscanf((char * )p2->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", playerId, &connParams[0], &connParams[1], &connParams[2], &connParams[3], &connParams[4], &connParams[5], &connParams[6], &connParams[7], &connParams[8], &connParams[9], &connParams[10], &connParams[11], &connParams[12], &connParams[13], &connParams[14], &connParams[15], &connParams[16], &connParams[17], &connParams[18], &connParams[19]);
        for(int i = 0; i < MAX_PLAYERS; i++) {
            for(int j = 0; j < 6; j++) {
                if(*playerId != i) {
                    switch(j) {
                        case 0:
                            setSoldierId(soldiers[i],connParams[j+margin]);
                            break;
                        case 1:
                            setSoldierPositionX(soldiers[i], connParams[j+margin]);
                            break;
                        case 2:
                            setSoldierPositionY(soldiers[i], connParams[j+margin]);
                            break;
                        case 3:
                            setSoldierFrame(soldiers[i], connParams[j+margin]);
                            break;
                        case 4:
                            setSoldierShotFired(soldiers[i], connParams[j+margin]);
                            break;
                    }
                }
            }
            margin += 5;
        }
    }

    
    

}

// Handles bullets
PRIVATE void bulletsRenderer(SDL_Renderer *gRenderer, Bullet bullets[MAX_BULLETS], SDL_Texture **bulletTexture, int *amountOfBullets, int weaponSpeed, int *bulletsActive){
    SDL_Rect bulletPosition, bullet;

    if(*amountOfBullets == 0){
        *bulletsActive = 0;
    }
    for (int i = 0; i < *amountOfBullets; i++){
        bulletPosition = getBulletPositionSDL(bullets[i]);
        bullet = getBulletSDL(bullets[i]);
        move(&bulletPosition, getBulletFrame(bullets[i]), weaponSpeed);
        SDL_RenderCopyEx(gRenderer, *bulletTexture, &bullet, &bulletPosition, getBulletAngle(bullets[i]), NULL, SDL_FLIP_NONE);
        if(checkBulletOutOfBoundaries(bullets[i], bulletPosition)){
            free(bullets[i]);
            *amountOfBullets = deleteBullet(amountOfBullets,bullets, i);
        }else{
            setBulletPositionX(bullets[i], bulletPosition.x);
            setBulletPositionY(bullets[i], bulletPosition.y);
        }
    }



    
}

PRIVATE void createAllCurrentBullets(Soldier soldiers[], Bullet bullets[], int *amountOfBullets, int *bulletsActive){
    int bulletAngle;
    if(*amountOfBullets == 0){
        *bulletsActive = 0;
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        
        if (getSoldierShotFired(soldiers[i]))
        {
            *bulletsActive = 1;
            Bullet b = createBullet(getSoldierPositionX(soldiers[i]), getSoldierPositionY(soldiers[i])+14, soldiers[i]);
            setBulletFrame(b, getSoldierFrame(soldiers[i]));
            bulletAngle = checkBulletAngle(getBulletFrame(b));
            setBulletAngle(b,bulletAngle);
            bullets[*amountOfBullets] = b;
            (*amountOfBullets)++;
            
        }
        
    }
}

PRIVATE int checkShotFired(Soldier soldiers[]){
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (getSoldierShotFired(soldiers[i]))
        {
            return 1;
        }
        
    }
    return 0;
}

PRIVATE int checkBulletAngle(int frame)
{
    if(frame == 0 || frame == 1){
        return 90;
    }else if( frame == 2 || frame == 3){
        return 0;
    }else if(frame == 6 || frame == 7){
        return 180;
    }else{   
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

// Collision detection functions
PRIVATE bool soldierWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Soldier s, SDL_Rect *playerPosition, int frame){
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

                if( !((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) ){
                    stepBack(s, playerPosition, frame);
                }
            }   
        }
    }
    return false;
}

PRIVATE void bulletWallCollision(Tile tiles[AMOUNT_TILES][AMOUNT_TILES], Bullet bullets[], int *counter){
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

PRIVATE void stepBack(Soldier s, SDL_Rect *playerPosition, int frame){
    int newYPos, newXPos;
    if (frame == 0 || frame == 1){
        newYPos=(playerPosition->y-=2);
        setSoldierPositionY(s, newYPos);
    }
    if ((frame == 2 || frame == 3)){
        newXPos=(playerPosition->x-=2);
        setSoldierPositionX(s, newXPos);
    }
    if ((frame == 6 || frame == 7)){
        newXPos=(playerPosition->x+=2);
        setSoldierPositionX(s, newXPos);
    }
    if (frame == 4 || frame == 5){
        newYPos=(playerPosition->y+=2);
        setSoldierPositionY(s, newYPos);
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


// Load media functions
PRIVATE void loadBulletMedia(SDL_Renderer *gRenderer, SDL_Texture **bulletTexture)
{
    SDL_Surface* bulletSurface = IMG_Load("resources/THEBULLET.png");
    *bulletTexture = SDL_CreateTextureFromSurface(gRenderer, bulletSurface);
}

PRIVATE void loadMedia(SDL_Renderer *gRenderer, SDL_Texture **mSpaceman, SDL_Rect gSpriteClips[], SDL_Texture **mTiles, SDL_Rect gTiles[], Soldier s)
{
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

    gSpriteClips[ 6 ].x = 0;
    gSpriteClips[ 6 ].y =   32;
    gSpriteClips[ 6 ].w =  32;
    gSpriteClips[ 6 ].h = 32;

    gSpriteClips[ 7 ].x = 66;
    gSpriteClips[ 7 ].y =   32;
    gSpriteClips[ 7 ].w =  32;
    gSpriteClips[ 7 ].h = 32;

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

PRIVATE void renderBackground(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[], Tile tiles[AMOUNT_TILES][AMOUNT_TILES]){
    
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
}

PRIVATE void teleportThingy(Soldier s, Tile tiles[AMOUNT_TILES][AMOUNT_TILES], int i, int j, SDL_Rect *playerPosition){
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




