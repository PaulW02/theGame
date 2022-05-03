#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdbool.h>
#include "weapon.h"
#include "bullet.h"
#include "soldier.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define MAXCHAR 100

struct weapon{
    int range;
    int power;
    int speed;
    Bullet bullet;
    SDL_Texture *weaponTexture;
    SDL_Rect weapon;
    SDL_Surface weaponSurface;
    char weaponBullet[MAXCHAR];
};


PUBLIC Weapon createWeapon(int range, int power, int speed){
    Weapon w = malloc(sizeof(struct weapon));
    w->range = range;
    w->power = power;
    w->speed = speed;
    return w;
}

PUBLIC void setWeaponRange(Weapon w, int range){
    w->range = range;
}

PUBLIC int getWeaponRange(Weapon w){
    return w->range;
}

PUBLIC void setWeaponPower(Weapon w, int power){
    w->power = power;
}

PUBLIC int getWeaponPower(Weapon w){
    return w->power;
}

PUBLIC void setWeaponSpeed(Weapon w, int speed){
    w->speed = speed;
}

PUBLIC int getWeaponSpeed(Weapon w){
    return w->speed;
}

PUBLIC void setWeaponBullet(Weapon w, char weaponBullet[MAXCHAR])
{
    strcpy(w->weaponBullet,weaponBullet);
}

PUBLIC char* getWeaponBullet(Weapon w)
{
    return  w->weaponBullet;
}