/*#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>
#include <stdio.h>
 
#define MAX_PLAYERS 4



TCPsocket *add_client(TCPsocket sock, TCPsocket clients[], int *num_clients)
{
    if(*num_clients < MAX_PLAYERS){
        clients=(TCPsocket*)realloc(clients, (*num_clients+1)*sizeof(TCPsocket));
        clients[*num_clients]=sock;
        (*num_clients)++;
        //send_all();
    }
	return(&clients[*num_clients-1]);
}


int main(int argc,char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDLNet_Init();
    
    IPaddress ip;
    SDLNet_ResolveHost(&ip,NULL,1234);
    TCPsocket sock;
    TCPsocket server=SDLNet_TCP_Open(&ip);
    TCPsocket clients[MAX_PLAYERS];
    int num_clients = 0;
    char* text="HELLO SINEM!\n";
    char newText[100];
    while(1)
    {
        sock=SDLNet_TCP_Accept(server);
        if(sock)
        {
            char* name="You joined";

            /*printf("Accepted...\n"); 
            if(SDLNet_TCP_Recv(sock, text, strlen(text)+1))
            {

                
            }
            else
                SDLNet_TCP_Close(sock);
        }
    }
    SDLNet_TCP_Close(server);
    
    SDLNet_Quit();
    SDL_Quit();
}*/