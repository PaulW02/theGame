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
    int magazine_size;
    int magazine;
    int firerate;
    int reloadTime;
    int bulletTimer;
    bool shotCooldown;
    bool reload;
    Bullet bullet;
    SDL_Texture *weaponTexture;
    SDL_Rect weapon;
    SDL_Surface weaponSurface;
    char weaponBullet[MAXCHAR];
};


PUBLIC Weapon createWeapon(int range, int power, int speed, int magazine_size, int firerate, int reloadTime){
    Weapon w = malloc(sizeof(struct weapon));
    w->range = range;
    w->power = power;
    w->speed = speed;
    w->magazine_size = magazine_size;
    w->magazine = magazine_size;
    w->firerate = firerate;
    w->reloadTime = reloadTime;
    w->bulletTimer = 0;
    w->shotCooldown = false;
    w->reload = false;
    return w;
}

PUBLIC void manageFireRate(Weapon w)
{
    if(getWeaponReload(w))
    {
        return;
    }
    else if (getWeaponBulletTimer(w) < getWeaponFirerate(w))
    {
        setWeaponShotCooldown(w,true);
        setWeaponBulletTimer(w,getWeaponBulletTimer(w)+1);
    }
    else
    {
        setWeaponShotCooldown(w,false);
    }
}

PUBLIC void manageReload(Weapon w)
{
    if (getWeaponReload(w))
    {
        if(getWeaponBulletTimer(w) >= getWeaponReloadTime(w))
        {
            setWeaponReload(w,false);
            setWeaponMagazine(w,getWeaponMagazine_Size(w));
        }
        setWeaponBulletTimer(w,getWeaponBulletTimer(w)+1);
    }
    else if (getWeaponMagazine(w) == 0)
    {
        setWeaponBulletTimer(w, 0);
        setWeaponReload(w,true);
    }
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
PUBLIC void setWeaponMagazine_Size(Weapon w, int magazine_size)
{
    w->magazine_size = magazine_size;
}

PUBLIC int getWeaponMagazine_Size(Weapon w){
    return w->magazine_size;
}

PUBLIC void setWeaponMagazine(Weapon w, int magazine){
    w->magazine = magazine;
}

PUBLIC int getWeaponMagazine(Weapon w){
    return w->magazine;
}

PUBLIC void setWeaponFirerate(Weapon w, int firerate){
    w->firerate = firerate;
}

PUBLIC int getWeaponFirerate(Weapon w){
    return w->firerate;
}

PUBLIC void setWeaponBulletTimer(Weapon w, int bulletTimer){
    w->bulletTimer = bulletTimer;
}

PUBLIC int getWeaponBulletTimer(Weapon w){
    return w->bulletTimer;
}

PUBLIC void setWeaponReloadTime(Weapon w, int reloadTime){
    w->reloadTime = reloadTime;
}

PUBLIC int getWeaponReloadTime(Weapon w){
    return w->reloadTime;
}

PUBLIC void setWeaponShotCooldown(Weapon w, bool shotCooldown){
    w->shotCooldown = shotCooldown;
}

PUBLIC bool getWeaponShotCooldown(Weapon w){
    return w->shotCooldown;
}

PUBLIC void setWeaponReload(Weapon w, bool reload){
    w->reload = reload;
}

PUBLIC bool getWeaponReload(Weapon w){
    return w->reload;
}