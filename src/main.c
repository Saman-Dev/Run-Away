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

void manageCameraAngle(Camera *camera, Player players[], int playerNumber);
void manageGameOverConditions(Framework *game, Timer *timerData, Player players[]);

int main(int argc, char **argv) {
    int timeAtLoopBeginning;
    Framework game = { NULL, NULL, NULL, 0, false, false, false , false, false, false, false};
    Background resources;
    Player players[5] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
            handleInput(&game, &players[0], &players[1], &players[2]);
            handlePlayerMovement(&players[0], &camera);
            handlePlayerMovement(&players[1], &camera);
            handlePlayerMovement(&players[2], &camera);

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

void manageCameraAngle(Camera *camera, Player players[], int playerNumber) {
    /*
    for (int i = 0; i < MAX_CLIENTS; i++) {

        if ((i) == (playerNumber + 1)) {
            camera->x = players[i].position.x - SCREEN_WIDTH / 2;
            camera->y = players[i].position.y - SCREEN_HEIGHT / 2;
            break;
        }
    }
    */
    if (playerNumber == 0) {
        camera->x = players[0].position.x - SCREEN_WIDTH / 2;
        camera->y = players[0].position.y - SCREEN_HEIGHT / 2;
    }
    else if (playerNumber == 1) {
        camera->x = players[1].position.x - SCREEN_WIDTH / 2;
        camera->y = players[1].position.y - SCREEN_HEIGHT / 2;
    }
    else {
        camera->x = players[2].position.x - SCREEN_WIDTH / 2;
        camera->y = players[2].position.y - SCREEN_HEIGHT / 2;
    }
}

void manageGameOverConditions(Framework *game, Timer *timerData, Player players[]) {
    int captured_players = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if(players[i].captured){
            captured_players++;
        }
    }

    if (game->gameOver || captured_players == 2) {
        displayGameOverScreen(game, timerData);
    }
}