#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

#include "../audio/audio.h"

#define SCREEN_WIDTH  1280 // 1600
#define SCREEN_HEIGHT 720 //  960

#define FPS 60

#define TIMER_MINUTES 5

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Color white;
    SDL_Event event;
    bool menuState;
    bool quit;
    bool isMuted;
    bool gameOver;
    bool goBackToMenu;
    bool quitGame;
    bool isPaused;
} Framework;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Camera;

typedef struct {
    bool active;
    SDL_Texture *texture;
    SDL_Rect rectangle;
} Image;

typedef struct {
    time_t timeWhenStarting;
    int timeRemaining; // Change the type to int
    int minutesRemaining;
    int secondsRemaining;
} Timer;

void initialize(Framework *game);
void manageFrameRate(int timeAtLoopBeginning);
bool manageTimer(Framework *game, Timer *timerData);
static void calculateRemainingTime(Timer *timerData);
static void displayTime(Framework *game, Timer *timerData);
static bool checkIfTimerHasExpired(Timer *timerData, Framework *game);
static void drawRectangle(SDL_Renderer *renderer, int x, int y, int w, int h);
void displayGameOverScreen(Framework *game, Timer *timerData);
void handleGameOver(Framework *game);

#endif