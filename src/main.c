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

int main(int argc, char **argv) {
    int timeAtLoopBeginning;
    TCPLocalInformation TCPInformation = {0, 0, NULL, 0};
    TCPClientInformation client[MAX_CLIENTS] = {NULL, 0};
    Framework game;
    Background resources;
    Player players[5] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GameState state;
    Network information;
    PlayerData toSend = {0, 0, 0, 0, 0};
    ClientID record[MAX_CLIENTS];
    Timer timerData = {0, 0, 0, 0};

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

    char* menuOptions[] = {"Host Game", "Join Game", "Settings", "Quit"};
    char* lobbyOptions[] = {"Not connected", "Not connected", "Not connected", "Not connected", "Play", "Back"};
    char* settingsOptions[] = {"Mute Game", "Back to Menu"};

    Menu menu = {
        .optionWidth = 200,
        .optionHeight = 50,
        .menuX = 480,
        .menuY = 477,
    };

    while (!game.quit) {
        timeAtLoopBeginning = SDL_GetTicks();
        switch (state) {
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
                //SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //SDL_RenderClear(game.renderer);
                renderBackground(game.renderer, resources);

                // Render players
                renderPlayers(game, players);

                // Perk render
                renderSpeedBoostPerk(game.renderer, speedBoostPerk);
                HuntAndRevive(game.renderer, players);

                manageTimer(&game, &timerData);

                // Present the rendered frame
                SDL_RenderPresent(game.renderer);

                if (selectedOption == 0) {
                    manageServerDuties(&information, record, players, &toSend);
                    manageServerTCPActivity(&TCPInformation, client, record);
                }
                else {
                    manageUDPClientConnection(&information, &toSend, players, TCPInformation.playerNumber);
                }

                manageFrameRate(timeAtLoopBeginning);
                break;
            case GAME_OVER:
                printf("GAME_OVER\n");
                break;
            case START:
                printf("START\n");

                menu.options = menuOptions;
                menu.numOptions = 4;
                menu.optionSpacing = 30;
                strcpy(menu.img, "resources/start_menu.png");

                selectedOption = manageMenu(&game, &menu, &information, &state, record);
                if (selectedOption == 0) {
                    initiateServerTCPCapability(&TCPInformation);
                    TCPInformation.playerNumber = -1;
                    changeThemeSong();
                }
                else if (selectedOption == 1){
                    InitiateClientTCPCapability(&TCPInformation);
                    changeThemeSong();
                }
                timerData.timeWhenStarting = time(NULL);
                break;
            case LOBBY:
                printf("LOBBY\n");

                menu.options = lobbyOptions;
                menu.numOptions = 6;
                menu.optionSpacing = 55;
                menu.menuX = 280;
                strcpy(menu.img, "resources/lobby_menu.png");

                selectedOption = manageMenu(&game, &menu, &information, &state, record);
                break;
            case SETTINGS:
                printf("SETTINGS\n");

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
    TTF_CloseFont(game.font);
    Mix_CloseAudio();
    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}