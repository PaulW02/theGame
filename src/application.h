
#ifndef application_h
#define application_h

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define MAX_PLAYERS 4

typedef struct application *Application;

Application createApplication();
void applicationUpdate(Application theApp);
void destoryApplication(Application theApp);

#endif
