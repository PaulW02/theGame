#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "soundeffects.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
//#include "../network/clientmanager.h"

#define PUBLIC
#define PRIVATE static

PUBLIC void initSoundEffects(int shootingSound){
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,2 ,2048 );
    Mix_Music *backgroundSound = Mix_LoadMUS("resources/backgroundmusic.wav");
    Mix_PlayMusic(backgroundSound,-1);
    Mix_VolumeMusic(8);
}

PUBLIC int shootingSound()
{
    Mix_Chunk *shotEffect = Mix_LoadWAV("resources/shoot.wav");
    Mix_PlayChannel(-1, shotEffect, 0);
    Mix_VolumeChunk(shotEffect, 5);
} 