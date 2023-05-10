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

void handleInput(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
void HuntAndRevive(SDL_Renderer *renderer, Player players[]);

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
    Cargo toSend = {0, 0, 0, 0};
    AddressBook record;
    Network information;

    initialize(&game);
    initiateMapResources(game.renderer, &resources);
    initiateAddressBook(&record);

    /*
    if (number == 3) {
    setUpServer(&information, 2000);
    }
    else {
    setUpClient(&information, "192.168.0.30", 2000);
    } */

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

    manageMenu(&game, &menu);

    changeThemeSong();

    time_t start_time = time(NULL); // Sätt starttiden till nu
    while (!game.quit)
    {
        time_t current_time = time(NULL); // Hämta aktuell tid
        double elapsed_time = difftime(current_time, start_time); // Beräkna tiden som har gått
        /////
        timeAtLoopBeginning = SDL_GetTicks();
        // Handle events
        /*if (number == 3) {
            handleInput(&game, &player3, &hunter, &player1);
            handlePlayerMovement(&player3);
            handlePlayerMovement(&hunter);
        }
        else {*/
            handleInput(&game, &players[0], &players[1], &players[2]);
            handlePlayerMovement(&players[0]);
            handlePlayerMovement(&players[1]);
            handlePlayerMovement(&players[2]);
        //}

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

        // Present the rendered frame
        SDL_RenderPresent(game.renderer);

        /*
        if (number == 1) {
            sendData(&information, &toSend, &player1);
            receiveData(information, &hunter, &player3);
        }
        else if (number == 2) {
            sendData(&information, &toSend, &hunter);
            receiveData(information, &player1, &player3);
        }
        else if (number == 3) {
            manageServerDuties(&information, &record, &player1, &hunter, player3, &toSend);
        } */

        manageFrameRate(timeAtLoopBeginning);
        checkTimeLeft(&game, elapsed_time);
    }

    // Free resources and close SDL
    SDL_DestroyTexture(speedBoostPerk.texture);
    SDL_DestroyWindow(game.window);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}

void handleInput(Framework *game, Player *playerX, Player *playerY,Player *playerZ) {
    while (SDL_PollEvent(&game->event)) {
        if (game->event.type == SDL_QUIT) {
            game->quit = true;
        }
        else if (game->event.type == SDL_KEYDOWN) {
            handleKeyPresses(game, playerX, playerY, playerZ);
        }
        else if (game->event.type == SDL_KEYUP) {
            handleKeyReleases(game, playerX, playerY, playerZ);
        }
    }
}

static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY, Player *playerZ) {
    switch (game->event.key.keysym.sym) {
        case SDLK_UP:
            playerX->up = true;
            break;
        case SDLK_DOWN:
            playerX->down = true;
            break;
        case SDLK_LEFT:
            playerX->left = true;
            break;
        case SDLK_RIGHT:
            playerX->right = true;
            break;
        case SDLK_w:
            playerY->up = true;
            break;
        case SDLK_s:
            playerY->down = true;
            break;
        case SDLK_a:
            playerY->left = true;
            break;
        case SDLK_d:
            playerY->right = true;
            break;
            case SDLK_u:
            playerZ->up = true;
            break;
        case SDLK_j:
            playerZ->down = true;
            break;
        case SDLK_h:
            playerZ->left = true;
            break;
        case SDLK_k:
            playerZ->right = true;
            break;
        case SDLK_ESCAPE:
            game->quit = true;
            break;
        default:
            break;        
    }
}

static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY, Player *playerZ) {
    switch (game->event.key.keysym.sym) {
        case SDLK_UP:
            playerX->up = false;
            break;
        case SDLK_DOWN:
            playerX->down = false;
            break;
        case SDLK_LEFT:
            playerX->left = false;
            break;
        case SDLK_RIGHT:
            playerX->right = false;
            break;
        case SDLK_w:
            playerY->up = false;
            break;
        case SDLK_s:
            playerY->down = false;
            break;
        case SDLK_a:
            playerY->left = false;
            break;
        case SDLK_d:
            playerY->right = false;
            break;
            case SDLK_u:
            playerZ->up = false;
            break;
        case SDLK_j:
            playerZ->down = false;
            break;
        case SDLK_h:
           playerZ->left = false;
            break;
        case SDLK_k:
            playerZ->right = false;
            break;
        default:
            break;
    }
}

void HuntAndRevive(SDL_Renderer *renderer, Player players[]) {
    if (checkCollision(players[0].position, players[1].position)) {
        players[0].speed = 0;
        players[0].frame = 0;
    }
    else if (checkCollision(players[2].position, players[1].position)) {
        players[2].speed = 0;
        players[2].frame = 0;
    }
    else if (checkCollision(players[0].position, players[2].position)) {
        players[0].speed = 2;
        players[0].captured = false;
        players[2].speed = 2;
        players[2].captured = false;
    }
    if (players[0].speed == 0) {
        if (!players[0].captured) {
            playCageSound();
            players[0].captured = true;
        }
        SDL_Texture* cage = IMG_LoadTexture(renderer,"resources/cage.png");
        SDL_Rect cage1;
        cage1.x = (players[0].position.x-7); // -7 så att spelaren blir exakt i mitten av "cage"
        cage1.y = (players[0].position.y-2);
        cage1.w = 40;
        cage1.h = 40;
        SDL_RenderCopy(renderer,cage,NULL,&cage1);
        }
    if (players[2].speed == 0) {
        if (!players[2].captured) {
            playCageSound();
            players[2].captured = true;
        }
        SDL_Texture* cage = IMG_LoadTexture(renderer,"resources/cage.png");
        SDL_Rect cage1;
        cage1.x = (players[2].position.x-7); // -7 så att spelaren blir exakt i mitten av "cage"
        cage1.y = (players[2].position.y-2);
        cage1.w = 40;
        cage1.h = 40;
        SDL_RenderCopy(renderer,cage,NULL,&cage1);
    }
}