#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static Mix_Music *music;
static Mix_Chunk *walkingSound;
static Mix_Chunk *cageLockSound;
static Mix_Chunk *cageUnlockSound;
static Mix_Chunk *menuClickSound;

void initializeAudio(void);
void changeThemeSong(void);
void playWalkingSound(void);
void playCageLockSound(void);
void playCageUnlockSound(void);
void playMenuClickSound(void);

#endif