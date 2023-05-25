#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

#include "Entities/audio/audio.h"
#include "Entities/foundation/foundation.h"
#include "Entities/map/map.h"
#include "Entities/mechanics/mechanics.h"
#include "Entities/menu/menu.h"
#include "Entities/network/network.h"
#include "Entities/perks/perks.h"
#include "Entities/player/player.h"

int main(int argc, char **argv) {
    int timeAtLoopBeginning;
    Framework game = { NULL, NULL, NULL, 0, false, false, false};
    Background resources;
    Player players[5] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    Timer timerData = { 0, 0, 0, 0 };
    NetworkBundle networkData = {{NULL, 0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, NULL, 0}, {NULL, 0}};

    initialize(&game);
    initiateMapResources(game.renderer, &resources);
    loadPlayers(game.renderer, players);

    Perk speedBoostPerk = initializePerk(game.renderer, 1);
    Perk freezPerk = initializePerk(game.renderer, 2);

    while (!game.quit) {
        timeAtLoopBeginning = SDL_GetTicks();
        if (game.menuState) {
            manageMenu(&game, &networkData.UDPInformation, &networkData.TCPInformation, networkData.UDPRecord);
            timerData.timeWhenStarting = time(NULL);
        }
        else {
            // Handle events
            handleInput(&game, &players[0], &players[1], &players[2]);
            handlePlayerMovement(&players[0]);
            handlePlayerMovement(&players[1]);
            handlePlayerMovement(&players[2]);
            renderBackground(game.renderer, resources);

            // Render players
            renderPlayers(game, players);

            // Check for perk collision
            applyPerk(players, &freezPerk, game.renderer);
            applyPerk(players, &speedBoostPerk, game.renderer);
            checkPerkRespawn(&speedBoostPerk);
            checkPerkRespawn(&freezPerk);

            // Perk render
            renderPerk(game.renderer, &speedBoostPerk);
            renderPerk(game.renderer, &freezPerk);
            HuntAndRevive(game.renderer, players);

            manageTimer(&game, &timerData);

            // Present the rendered frame
            SDL_RenderPresent(game.renderer);

            manageNetwork(&networkData, players);

            manageFrameRate(timeAtLoopBeginning);
        }
    }

    // Free resources and close SDL
    SDL_DestroyTexture(speedBoostPerk.texture);
    SDL_DestroyTexture(freezPerk.texture);
    SDL_DestroyWindow(game.window);
    TTF_CloseFont(game.font);
    Mix_CloseAudio();
    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}

    //char *menuOptions[] = { "Host Game", "Join Game", "Settings", "Quit", "\0"};
    //char *lobbyOptions[] = { "Not connected", "Not connected", "Not connected", "Not connected", "Play", "Back", "\0"};
    //char *settingsOptions[] = { "Mute Game", "Back to Menu", "\0"};
    //char *gameOverOptions[] = { "Back to Menu", "Quit", "\0"};

/*            break;
        case GAME_OVER:
            printf("GAME_OVER\n");
            menu.options = gameOverOptions;
            menu.optionSpacing = 67;
            menu.menuX = 713,
                menu.menuY = 600;
            strcpy(menu.imageFilePath, "resources/game_over_hunters_lose_menu.png");

            selectedOption = manageMenu(&game, &menu, &information, &TCPInformation, &state, record);
            break;
        case START:
            printf("START\n");

            menu.options = menuOptions;
            menu.optionSpacing = 60;
            menu.menuX = 730,
                menu.menuY = 425;
            strcpy(menu.imageFilePath, "resources/start_menu.png");

            selectedOption = manageMenu(&game, &menu, &information, &TCPInformation, &state, record);
            if (selectedOption == 0) {
                initiateServerTCPCapability(&TCPInformation);
                TCPInformation.playerNumber = -1;
                changeThemeSong();
            }
            else if (selectedOption == 1) {
                InitiateClientTCPCapability(&TCPInformation);
                changeThemeSong();
            }
            timerData.timeWhenStarting = time(NULL);
            break;
        case LOBBY:
            printf("LOBBY\n");

            menu.options = lobbyOptions;
            menu.optionSpacing = 55;
            menu.menuX = 400,
                menu.menuY = 425;
            strcpy(menu.imageFilePath, "resources/lobby_menu.png");

            selectedOption = manageMenu(&game, &menu, &information, &TCPInformation, &state, record);
            break;
        case SETTINGS:
            printf("SETTINGS\n");

            menu.options = settingsOptions;
            menu.optionSpacing = 240;
            menu.menuX = 710,
                menu.menuY = 425;
            strcpy(menu.imageFilePath, "resources/settings_menu.png");

            selectedOption = manageMenu(&game, &menu, &information, &TCPInformation, &state, record);
            break;
        default:
            break;
        }*/