#include "audio.h"

void initializeAudio(void) {
    // Open audio device
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer: %s", Mix_GetError());
        exit(1);
    }

    // Load wav file
    music = Mix_LoadMUS("resources/ThemeSong.mp3");
    if (!music)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
        exit(1);
    }

    // Set the music volume to 20%
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);

    // Play the main theme in a loop
    Mix_PlayMusic(music, -1);

    // Load walking sound
    playWalkingSound();

}

void playWalkingSound(void) {
    static Mix_Chunk *sound;
    static int timeSinceLastCalled;
    int CurrentTime;
    if (!sound) {
        sound = Mix_LoadWAV("resources/WalkingSound.mp3");
        Mix_VolumeChunk(sound, 1);
    }
    else {
        CurrentTime = SDL_GetTicks() - timeSinceLastCalled;
        if (CurrentTime > 460) {
            Mix_PlayChannelTimed(-1, sound, 0, 460);
            timeSinceLastCalled = SDL_GetTicks();
        }
    }
}

void changeThemeSong(void) {
    music = Mix_LoadMUS("resources/main_theme.wav");
    if (!music)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
        exit(1);
    }

    // Play the main theme in a loop
    Mix_PlayMusic(music, -1);
}