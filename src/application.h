
#ifndef application_h
#define application_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include "player/soldier.h"

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define MAX_PLAYERS 4
#define PATHLENGTH 64
#define MAX_NAME 16

typedef struct application *Application;



Application createApplication();
void applicationUpdate(Application theApp);
void destoryApplication(Application theApp);

#endif
