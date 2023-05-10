#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static Mix_Music *music;
static Mix_Chunk *walkingSound;
static Mix_Chunk *cageSound;

void initializeAudio(void);
void changeThemeSong(void);
void playWalkingSound(void);
void playCageSound(void);

#endif