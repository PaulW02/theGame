#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "timers.h"

#include "handlers/playerhandler.h"
#include "network/clientmanager.h"
#include "player/soldier.h"
#include "draw/media.h"

#define MAX_PLAYERS 4
#define PATHLENGTH 64

struct playerConnDetails{
    TCPsocket sock;
	int id;
};
typedef struct playerConnDetails PlayerConnDetails;

struct serverGameInfo {
    PlayerConnDetails playerConnections[MAX_PLAYERS];
    PlayersData playersData[MAX_PLAYERS];
    Soldier soldiers[MAX_PLAYERS];
    int id;
    int amountOfPlayersConnected;
    char soldierImagePaths[MAX_PLAYERS][PATHLENGTH];
};
typedef struct serverGameInfo ServerGameInfo;

void *handlePlayer(void *ptr);

int main(int argc,char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDLNet_Init();
    
    IPaddress ip;
    SDLNet_ResolveHost(&ip,NULL,2000);
    
    TCPsocket server=SDLNet_TCP_Open(&ip);
    TCPsocket client;
    //PlayerConnDetails players[4] = (struct playerConnDetails*)malloc(4*sizeof(PlayerConnDetails));
    ServerGameInfo *serverGameInfo = (struct serverGameInfo *)malloc(sizeof(struct serverGameInfo));
    int threadNr = 0;
    int gameOver = 0, gameStarted = 0;
    Uint32 matchTimer = 0;
    serverGameInfo->amountOfPlayersConnected = 0;
    char soldierImagePath[PATHLENGTH];

    pthread_t threads[MAX_PLAYERS];
    while(!gameOver)
    {
        client=SDLNet_TCP_Accept(server);
        if(client)
        {
            
            // Asign player ID in a vacant position
            // Create thread for player
            serverGameInfo->playerConnections[threadNr].id = threadNr;
            serverGameInfo->playerConnections[threadNr].sock = client;
            serverGameInfo->id = threadNr;

            SDLNet_TCP_Recv(serverGameInfo->playerConnections[threadNr].sock, soldierImagePath, PATHLENGTH+1);
            Soldier newSoldier;
            setValuesForConnectedPlayer(&newSoldier, threadNr, soldierImagePath);
            serverGameInfo->soldiers[threadNr] = newSoldier;
            strcpy(serverGameInfo->soldierImagePaths[threadNr], getSoldierFileName(serverGameInfo->soldiers[threadNr]));
            serverGameInfo->amountOfPlayersConnected++;
            pthread_create(&threads[threadNr], NULL, handlePlayer, (void *)serverGameInfo);
            threadNr++;
            printf("%d Threads\n", threadNr);

        }
        if(serverGameInfo->amountOfPlayersConnected == 4)
        {
            gameStarted = 1;
        }
        if(gameStarted)
        {
            if (!matchTimer) 
            {
                matchTimer = SDL_GetTicks() + 180000; //3 minutes
            }
            else if (SDL_TICKS_PASSED(SDL_GetTicks(), matchTimer))
            {
                printf("Game Over!\n");
                gameOver = 1;
            }
        }
    }
    SDLNet_TCP_Close(server);
    
    SDLNet_Quit();
    SDL_Quit();
}

void *handlePlayer(void *ptr) {
    int playerInfo[7];
    int currentPlayerId = ((ServerGameInfo *)ptr)->id;
    char soldierFileName[PATHLENGTH];
    int oldAmountOfPlayersConnected = -1;
    int sentCount = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        ((ServerGameInfo *)ptr)->playersData[i].x = -50;
        ((ServerGameInfo *)ptr)->playersData[i].y = -50;
    }
    
    PlayersData receivedPlayersData;
    while (((ServerGameInfo *)ptr)->amountOfPlayersConnected < 4);
    
    //countDown();
    //usleep(20000000);

    SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->soldierImagePaths, 4*(PATHLENGTH+1));

    SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock,&((ServerGameInfo *)ptr)->amountOfPlayersConnected, sizeof(((ServerGameInfo *)ptr)->amountOfPlayersConnected));

    playerInfo[0] = currentPlayerId;
    playerInfo[1] = getSoldierFrame(((ServerGameInfo *)ptr)->soldiers[currentPlayerId]);
    playerInfo[2] = getSoldierPositionX(((ServerGameInfo *)ptr)->soldiers[currentPlayerId]);
    playerInfo[3] = getSoldierPositionY(((ServerGameInfo *)ptr)->soldiers[currentPlayerId]);
    playerInfo[4] = getSoldierShotFired(((ServerGameInfo *)ptr)->soldiers[currentPlayerId]);
    playerInfo[5] = getSoldierHealth(((ServerGameInfo *)ptr)->soldiers[currentPlayerId]);
    playerInfo[6] = getWeaponMagazine(getSoldierWeapon(((ServerGameInfo *)ptr)->soldiers[currentPlayerId]));
    if(SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, playerInfo, sizeof(playerInfo))< sizeof(playerInfo)){
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    int gameOver = 0;

    while (!gameOver)
    {
        if(SDLNet_TCP_Recv(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, &receivedPlayersData, sizeof(struct playersData)))
        {
            ((ServerGameInfo *)ptr)->playersData[currentPlayerId].id = currentPlayerId;
            ((ServerGameInfo *)ptr)->playersData[currentPlayerId].x = receivedPlayersData.x;
            ((ServerGameInfo *)ptr)->playersData[currentPlayerId].y = receivedPlayersData.y;
            ((ServerGameInfo *)ptr)->playersData[currentPlayerId].frame = receivedPlayersData.frame;
            ((ServerGameInfo *)ptr)->playersData[currentPlayerId].shotFired = receivedPlayersData.shotFired;
            ((ServerGameInfo *)ptr)->playersData[currentPlayerId].health = receivedPlayersData.health;
            ((ServerGameInfo *)ptr)->playersData[currentPlayerId].magazine = receivedPlayersData.magazine;
        }

        SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->playersData, sizeof(((ServerGameInfo *)ptr)->playersData));

		if(SDLNet_GetError() && strlen(SDLNet_GetError())) { // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        }
    }
}