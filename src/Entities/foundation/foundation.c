#include "foundation.h"

void initialize(Framework *game) {
    // Initialize SDL, timer and Mixer Library
    SDL_Init(SDL_INIT_VIDEO);
    initializeAudio();
    srand(time(NULL));
    
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(1);
	}

    if (TTF_Init() == -1) {
        printf("TTF_Init error: %s\n", TTF_GetError());
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

void manageTimer(Framework *game, Timer *timerData) {
    calculateRemainingTime(timerData);
    displayTime(game->renderer, timerData);
    checkIfTimerHasExpired(game, timerData);
}

static void calculateRemainingTime(Timer *timerData) {
    time_t currentTime = time(NULL);
    timerData->timeRemaining = (2 * 60) - difftime(currentTime, timerData->timeWhenStarting); // Den här funktion är ansvarig för hur många minuter en timer har
    timerData->minutesRemaining = ((int) timerData->timeRemaining / 60);
    timerData->secondsRemaining = ((int) timerData->timeRemaining % 60);
}

static void displayTime(SDL_Renderer *renderer, Timer *timerData) {
    static Image toDisplay = {0, NULL, {430, 15, 91, 35}};
    static TTF_Font *font;
    static SDL_Color color = {0, 0, 0};
    static int lastSeenTimeRemainingValue = 0;
    char numbersToPrint[8];
    SDL_Surface *surface;

    if (lastSeenTimeRemainingValue != timerData->timeRemaining) {
        lastSeenTimeRemainingValue = timerData->timeRemaining;

        snprintf(numbersToPrint, sizeof(numbersToPrint), "%02d:%02d", timerData->minutesRemaining, timerData->secondsRemaining);

        if (!toDisplay.active) {
            font = TTF_OpenFont("resources/font.ttf", 28);
            toDisplay.active = true;
        }

        surface = TTF_RenderText_Solid(font, numbersToPrint, color);
        if (surface == NULL) {
            printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        }

        toDisplay.texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (toDisplay.texture == NULL) {
            printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(surface);
    }

    SDL_RenderCopy(renderer, toDisplay.texture, NULL, &toDisplay.rectangle);
    if (timerData->timeRemaining == 0) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(toDisplay.texture);
    }
}

static void checkIfTimerHasExpired(Framework *game, Timer *timerData) {
    if (timerData->timeRemaining == 0) {
        printf("The time has run out!\n");
        game->quit = true;
    }
}