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
    int matchDone;
    int playerKills[MAX_PLAYERS];
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
    serverGameInfo->matchDone = 0;
    char soldierImagePath[PATHLENGTH];
    char soldierName[MAX_NAME];

    pthread_t threads[MAX_PLAYERS];
    while(!gameOver)
    {
        client=SDLNet_TCP_Accept(server);
        if(client)
        {
            //Assign id for the client that connected to the server
            serverGameInfo->gameState = 1;
            serverGameInfo->playerConnections[threadNr].id = threadNr;
            serverGameInfo->playerConnections[threadNr].sock = client;
            serverGameInfo->id = threadNr;

            //Receive player image and name
            SDLNet_TCP_Recv(serverGameInfo->playerConnections[threadNr].sock, soldierImagePath, PATHLENGTH+1);
            SDLNet_TCP_Recv(serverGameInfo->playerConnections[threadNr].sock, soldierName, MAX_NAME+1);

            //Create new soldier with the values received
            Soldier newSoldier;
            setValuesForConnectedPlayer(&newSoldier, threadNr, soldierImagePath, soldierName);
            serverGameInfo->soldiers[threadNr] = newSoldier;
            strcpy(serverGameInfo->playerLobbyInformation[threadNr].soldierImagePath, getSoldierFileName(serverGameInfo->soldiers[threadNr]));
            strcpy(serverGameInfo->playerLobbyInformation[threadNr].soldierName, getSoldierName(serverGameInfo->soldiers[threadNr]));
            
            serverGameInfo->amountOfPlayersConnected++;
            
            //Create thread to handle the player that connected
            pthread_create(&threads[threadNr], NULL, handlePlayer, (void *)serverGameInfo);
            threadNr++;
            printf("%d Threads\n", threadNr);

        }

        //Check if game has started (Game state == 2) and if so then start a timer that counts to 3 minutes before it closes everything
        if(serverGameInfo->gameState == 2)
        {
            if (!matchTimer) 
            {
                matchTimer = SDL_GetTicks() + 180000 + 10000; //3 minutes (+ 10s because of lobby delay)
            }
            else if (SDL_TICKS_PASSED(SDL_GetTicks(), matchTimer))
            {
                serverGameInfo->matchDone = 1;
            }
        }
    }

    //Waiting for threads
    for (int i = 0; i < threadNr; i++)
    {
        pthread_join(threads[i], NULL);
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
    PlayersData receivedPlayersData;

    //Setting start value outside of the map
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        ((ServerGameInfo *)ptr)->playersData[i].x = -50;
        ((ServerGameInfo *)ptr)->playersData[i].y = -50;
    }
    
    
    //Looping until there are 4 players connected and then sending to all clients the amountOfPlayersConnected variable and the image and name of all the 4 players
    while (((ServerGameInfo *)ptr)->amountOfPlayersConnected < MAX_PLAYERS);
    SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock,&((ServerGameInfo *)ptr)->amountOfPlayersConnected, sizeof(((ServerGameInfo *)ptr)->amountOfPlayersConnected));
    SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->playerLobbyInformation, sizeof(((ServerGameInfo *)ptr)->playerLobbyInformation));
    
    //Setting gameState to 2
    ((ServerGameInfo *)ptr)->gameState = 2;

    //Setting the playerInfo array to all the player values of current player and send them to the client that uses that player
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

    //Looping while the match is on
    while (((ServerGameInfo *)ptr)->gameState == 2)
    {
        //Receiving a players data and setting them to the correct index of the playersData array
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

        //Sending the whole playersData array to the client
        SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->playersData, sizeof(((ServerGameInfo *)ptr)->playersData));

        //Checking to see if the timer is done, if it is end the match by setting gameState to 0
        if (((ServerGameInfo *)ptr)->matchDone)
        {
            ((ServerGameInfo *)ptr)->gameState = 0;
        }

        if(currentPlayerId == 0){
            SDLNet_TCP_Recv(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->playerKills, sizeof(((ServerGameInfo *)ptr)->playerKills));
        }else{
            SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((ServerGameInfo *)ptr)->playerKills, sizeof(((ServerGameInfo *)ptr)->playerKills));
        }
        
        //Send the gameState variable everytime to let the clients know that the game is on or not
        SDLNet_TCP_Send(((ServerGameInfo *)ptr)->playerConnections[currentPlayerId].sock, &((ServerGameInfo *)ptr)->gameState, sizeof(((ServerGameInfo *)ptr)->gameState));

		if(SDLNet_GetError() && strlen(SDLNet_GetError())) { // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        }

    }
}