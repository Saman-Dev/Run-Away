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

typedef struct {
    bool active;
    SDL_Texture* texture;
    SDL_Rect rectangle;
} Image;

#define WINDOW_WIDTH 900//1280
#define WINDOW_HEIGHT 560//960

void handleInput(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY, Player *playerZ);

void HuntAndRevive(SDL_Renderer *renderer, Player players[]);
static void checkCapturedStatus(Player players[]);
static void handleCage(SDL_Renderer *renderer, Image *cage, Player players[]);

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

    game.isMuted = false;
    game.quit = false;

    initialize(&game);
    initiateMapResources(game.renderer, &resources);
    initiateAddressBook(&record);

    SpeedBoostPerk speedBoostPerk = initializeSpeedBoostPerk(game.renderer);

    players[0] = createPlayer(game.renderer, "resources/Hunter.png", 2, 142, 280);
    players[1] = createPlayer(game.renderer, "resources/Runner_1.png", 1, 200, 200);
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

    if (selectedOption == 0) {
        number = 0;
    }

    changeThemeSong();

    time_t start_time = time(NULL); // Sätt starttiden till nu

    while (!game.quit)
    {   
        time_t current_time = time(NULL); // Hämta aktuell tid
        double elapsed_time = difftime(current_time, start_time); // Beräkna tiden som har gått
        timeAtLoopBeginning = SDL_GetTicks();

        switch (state)
        {
            case ONGOING:
                // Handle events
                handleInput(&game, &players[1], &players[0], &players[2]);
                handlePlayerMovement(&players[1]);
                handlePlayerMovement(&players[0]);
                handlePlayerMovement(&players[2]);

                // Check for perk collision
                applySpeedBoostPerk(&players[1], &speedBoostPerk);
                applySpeedBoostPerk(&players[0], &speedBoostPerk);

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

                if (selectedOption == 0) {
                    manageServerDuties(&information, &record, &players[1], &players[0], &players[2], &toSend);
                } 
                
                
                if (number == 1) {
                    sendData(&information, &toSend, &players[1]);
                    receiveData(&information, &players[1], &players[0], &players[2]);
                }else if (number == 2) {
                    sendData(&information, &toSend, &players[0]);
                    receiveData(&information, &players[1], &players[0], &players[2]);
                }else if (number == 3) {
                    sendData(&information, &toSend, &players[2]);
                    receiveData(&information, &players[1], &players[0], &players[2]);
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
                selectedOption = manageMenu(&game, &menu, &information, &state);
                break;
        }
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
        case SDLK_m:
            game->isMuted = !game->isMuted;
            if (game->isMuted) {
                Mix_VolumeMusic(0);
            } else {
                Mix_VolumeMusic(MIX_MAX_VOLUME);
            }
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
    static Image cage = {0, NULL, {0, 0, 40, 40}};
    checkCapturedStatus(players);
    handleCage(renderer, &cage, players);
}

static void handleCage(SDL_Renderer *renderer, Image *cage, Player players[]) {
    for (int i = 0; players[i].player != 0; i++) {
        if (players[i].speed == 0) {
            if (!cage->active) {
                cage->texture = IMG_LoadTexture(renderer,"resources/cage.png");
                cage->active = true;
            }
            if (players[i].captured) {
                cage->rectangle.x = (players[i].position.x-7); // -7 så att spelaren blir exakt i mitten av "cage"
                cage->rectangle.y = (players[i].position.y-2);
                SDL_RenderCopy(renderer, cage->texture, NULL, &cage->rectangle);
            }
        }
    }
    
    if (cage->active) {
        int numberOfCapturedPlayers;
        for (int i = 0; players[i].player != 0; i++) {
            if (players[i].speed == 0) {
                numberOfCapturedPlayers++;
            }
        }
        if (numberOfCapturedPlayers == 0) {
            SDL_DestroyTexture(cage->texture);
            cage->active = false;
        }
    }
}

static void checkCapturedStatus(Player players[]) {
    for (int i = 0; players[i].player != 0; i++) {
        if (players[i].player == 2) {
            continue;
        }
        else {
            if (checkCollision(players[i].position, players[0].position) && players[i].captured == false) {
                playCageLockSound();
                players[i].captured = true;
                players[i].speed = 0;
                players[i].frame = 1;
            }
            else if (checkCollision(players[1].position, players[2].position)) {
                if (players[1].captured == true || players[2].captured == true) {
                    playCageUnlockSound();
                    players[1].speed = 2;
                    players[1].captured = false;
                    players[2].speed = 2;
                    players[2].captured = false;
                }
            }
        }
    }
}