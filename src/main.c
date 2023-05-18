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
    TCPLocalInformation TCPInformation = {0, 0, NULL, 0};
    TCPClientInformation client[MAX_CLIENTS] = {NULL, 0};

    int timeAtLoopBeginning;

    Framework game;
    Background resources;
    Player players[5] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GameState state;
    Network information;
    PlayerData toSend = {0, 0, 0, 0, 0};
    ClientID record[MAX_CLIENTS];
    
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

    int selectedOption;

    state = START;

    changeThemeSong();
    
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

    time_t start_time = time(NULL); // Set start time 

    char* menuOptions[] = {"Host Game", "Join Game", "Settings", "Quit"};
    char* settingsOptions[] = {"Mute Game", "Back to Menu"};
    Menu menu = {
        .optionWidth = 200,
        .optionHeight = 50,
        .menuX = 480,
        .menuY = 477,
    };

    while (!game.quit)
    {   
        time_t current_time = time(NULL); // Hämta aktuell tid
        double elapsed_time = (1 * 60) - difftime(current_time, start_time); // Den här funktion är ansvarig för hur många minuter en timer har
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
                
                // Timer render
                SDL_RenderCopy(game.renderer, timerTexture, NULL, &timerRect);

                // Present the rendered frame
                SDL_RenderPresent(game.renderer);

                if (selectedOption == 0) {
                    manageServerDuties(&information, record, players, &toSend);
                    //manageServerTCPActivity(&TCPInformation, client);
                }
                // Calculate elapsed time in seconds
                int elapsedSeconds = (int)elapsed_time;

                // Create timer string
                int remainingMinutes = (int)elapsed_time / 60;
                int remainingSeconds = (int)elapsed_time % 60;
                char timerText[10];
                snprintf(timerText, sizeof(timerText), "%02d:%02d", remainingMinutes, remainingSeconds);

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
                if (remainingMinutes == 0 &&  remainingSeconds == 0){
                    printf("Game over!");
                    break; 
                }
                
                if (TCPInformation.playerNumber == 0) {
                    sendData(&information, &toSend, &players[0]);
                    receiveData(&information, players);
                }else if (TCPInformation.playerNumber == 1) {
                    sendData(&information, &toSend, &players[1]);
                    receiveData(&information, players);
                }

                manageFrameRate(timeAtLoopBeginning);
                checkTimeLeft(&game, elapsed_time);
                break;
            case GAME_OVER:
                printf(
                    "GAME_OVER\n"
                );
                
                break;
            case START:
                printf(
                    "START\n"
                );

                menu.options = menuOptions;
                menu.numOptions = 4;
                menu.optionSpacing = 10;
                strcpy(menu.img, "resources/start_menu.png");

                selectedOption = manageMenu(&game, &menu, &information, &state, record);
                /*
                if (selectedOption == 0) {
                    initiateServerTCPCapability(&TCPInformation);
                    TCPInformation.playerNumber = -1;
                }
                else {
                    InitiateClientTCPCapability(&TCPInformation);
                }
                */
                break;
            case SETTINGS:
                printf(
                    "SETTINGS\n"
                );

                menu.options = settingsOptions;
                menu.numOptions = 2;
                menu.optionSpacing = 60;
                strcpy(menu.img, "resources/settings_menu.png");
                    
                selectedOption = manageMenu(&game, &menu, &information, &state, record);
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