#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

#include "../audio/audio.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 960

#define FPS 60

#define TIMER_MINUTES 1

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TTF_Font *winFont;
    SDL_Event event;
    bool isMuted;
    bool quit;
} Framework;

typedef struct {
    bool active;
    SDL_Texture* texture;
    SDL_Rect rectangle;
} Image;

typedef struct {
    time_t timeWhenStarting;
    int timeRemaining;  // Change the type to int
    int minutesRemaining;
    int secondsRemaining;
} Timer;


void initialize(Framework *game);
void manageFrameRate(int timeAtLoopBeginning);
bool manageTimer(Framework *game, Timer *timerData);
static void calculateRemainingTime(Timer *timerData);
static void displayTime(Framework *game, Timer *timerData);
bool checkIfTimerHasExpired(Timer *timerData);
static void drawRectangle(SDL_Renderer *renderer, int x, int y, int w, int h);

#endif