#include "audio.h"

void initializeAudio(void) {
    // Open audio device
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer: %s", Mix_GetError());
        exit(1);
    }

    music = Mix_LoadMUS("resources/ThemeSong.mp3");
    if (!music) {
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

    cageLockSound = Mix_LoadWAV("resources/CageLockSound.mp3");
    if (cageLockSound == NULL) {
        printf("%s\n", Mix_GetError());
        exit(1);
    }
    Mix_VolumeChunk(cageLockSound, 20);

    cageUnlockSound = Mix_LoadWAV("resources/CageUnlockSound.mp3");
    if (cageUnlockSound == NULL) {
        printf("%s\n", Mix_GetError());
        exit(1);
    }
    Mix_VolumeChunk(cageUnlockSound, 20);

    menuClickSound = Mix_LoadWAV("resources/MenuClickSound.mp3");
    if (menuClickSound == NULL) {
        printf("%s\n", Mix_GetError());
        exit(1);
    }
    Mix_VolumeChunk(menuClickSound, 40);
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
    static bool menuSound = true;
    Mix_FreeMusic(music);
    if (menuSound) {
        music = Mix_LoadMUS("resources/main_theme2.mp3");
        if (!music) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
            exit(1);
        }
        menuSound = false;
    }
    else {
        music = Mix_LoadMUS("resources/ThemeSong.mp3");
        if (!music) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
            exit(1);
        }
        menuSound = true;
    }
    Mix_PlayMusic(music, -1);
}

void playCageLockSound(void) {
    Mix_PlayChannel(-1, cageLockSound, 0);
}

void playCageUnlockSound(void) {
    Mix_PlayChannel(-1, cageUnlockSound, 0);
}

void playMenuClickSound(void) {
    Mix_PlayChannel(-1, menuClickSound, 0);
}