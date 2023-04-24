#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "audio.h"

int init_audio()
{
    // Initialize SDL2 Mixer library
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer: %s", Mix_GetError());
        return -1;
    }

    // Load OGG file
    Mix_Music *music = Mix_LoadMUS("resources/main_theme.wav");
    if (!music)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
        Mix_CloseAudio();
        return -1;
    }

    // Set the music volume to 5%
    Mix_VolumeMusic(MIX_MAX_VOLUME / 20);

    // Play music in a loop
    Mix_PlayMusic(music, -1);

    return 0;
}
