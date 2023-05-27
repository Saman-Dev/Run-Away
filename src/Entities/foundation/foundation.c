#include "foundation.h"

void initialize(Framework *game) {
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Set initial game state
    game->menuState = true;
    game->gameOver = false;
    game->showGameOverScreen = false;
    game->goBackToMenu = false;

    // Initialize audio
    initializeAudio();

    // Seed random number generator
    srand(time(NULL));

    // Initialize SDL_image library
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        exit(1);
    }

    // Initialize SDL_mixer library
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Failed to initialize SDL_mixer: %s\n", Mix_GetError());
        exit(1);
    }

    // Initialize SDL_net library
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(1);
    }

    // Initialize SDL_ttf library
    if (TTF_Init() == -1) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        exit(1);
    }

    // Load font
    game->font = TTF_OpenFont("resources/font.ttf", 24);
    if (game->font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }

    // Create game window
    game->window = SDL_CreateWindow("RUN AWAY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (game->window == NULL) {
        printf("Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    // Create renderer
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (game->renderer == NULL) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    // Set font style
    TTF_SetFontStyle(game->font, TTF_STYLE_BOLD);

    // Set default color
    game->white.r = 255;
    game->white.g = 255;
    game->white.b = 255;
    game->white.a = 0;
}

void manageFrameRate(int timeAtLoopBeginning) {
    int endOfLoopTime = SDL_GetTicks() - timeAtLoopBeginning;
    if (endOfLoopTime < (1000 / FPS)) {
        SDL_Delay((1000 / FPS) - endOfLoopTime);
    }
}

bool manageTimer(Framework *game, Timer *timerData) {
    calculateRemainingTime(timerData);
    displayTime(game, timerData);
    if (checkIfTimerHasExpired(timerData, game)) {
        return true;
    }
    return false;
}

static void calculateRemainingTime(Timer *timerData) {
    time_t currentTime = time(NULL);
    int timeElapsed = (int) difftime(currentTime, timerData->timeWhenStarting);

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
    static Image toDisplay = {0, NULL, {0, 0, 91, 35}};
    toDisplay.rectangle.x = (SCREEN_WIDTH - toDisplay.rectangle.w) / 2;
    toDisplay.rectangle.y = 15;
    static int lastSeenTimeRemainingValue = 0;
    char numbersToPrint[8];

    if (lastSeenTimeRemainingValue != timerData->timeRemaining) {
        lastSeenTimeRemainingValue = timerData->timeRemaining;

        snprintf(numbersToPrint, sizeof(numbersToPrint), "%02d:%02d", timerData->minutesRemaining, timerData->secondsRemaining);

        SDL_Surface *surface = TTF_RenderText_Solid(game->font, numbersToPrint, game->white);
        if (surface == NULL) {
            printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        }

        if (toDisplay.texture != NULL) {
            SDL_DestroyTexture(toDisplay.texture);
        }

        toDisplay.texture = SDL_CreateTextureFromSurface(game->renderer, surface);
        if (toDisplay.texture == NULL) {
            printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(surface);
    }

    drawRectangle(game->renderer, 590, 10, 100, 50);
    SDL_RenderCopy(game->renderer, toDisplay.texture, NULL, &toDisplay.rectangle);

    if (timerData->timeRemaining == 0) {
        SDL_DestroyTexture(toDisplay.texture);
    }
}

bool checkIfTimerHasExpired(Timer *timerData, Framework *game) {
    if (timerData->timeRemaining == 0) {
        printf("The time has run out!\n");
        handleGameOver(game); // Call the game over function when time is 0
        return true;
    }
    return false;
}

static void drawRectangle(SDL_Renderer *renderer, int x, int y, int w, int h) {
    SDL_Rect rectangle = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
}

void handleGameOver(Framework *game) {
    game->gameOver = true; // Set the game over flag to true
}

void displayGameOverScreen(Framework *game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    // Load the game over image
    SDL_Surface *gameOverSurface = IMG_Load("resources/game_over_hunters_lose_menu.png");
    if (!gameOverSurface) {
        printf("Failed to load game over image: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *gameOverTexture = SDL_CreateTextureFromSurface(game->renderer, gameOverSurface);
    if (!gameOverTexture) {
        printf("Failed to create texture from game over surface: %s\n", SDL_GetError());
        SDL_FreeSurface(gameOverSurface);
        return;
    }

    SDL_Rect gameOverRect = {(SCREEN_WIDTH - gameOverSurface->w) / 2, (SCREEN_HEIGHT - gameOverSurface->h) / 2, gameOverSurface->w, gameOverSurface->h};
    SDL_RenderCopy(game->renderer, gameOverTexture, NULL, &gameOverRect);

    // Render the "Go Back to Menu" option
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *optionSurface = TTF_RenderText_Solid(game->font, "Go Back to Menu", textColor);
    SDL_Texture *optionTexture = SDL_CreateTextureFromSurface(game->renderer, optionSurface);
    SDL_Rect optionRect = {(SCREEN_WIDTH - optionSurface->w) / 2, ((SCREEN_HEIGHT + 200) + optionSurface->h) / 2, optionSurface->w, optionSurface->h};
    SDL_RenderCopy(game->renderer, optionTexture, NULL, &optionRect);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= optionRect.x && mouseX < optionRect.x + optionRect.w && mouseY >= optionRect.y && mouseY < optionRect.y + optionRect.h) {
                // Player clicked on "Go Back to Menu" option
                game->goBackToMenu = true;
            }
        }
    }

    if (game->showGameOverScreen) {
        if (game->goBackToMenu) {
            game->menuState = true;
            game->showGameOverScreen = false;
            // Reset any other necessary game state variables here
        }
    }

    SDL_FreeSurface(gameOverSurface);
    SDL_FreeSurface(optionSurface);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(optionTexture);

    SDL_RenderPresent(game->renderer);
}