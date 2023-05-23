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
    TCPLocalInformation TCPInformation = { 0, 0, NULL, 0 };
    TCPClientInformation client[MAX_CLIENTS] = { NULL, 0 };
    Framework game = { NULL, NULL, NULL, 0, false, false };
    Background resources;
    Player players[5] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    GameState state;
    Network information;
    PlayerData toSend = { 0, 0, 0, 0, 0 };
    ClientID record[MAX_CLIENTS];
    Timer timerData = { 0, 0, 0, 0 };

    initialize(&game);
    initiateMapResources(game.renderer, &resources);

    Perk speedBoostPerk = initializePerk(game.renderer, 1);
    Perk freezPerk = initializePerk(game.renderer, 2);

    players[0] = createPlayer(game.renderer, "resources/Runner_1.png", 1, 300, 300);
    players[1] = createPlayer(game.renderer, "resources/Hunter.png", 2, 242, 280);
    players[2] = createPlayer(game.renderer, "resources/Runner_3.png", 3, 300, 400);

    int selectedOption;

    state = START;

    char *menuOptions[] = { "Host Game", "Join Game", "Settings", "Quit", "\0"};
    char *lobbyOptions[] = { "Not connected", "Not connected", "Not connected", "Not connected", "Play", "Back", "\0"};
    char *settingsOptions[] = { "Mute Game", "Back to Menu", "\0"};
    char *gameOverOptions[] = { "Back to Menu", "Quit", "\0"};

    Menu menu;

    while (!game.quit) {
        timeAtLoopBeginning = SDL_GetTicks();
        switch (state) {
        case ONGOING:
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

            if (manageTimer(&game, &timerData)) {
                state = GAME_OVER;
            };

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