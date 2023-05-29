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
    Framework game = { NULL, NULL, NULL, 0, false, false, false , false, false, false, false};
    Background resources;
    Player players[6] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    Timer timerData = { 0, 0, 0, 0 };
    NetworkBundle networkData = { {NULL, 0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, NULL, 0}, {NULL, 0} };

    initialize(&game);
    initiateMapResources(game.renderer, &resources);
    loadPlayers(game.renderer, players);

    Perk speedBoostPerk = initializePerk(game.renderer, 1);
    Perk freezPerk = initializePerk(game.renderer, 2);
    Perk reverseKeysPerk = initializePerk(game.renderer, 3);
    Camera camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    while (!game.quit) {
        //int randomPlayerIndex = (rand() % MAX_PLAYERS);
        //printf("Plyer :%d \n", randomPlayerIndex);
        timeAtLoopBeginning = SDL_GetTicks();
        if (game.menuState) {
            manageMenu(&game, &networkData, players);
            timerData.timeWhenStarting = time(NULL);
        }
        else {
            // Handle events
            handleInput(&game, players, networkData.TCPInformation.playerNumber);
            
            handlePlayerMovement(players, &camera);

            manageCameraAngle(&camera, players, networkData.TCPInformation.playerNumber);

            renderBackground(&game, resources, &camera);

            // Render players
            renderPlayers(game, players, camera);

            // Check for perk collision
            applyPerk(players, &freezPerk, game.renderer, &camera);
            applyPerk(players, &speedBoostPerk, game.renderer, &camera);
            applyPerk(players, &reverseKeysPerk, game.renderer, &camera);
            checkPerkRespawn(&speedBoostPerk);
            checkPerkRespawn(&freezPerk);
            checkPerkRespawn(&reverseKeysPerk);

            // Perk render
            renderPerk(game.renderer, &speedBoostPerk, &camera);
            renderPerk(game.renderer, &freezPerk, &camera);
            renderPerk(game.renderer, &reverseKeysPerk, &camera);
            HuntAndRevive(game.renderer, players, &camera);

            manageTimer(&game, &timerData);

            // Present the rendered frame
            SDL_RenderPresent(game.renderer);

            manageNetwork(&networkData, players);

            manageGameOverConditions(&game, &timerData, players);

            manageFrameRate(timeAtLoopBeginning);
        }
    }

    // Free resources and close SDL
    SDL_DestroyTexture(speedBoostPerk.texture);
    SDL_DestroyTexture(freezPerk.texture);
    SDL_DestroyTexture(reverseKeysPerk.texture);
    SDL_DestroyWindow(game.window);
    TTF_CloseFont(game.font);
    Mix_CloseAudio();
    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}