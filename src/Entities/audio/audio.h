#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static Mix_Music *music;

void initializeAudio(void);
void playWalkingSound(void);
void changeThemeSong(void);

#endif