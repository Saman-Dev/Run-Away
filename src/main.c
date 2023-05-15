#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

#include "Entities/perks/perks.h"
#include "Entities/audio/audio.h"
#include "Entities/map/map.h"
#include "Entities/player/player.h"
#include "Entities/network/network.h"
#include "Entities/menu/menu.h"
#include "Entities/foundation/foundation.h"
#include "Entities/mechanics/mechanics.h"

#define WINDOW_WIDTH 900 //1280
#define WINDOW_HEIGHT 560 //960

int main(int argc, char **argv) {
    int timeAtLoopBeginning;
    int number;

    if (argc == 1) {
        number = 1;
    }
    else if (argc == 2) {
        number = 2;
    }
    else if (argc == 3) {
        number = 3;
    }

    Framework game;
    Background resources;
    Player players[5] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GameState state;
    Network information;
    PlayerData toSend = {0, 0, 0, 0, 0, 0};
    AddressBook record;
    TTF_Font* font;
    SDL_Color textColor = { 0, 0, 0 }; // Black color for the text
    SDL_Surface* timerSurface = NULL;
    SDL_Texture* timerTexture = NULL;
    SDL_Rect timerRect;


    game.isMuted = false;
    game.quit = false;

    initialize(&game);
    initiateMapResources(game.renderer, &resources);

    SpeedBoostPerk speedBoostPerk = initializeSpeedBoostPerk(game.renderer);

    players[0] = createPlayer(game.renderer, "resources/Runner_1.png", 1, 200, 200);
    players[1] = createPlayer(game.renderer, "resources/Hunter.png", 2, 142, 280);
    players[2] = createPlayer(game.renderer, "resources/Runner_3.png", 3, 200, 400);

    char* options[] = {"Host Game", "Join Game", "Quit"};
    Menu menu = {
        .options = options,
        .numOptions = 3,
        .optionWidth = 200,
        .optionHeight = 50,
        .optionSpacing = 10,
        .menuX = 480,
        .menuY = 477,
    };

    int selectedOption;

    state = START;

    changeThemeSong();

    time_t start_time = time(NULL); // Sätt starttiden till nu
    
    if (TTF_Init() == -1) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        // Handle error
    }

    font = TTF_OpenFont("resources/font.ttf", 28); // Adjust the path and font size as needed
    if (font == NULL) {
        printf("TTF_OpenFont error: %s\n", TTF_GetError());
        // Handle error
    }

    if (font == NULL) {
        printf("TTF_OpenFont error: %s\n", TTF_GetError());
        // Handle error
    }

    while (!game.quit)
    {   
        time_t current_time = time(NULL); // Hämta aktuell tid
        double elapsed_time = difftime(current_time, start_time); // Beräkna tiden som har gått
        timeAtLoopBeginning = SDL_GetTicks();

        switch (state)
        {
            case ONGOING:
                // Handle events
                handleInput(&game, &players[0], &players[1], &players[2]);
                handlePlayerMovement(&players[0]);
                handlePlayerMovement(&players[1]);
                handlePlayerMovement(&players[2]);

                // Check for perk collision
                applySpeedBoostPerk(&players[0], &speedBoostPerk);
                applySpeedBoostPerk(&players[1], &speedBoostPerk);

                // Game renderer
                SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(game.renderer);
                renderBackground(game.renderer, resources);

                // Render players
                renderPlayers(game, players);

                // Perk render
                renderSpeedBoostPerk(game.renderer, speedBoostPerk);
                HuntAndRevive(game.renderer, players);
                
                //
                SDL_RenderCopy(game.renderer, timerTexture, NULL, &timerRect);

                // Present the rendered frame
                SDL_RenderPresent(game.renderer);

                if (selectedOption == 0) {
                    manageServerDuties(&information, &record, players, &toSend);
                }
                // Calculate elapsed time in seconds
                int elapsedSeconds = (int)elapsed_time;

                // Create timer string
                char timerText[10];
                snprintf(timerText, sizeof(timerText), "%02d:%02d", elapsedSeconds / 60, elapsedSeconds % 60);

                // Render timer text to a surface
                timerSurface = TTF_RenderText_Solid(font, timerText, textColor);
                if (timerSurface == NULL) {
                    printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
                    // Handle error
                }

                // Create a texture from the surface
                timerTexture = SDL_CreateTextureFromSurface(game.renderer, timerSurface);
                if (timerTexture == NULL) {
                    printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
                    // Handle error
                }

                // Set the destination rectangle for rendering the timer texture
                timerRect.x = 200; // Adjust the position as needed
                timerRect.y = 200; // Adjust the position as needed
                timerRect.w = timerSurface->w;
                timerRect.h = timerSurface->h;

                // Free the timer surface since it's not needed anymore
                SDL_FreeSurface(timerSurface);

                
                if (number == 1) {
                    sendData(&information, &toSend, &players[0]);
                    receiveData(&information, players);
                }else if (number == 2) {
                    sendData(&information, &toSend, &players[1]);
                    receiveData(&information, players);
                }

                manageFrameRate(timeAtLoopBeginning);
                checkTimeLeft(&game, elapsed_time);
                break;
            case GAME_OVER:
                printf(
                    "GAME_OVER"
                );
                break;
            case START:
                printf(
                    "START\n"
                );
                selectedOption = manageMenu(&game, &menu, &information, &state, &record);
                if (selectedOption == 0) {
                    number = 0;
                }
                break;
            default:
                break;
        }
    }

    // Free resources and close SDL
    SDL_DestroyTexture(speedBoostPerk.texture);
    SDL_DestroyWindow(game.window);
    Mix_CloseAudio();
    SDL_Quit();
    SDL_DestroyTexture(timerTexture);
    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}