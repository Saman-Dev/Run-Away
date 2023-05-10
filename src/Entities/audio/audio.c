#include "audio.h"

void initializeAudio(void) {
    // Open audio device
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer: %s", Mix_GetError());
        exit(1);
    }

    music = Mix_LoadMUS("resources/ThemeSong.mp3");
    if (!music)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
        exit(1);
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
    Mix_PlayMusic(music, -1);

    walkingSound = Mix_LoadWAV("resources/WalkingSound.mp3");
    if (walkingSound == NULL) {
        printf("%s\n", Mix_GetError());
        exit(1);
    }
    Mix_VolumeChunk(walkingSound, 1);

    cageSound = Mix_LoadWAV("resources/CageSound.mp3");
    if (cageSound == NULL) {
        printf("%s\n", Mix_GetError());
        exit(1);
    }
    Mix_VolumeChunk(cageSound, 20);
}

void playWalkingSound(void) {
    static int timeSinceLastCalled;
    int CurrentTime;
    CurrentTime = SDL_GetTicks() - timeSinceLastCalled;
    if (CurrentTime > 460) {
        Mix_PlayChannelTimed(-1, walkingSound, 0, 460);
        timeSinceLastCalled = SDL_GetTicks();
    }
}

void changeThemeSong(void) {
    music = Mix_LoadMUS("resources/main_theme2.mp3");
    if (!music) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
        exit(1);
    }
    Mix_PlayMusic(music, -1);
}

void playCageSound(void) {
    Mix_PlayChannel(-1, cageSound, 0);
}