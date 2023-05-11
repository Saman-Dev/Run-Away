#include "foundation.h"
#include <time.h>

void initialize(Framework *game) {
    // Initialize SDL, timer and Mixer Library
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));
    
    /* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(1);
	}
    initializeAudio();

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

void checkTimeLeft(Framework *game, double elapsedTime) {
    if (elapsedTime >= TIMER_LENGTH) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, NULL, "Tiden är ute!", game->window);
    game->quit = true;
    }
}