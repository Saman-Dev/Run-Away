#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../audio/audio.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 900

#define FPS 60

#define TIMER_LENGTH 1000

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool isMuted;
    bool quit;
} Framework;

typedef struct {
    bool active;
    SDL_Texture* texture;
    SDL_Rect rectangle;
} Image;

void initialize(Framework *game);
void manageFrameRate(int timeAtLoopBeginning);
void checkTimeLeft(Framework *game, double elapsedTime);

#endif