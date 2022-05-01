#include <stdio.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"

#include "eventhandler.h"

#include "../player/soldier.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void movementInput(SDL_Event appWindowEvent, Soldier s, int *pframe, int *amountOfBullets){
    //Bör ens dessa finnas kvar?
    int speedX=0, speedY=0;
    
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    //Vilket keystate
     
    if(appWindowEvent.type == SDL_KEYDOWN && appWindowEvent.key.repeat == 0){
        
        if(keystate[SDL_SCANCODE_UP]){
            speedY -= getSoldierSpeed(s);
            setSoldierSpeedY(s, speedY);
        }
        if(keystate[SDL_SCANCODE_DOWN]){
            speedY += getSoldierSpeed(s);
            setSoldierSpeedY(s, speedY);
        }
        if(keystate[SDL_SCANCODE_LEFT]){
            speedX -= getSoldierSpeed(s);
            setSoldierSpeedX(s, speedX);      
        }
        if(keystate[SDL_SCANCODE_RIGHT]){
            speedX += getSoldierSpeed(s);
            setSoldierSpeedX(s, speedX);
        }
        if(keystate[SDL_SCANCODE_SPACE]){
            Weapon weapon = getSoldierWeapon(s);
            if(!getWeaponReload(weapon))
            {
                setSoldierShotFired(s, 1);
            }
        }
    }
    
    if(appWindowEvent.type == SDL_KEYUP){
        switch(appWindowEvent.key.keysym.scancode){
            case SDL_SCANCODE_UP:
                setSoldierSpeedY(s, getSoldierSpeedY(s) + getSoldierSpeed(s));
                break;
            case SDL_SCANCODE_DOWN:
                setSoldierSpeedY(s, getSoldierSpeedY(s) - getSoldierSpeed(s));
                break;
            case SDL_SCANCODE_LEFT:
                setSoldierSpeedX(s, getSoldierSpeedX(s) + getSoldierSpeed(s));
                break;
            case SDL_SCANCODE_RIGHT:
                setSoldierSpeedX(s, getSoldierSpeedX(s) - getSoldierSpeed(s));
                break;
        }
    }
}

PUBLIC void motion(Soldier s, int *pframe){
    int newYPos, newXPos;

    if(getSoldierSpeedX(s)>0 || (getSoldierSpeedX(s)>0 && getSoldierSpeedY(s)!=0)){
        if((*pframe) == 2)
            (*pframe) = 3;
        else
            (*pframe) = 2;  
    }else if (getSoldierSpeedX(s)<0 || (getSoldierSpeedX(s)<0 && getSoldierSpeedY(s)!=0)){
        if((*pframe) == 6)
            (*pframe) = 7;
        else
            (*pframe) = 6;
    }

    if(getSoldierSpeedY(s)>0 && getSoldierSpeedX(s)==0){
        if((*pframe) == 0)
            (*pframe) = 1;
        else
            (*pframe) = 0;
    }else if (getSoldierSpeedY(s)<0 && getSoldierSpeedX(s)==0){
        if((*pframe) == 4)
            (*pframe) = 5;
        else
            (*pframe) = 4;
    }
    newYPos=(getSoldierPositionY(s))+(getSoldierSpeedY(s));
    setSoldierPositionY(s, newYPos);

    newXPos=(getSoldierPositionX(s))+(getSoldierSpeedX(s));
    setSoldierPositionX(s, newXPos); 
    setSoldierFrame(s, (*pframe));
    SDL_Delay(25);
}
