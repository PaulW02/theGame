#include <stdio.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include "../sounds/soundeffects.h"
#include "eventhandler.h"

#include "../player/soldier.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void movementInput(SDL_Event appWindowEvent, Soldier s){
    //Bör ens dessa finnas kvar?
    int speedX=0, speedY=0;
    Weapon weapon = getSoldierWeapon(s);
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    //Vilket keystate
     
    if(appWindowEvent.type == SDL_KEYDOWN && appWindowEvent.key.repeat == 0){
        
        if(keystate[SDL_SCANCODE_UP]||keystate[SDL_SCANCODE_W]){
            speedY -= getSoldierSpeed(s);
            setSoldierSpeedY(s, speedY);
        }
        if(keystate[SDL_SCANCODE_DOWN]||keystate[SDL_SCANCODE_S]){
            speedY += getSoldierSpeed(s);
            setSoldierSpeedY(s, speedY);
        }
        if(keystate[SDL_SCANCODE_LEFT]||keystate[SDL_SCANCODE_A]){
            speedX -= getSoldierSpeed(s);
            setSoldierSpeedX(s, speedX);      
        }
        if(keystate[SDL_SCANCODE_RIGHT]||keystate[SDL_SCANCODE_D]){
            speedX += getSoldierSpeed(s);
            setSoldierSpeedX(s, speedX);
        }
        if(keystate[SDL_SCANCODE_SPACE]){
            setSoldierShotFired(s, 1);
            shootingSound();
            if(!getWeaponReload(weapon))
            {
                setSoldierShotFired(s, 1);
            }
        }
        if(keystate[SDL_SCANCODE_R])
        {
            setWeaponReload(weapon, true);
        }
    }
    
    if(appWindowEvent.type == SDL_KEYUP){
        switch(appWindowEvent.key.keysym.scancode){
            case SDL_SCANCODE_UP: case SDL_SCANCODE_W:
                setSoldierSpeedY(s, getSoldierSpeedY(s) + getSoldierSpeed(s));
                break;
            case SDL_SCANCODE_DOWN: case SDL_SCANCODE_S:
                setSoldierSpeedY(s, getSoldierSpeedY(s) - getSoldierSpeed(s));
                break;
            case SDL_SCANCODE_LEFT: case SDL_SCANCODE_A:
                setSoldierSpeedX(s, getSoldierSpeedX(s) + getSoldierSpeed(s));
                break;
            case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D:
                setSoldierSpeedX(s, getSoldierSpeedX(s) - getSoldierSpeed(s));
                break;
            case SDL_SCANCODE_SPACE:
                setSoldierShotFired(s, 0);
                break;
        }
    }
}

PUBLIC void motion(Soldier s, int *pframe, SDL_Rect *healthBarPosition, SDL_Rect *ammoPosition, SDL_Rect *bulletIndicator){
    int newYPos, newXPos;
    if((getSoldierFrameTimer(s))>2){
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
        (setSoldierFrameTimer(s, 0));
    }
    newYPos=(getSoldierPositionY(s))+(getSoldierSpeedY(s));
    setSoldierPositionY(s, newYPos);
    healthBarPosition->y = getSoldierPositionY(s) - 12;
    ammoPosition->y = healthBarPosition->y - 8;
    bulletIndicator->y = healthBarPosition->y - 9;

    newXPos=(getSoldierPositionX(s))+(getSoldierSpeedX(s));
    setSoldierPositionX(s, newXPos); 
    healthBarPosition->x = getSoldierPositionX(s) - 4;
    ammoPosition->x = healthBarPosition->x + 8;
    bulletIndicator->x = healthBarPosition->x;
    setSoldierFrame(s, (*pframe));

    SDL_Delay(0);
}
