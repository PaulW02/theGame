#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "handlers/playerhandler.h"
#include "player/soldier.h"

#define MAX_PLAYERS 4

struct playersData{
    int id;
	int x;
	int y;
    int frame;
    int shotFired;
};
typedef struct playersData PlayersData;

struct playerConnDetails{
    TCPsocket sock;
	int id;
    int connected;
};
typedef struct playerConnDetails PlayerConnDetails;

struct gameInfo {
    PlayerConnDetails playerConnections[MAX_PLAYERS];
    PlayersData playersData[MAX_PLAYERS];
    int connectedPlayers[MAX_PLAYERS];
    Soldier soldiers[MAX_PLAYERS];
    int id;
};
typedef struct gameInfo GameInfo;

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
    GameInfo *gameInfo = (struct gameInfo *)malloc(sizeof(struct gameInfo));
    int *number;
    char text[100];
    int threadNr = 0;
    
    for (int i = 0; i < MAX_PLAYERS; i++){
        gameInfo->connectedPlayers[i] = 0;
    }
    

    pthread_t threads[MAX_PLAYERS];
    while(1)
    {
        client=SDLNet_TCP_Accept(server);
        if(client)
        {
            
            // Asign player ID in a vacant position
            // Create thread for player
            gameInfo->playerConnections[threadNr].id = threadNr;
            gameInfo->playerConnections[threadNr].sock = client;
            gameInfo->id = threadNr;
            pthread_create(&threads[threadNr], NULL, handlePlayer, (void *)gameInfo);
            threadNr++;
            printf("%d Threads\n", threadNr);
        }
    }
    SDLNet_TCP_Close(server);
    
    SDLNet_Quit();
    SDL_Quit();
}

void *handlePlayer(void *ptr) {
    int playerInfo[6];
    int currentPlayerId = ((GameInfo *)ptr)->id;
    
    Soldier newSoldier;
    PlayersData receivedPlayersData;
    if(currentPlayerId == 0){
        newSoldier = createSoldier(10, 10);
        setSoldierFrame(newSoldier, 2);
    }else if(currentPlayerId == 1){
        newSoldier = createSoldier(470, 10);
        setSoldierFrame(newSoldier, 6);
    }else if(currentPlayerId == 2){
        newSoldier = createSoldier(470, 470);
        setSoldierFrame(newSoldier, 6);
    }else{
        newSoldier = createSoldier(10, 470);
        setSoldierFrame(newSoldier, 2);
    }

    setSoldierId(newSoldier, currentPlayerId);
    setSoldierPosition(newSoldier, getSoldierPositionX(newSoldier), getSoldierPositionY(newSoldier), 32, 32);
    setSoldierFileName(newSoldier,"resources/Karaktarer/BOY/BOYpistol.png");
    setSoldierShotFired(newSoldier, 0);
    weaponChoiceHandler(newSoldier);
    ((GameInfo *)ptr)->soldiers[currentPlayerId] = newSoldier;
    ((GameInfo *)ptr)->connectedPlayers[currentPlayerId] = 1;
    playerInfo[0] = currentPlayerId;
    playerInfo[1] = getSoldierFrame(newSoldier);
    playerInfo[2] = getSoldierPositionX(newSoldier);
    playerInfo[3] = getSoldierPositionY(newSoldier);
    playerInfo[4] = ((GameInfo *)ptr)->connectedPlayers[currentPlayerId];
    playerInfo[5] = getSoldierShotFired(newSoldier);
    if(SDLNet_TCP_Send(((GameInfo *)ptr)->playerConnections[currentPlayerId].sock, playerInfo, sizeof(playerInfo))< sizeof(playerInfo)){
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }



    int gameOver = 0;
    // While loop
    while (!gameOver)
    {
        
        if(SDLNet_TCP_Recv(((GameInfo *)ptr)->playerConnections[currentPlayerId].sock, &receivedPlayersData, sizeof(struct playersData)))
        {
            ((GameInfo *)ptr)->playersData[currentPlayerId].id = currentPlayerId;
            ((GameInfo *)ptr)->playersData[currentPlayerId].x = receivedPlayersData.x;
            ((GameInfo *)ptr)->playersData[currentPlayerId].y = receivedPlayersData.y;
            ((GameInfo *)ptr)->playersData[currentPlayerId].frame = receivedPlayersData.frame;
            ((GameInfo *)ptr)->playersData[currentPlayerId].shotFired = receivedPlayersData.shotFired;
            
            //printf("%d %d %d\n", ((GameInfo *)ptr)->playersMovement[currentPlayerId].id, ((GameInfo *)ptr)->playersMovement[currentPlayerId].x, ((GameInfo *)ptr)->playersMovement[currentPlayerId].y);
            
        }

        //printf("%d Will be sending now... \n", currentPlayerId);
        SDLNet_TCP_Send(((GameInfo *)ptr)->playerConnections[currentPlayerId].sock, ((GameInfo *)ptr)->playersData, 4*sizeof(struct playersData));


		if(SDLNet_GetError() && strlen(SDLNet_GetError())) { // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        }

        usleep(0);
    }
    


}