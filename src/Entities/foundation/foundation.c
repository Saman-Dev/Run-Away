#include "foundation.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>




void initialize(Framework *game) {
    // Initialize SDL, timer and Mixer Library
    SDL_Init(SDL_INIT_VIDEO);
    initializeAudio();
    srand(time(NULL));
     // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize SDL_image, SDL_mixer, SDL_net, SDL_ttf libraries
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Failed to initialize SDL_mixer: %s\n", Mix_GetError());
        exit(1);
    }

    if (SDLNet_Init() == -1) {
        printf("Failed to initialize SDL_net: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (TTF_Init() == -1) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    game->winFont = TTF_OpenFont("resources/font.ttf", 48);
    if (game->winFont == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }

	if (SDLNet_Init() == -1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(1);
	}

    if (TTF_Init() == -1) {
        printf("TTF_Init error: %s\n", TTF_GetError());
    }

    game->font = TTF_OpenFont("resources/font.ttf", 24);
    if (game->font == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }

    game->window = SDL_CreateWindow("RUN AWAY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (game->window == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (game->renderer == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
}

void manageFrameRate(int timeAtLoopBeginning) {
    int endOfLoopTime;
    endOfLoopTime = (SDL_GetTicks()) - timeAtLoopBeginning;
    if (endOfLoopTime < (1000 / FPS)) {
        SDL_Delay((1000 / FPS) - endOfLoopTime);
    }
}

bool manageTimer(Framework *game, Timer *timerData) {
    calculateRemainingTime(timerData);
    displayTime(game, timerData);
    if(checkIfTimerHasExpired(timerData)){
        return true;
    }
}

static void calculateRemainingTime(Timer *timerData) {
    time_t currentTime = time(NULL);
    int timeElapsed = (int)difftime(currentTime, timerData->timeWhenStarting);

    if (timeElapsed >= (TIMER_MINUTES * 60)) {
        // Timer has expired, set remaining time to 0
        timerData->timeRemaining = 0;
    } else {
        timerData->timeRemaining = (TIMER_MINUTES * 60) - timeElapsed;
    }

    timerData->minutesRemaining = timerData->timeRemaining / 60;
    timerData->secondsRemaining = timerData->timeRemaining % 60;
}

static void displayTime(Framework *game, Timer *timerData) {
    static Image toDisplay = {0, NULL, {434, 15, 91, 35}};
    static SDL_Color color = {255, 255, 255};
    static int lastSeenTimeRemainingValue = 0;
    char numbersToPrint[8];
    SDL_Surface *surface;

    if (lastSeenTimeRemainingValue != timerData->timeRemaining) {
        lastSeenTimeRemainingValue = timerData->timeRemaining;

        snprintf(numbersToPrint, sizeof(numbersToPrint), "%02d:%02d", timerData->minutesRemaining, timerData->secondsRemaining);

        surface = TTF_RenderText_Solid(game->font, numbersToPrint, color);
        if (surface == NULL) {
            printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        }

        toDisplay.texture = SDL_CreateTextureFromSurface(game->renderer, surface);
        if (toDisplay.texture == NULL) {
            printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        }

        if (timerData->timeRemaining == 0) {
        SDL_DestroyTexture(toDisplay.texture);
        }

        SDL_FreeSurface(surface);
    }

    drawRectangle(game->renderer, 428, 10, 100, 50);
    SDL_RenderCopy(game->renderer, toDisplay.texture, NULL, &toDisplay.rectangle);
    if (timerData->timeRemaining == 0) {
        SDL_DestroyTexture(toDisplay.texture);
    }
}

bool checkIfTimerHasExpired(Timer *timerData) {
    if (timerData->timeRemaining == 0) {
        printf("The time has run out!\n");
        return true;
    }
    return false;
}


static void drawRectangle(SDL_Renderer *renderer, int x, int y, int w, int h) {
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
}