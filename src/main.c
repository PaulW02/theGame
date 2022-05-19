#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "application.h"

int main()
{
    Application theApp = createApplication();
    applicationUpdate(theApp);
    destoryApplication(theApp);
}