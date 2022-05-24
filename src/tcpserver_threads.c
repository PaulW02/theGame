#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "timers.h"

#include "handlers/playerhandler.h"
#include "network/clientmanager.h"
#include "player/soldier.h"
#include "draw/media.h"

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
    int gameState;
    PlayerLobbyInformation playerLobbyInformation[MAX_PLAYERS];
    int amountOfPlayersConnected;
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
    int threadNr = 0, gameOver = 0;
    Uint32 matchTimer = 0;
    serverGameInfo->amountOfPlayersConnected = 0;
    char soldierImagePath[PATHLENGTH];
    char soldierName[MAX_NAME];

    pthread_t threads[MAX_PLAYERS];
    while(!gameOver)
    {
        client=SDLNet_TCP_Accept(server);
        if(client)
        {
            
            // Asign player ID in a vacant position
            // Create thread for player
            serverGameInfo->gameState = 1;
            serverGameInfo->playerConnections[threadNr].id = threadNr;
            serverGameInfo->playerConnections[threadNr].sock = client;
            serverGameInfo->id = threadNr;

            SDLNet_TCP_Recv(serverGameInfo->playerConnections[threadNr].sock, soldierImagePath, PATHLENGTH+1);
            SDLNet_TCP_Recv(serverGameInfo->playerConnections[threadNr].sock, soldierName, MAX_NAME+1);
            Soldier newSoldier;
            setValuesForConnectedPlayer(&newSoldier, threadNr, soldierImagePath, soldierName);
            serverGameInfo->soldiers[threadNr] = newSoldier;
            strcpy(serverGameInfo->playerLobbyInformation[threadNr].soldierImagePath, getSoldierFileName(serverGameInfo->soldiers[threadNr]));
            strcpy(serverGameInfo->playerLobbyInformation[threadNr].soldierName, getSoldierName(serverGameInfo->soldiers[threadNr]));
            serverGameInfo->amountOfPlayersConnected++;
            SDLNet_TCP_Send(serverGameInfo->playerConnections[threadNr].sock, &serverGameInfo->amountOfPlayersConnected, sizeof(serverGameInfo->amountOfPlayersConnected));
            SDLNet_TCP_Send(serverGameInfo->playerConnections[threadNr].sock, serverGameInfo->playerLobbyInformation, sizeof(serverGameInfo->playerLobbyInformation));
            pthread_create(&threads[threadNr], NULL, handlePlayer, (void *)serverGameInfo);
            threadNr++;
            printf("%d Threads\n", threadNr);
        }

        if(serverGameInfo->gameState == 2)
        {
            if (!matchTimer) 
            {
                matchTimer = SDL_GetTicks() + 180000; //3 minutes
            }
            else if (SDL_TICKS_PASSED(SDL_GetTicks(), matchTimer))
            {
                serverGameInfo->gameState = 0;
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
    while (((ServerGameInfo *)ptr)->gameState == 1){
       /* SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock,&((ServerGameInfo *)ptr)->amountOfPlayersConnected, sizeof(((ServerGameInfo *)ptr)->amountOfPlayersConnected));
        SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->playerLobbyInformation, sizeof(((ServerGameInfo *)ptr)->playerLobbyInformation));
        for (int i = 0; i < ((ServerGameInfo *)ptr)->amountOfPlayersConnected; i++)
        {
            printf("%s %s SERVER\n", ((ServerGameInfo *)ptr)->playerLobbyInformation[i].soldierName, ((ServerGameInfo *)ptr)->playerLobbyInformation[i].soldierImagePath);
        }
        usleep(100000);*/        
        if(((ServerGameInfo *)ptr)->amountOfPlayersConnected == MAX_PLAYERS){
            ((ServerGameInfo *)ptr)->gameState = 2;
        }

        
    }
    
    //countDown();
    //usleep(20000000);
    SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->playerLobbyInformation, sizeof(((ServerGameInfo *)ptr)->playerLobbyInformation));



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

    while (((ServerGameInfo *)ptr)->gameState == 2)
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