#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "weapon.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


#define PUBLIC /* empty */
#define PRIVATE static

#define bulletVel 2

struct bullet{
    int xPos;
    int yPos;
    int speed;
    int power;
    int range;
    SDL_Rect bulletPosition;
    SDL_Rect bulletSDL;
    int bulletFrame;
    SDL_RendererFlip bulletflip;
    int bulletAngle;
    Weapon weapon;
};

PUBLIC Bullet createBullet(int x, int y){
    Bullet b = malloc(sizeof(struct bullet));
    b->xPos = x;
    b->yPos = y;
    return b;
}

PUBLIC void move(SDL_Rect *b, int frame, SDL_RendererFlip flip, int speed){
    switch (frame)
    {
    case 0:
        b->y += speed;
        break;
    case 1:
        b->y += speed;
        break;
    case 2:
        if(flip == SDL_FLIP_HORIZONTAL){
           b->x -= speed;
        }
        if(flip == SDL_FLIP_NONE){
            b->x += speed;
        }
        break;
    case 3:
        if(flip == SDL_FLIP_HORIZONTAL){
            b->x -= speed;
        }if(flip == SDL_FLIP_NONE){
            b->x += speed;
        }
        break; 
    case 4:
        b->y -= speed;
        break;
    case 5:
        b->y -= speed;
        break;
    default:
        break;
    }
}


PUBLIC void setBulletPositionX(Bullet b, int positionX){
    b->bulletPosition.x = positionX;
}

PUBLIC int getBulletPositionX(Bullet b){
    return b->bulletPosition.x;
}

PUBLIC void setBulletPositionY(Bullet b, int positionY){
    b->bulletPosition.y = positionY;
}

PUBLIC int getBulletPositionY(Bullet b){
    return b->bulletPosition.y;
}

PUBLIC void setBulletHeight(Bullet b, int height){
    b->bulletPosition.h = height;
}

PUBLIC int getBulletHeight(Bullet b){
    return b->bulletPosition.h;
}

PUBLIC void setBulletWidth(Bullet b, int width){
    b->bulletPosition.w = width;
}

PUBLIC int getBulletWidth(Bullet b){
    return b->bulletPosition.w;
}

PUBLIC void setBulletSDLPos(Bullet b, int x, int y, int w, int h){
    b->bulletSDL.x = x;
    b->bulletSDL.y = y;
    b->bulletSDL.w = w;
    b->bulletSDL.h = h;
    
}

PUBLIC SDL_Rect getBulletSDL(Bullet b){
    return b->bulletSDL;
}

PUBLIC SDL_Rect getBulletPositionSDL(Bullet b){
    return b->bulletPosition;
}

PUBLIC void setBulletFrame(Bullet b, int frame){
    b->bulletFrame = frame;
}

PUBLIC int getBulletFrame(Bullet b){
    return b->bulletFrame;
}

PUBLIC void setBulletFlip(Bullet b, SDL_RendererFlip flip){
    b->bulletflip = flip;
}

PUBLIC SDL_RendererFlip getBulletFlip(Bullet b){
    return b->bulletflip;
}

PUBLIC void setBulletAngle(Bullet b, int bulletAngle){
    b->bulletAngle = bulletAngle;
}

PUBLIC int getBulletAngle(Bullet b){
    return b->bulletAngle;
}