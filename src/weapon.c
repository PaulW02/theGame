#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdbool.h>
#include "weapon.h"
#include "bullet.h"

#define PUBLIC /* empty */
#define PRIVATE static

struct weapon{
    Bullet bullet;
    SDL_Texture *weaponTexture;
    SDL_Rect weapon;
    SDL_Surface weaponSurface;

};