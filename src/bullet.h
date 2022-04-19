
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#ifndef bullet_h
#define bullet_h

typedef struct bullet *Bullet;

Bullet createBullet(int x, int y, int speed);
void move(SDL_Rect *b, int frame, SDL_RendererFlip flip);
void setBulletPositionX(Bullet b, int positionX);
int getBulletPositionX(Bullet b);
void setBulletPositionY(Bullet b, int positionY);
int getBulletPositionY(Bullet b);
void setBulletHeight(Bullet b, int height);
int getBulletHeight(Bullet b);
void setBulletWidth(Bullet b, int width);
int getBulletWidth(Bullet b);
void setBulletSDLPos(Bullet b, int x, int y, int w, int h);
SDL_Rect getBulletSDL(Bullet b);
SDL_Rect getBulletPositionSDL(Bullet b);
void setBulletFrame(Bullet b, int frame);
int getBulletFrame(Bullet b);
void setBulletFlip(Bullet b, SDL_RendererFlip flip);
SDL_RendererFlip getBulletFlip(Bullet b);
void setBulletAngle(Bullet b, int bulletAngle);
int getBulletAngle(Bullet b);
void setShooter(Bullet b, int player);
int getShooter(Bullet b);

#endif