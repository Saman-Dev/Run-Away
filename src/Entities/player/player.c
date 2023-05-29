#include "player.h"

static Player createPlayer(SDL_Renderer *renderer, char playerModel[], int playerNumber, int positionX, int positionY) {
    Player playerX;

    playerX.spriteSheetTexture = IMG_LoadTexture(renderer, playerModel);
    if (playerX.spriteSheetTexture == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }

    int frame_count = 0;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            playerX.spriteClip[frame_count].x = x * (PLAYER_FRAME_HEIGHT)+1;
            playerX.spriteClip[frame_count].y = y * (PLAYER_FRAME_WIDTH)+3;
            playerX.spriteClip[frame_count].w = PLAYER_FRAME_WIDTH;
            playerX.spriteClip[frame_count].h = PLAYER_FRAME_HEIGHT;
            frame_count++;
        }
    }

    playerX.down = false;
    playerX.up = false;
    playerX.left = false;
    playerX.right = false;

    playerX.position.x = positionX;
    playerX.position.y = positionY;
    playerX.position.w = PLAYER_FRAME_WIDTH;
    playerX.position.h = PLAYER_FRAME_HEIGHT;

    playerX.frame = 6;
    playerX.speed = Default_Speed;
    // playerX.speedBoostStartTime = 0;
    // playerX.baseSpeed = 2;
    // playerX.boostedSpeed = 4;

    playerX.player = playerNumber;
    playerX.captured = false;
    playerX.frozen = false;
    playerX.movementKeysReversed = false;
    return playerX;
}

void handlePlayerMovement(Player player[], Camera *camera) {
    for (int i = 0; player[i].player != 0; i++) {
        if (!player[i].captured) {
            if (player[i].up) {
                if (!encountersForbiddenTile(player[i].position.x + (PLAYER_FRAME_WIDTH / 2), player[i].position.y)) {
                    playWalkingSound();
                    // printf("Player: Up\n");
                    // printf("X: %d, Y: %d\n", playerX->position.x, playerX->position.y);
                    player[i].position.y -= player[i].speed;
                    if (player[i].frame == 9 || player[i].frame == 10)
                        player[i].frame++;
                    else
                        player[i].frame = 9;
                }
            }
            else if (player[i].down) {
                if (!encountersForbiddenTile(player[i].position.x + (PLAYER_FRAME_WIDTH / 2), player[i].position.y + PLAYER_FRAME_HEIGHT)) {
                    playWalkingSound();
                    // printf("Player: Down\n");
                    // printf("X: %d, Y: %d\n", playerX->position.x, playerX->position.y);
                    player[i].position.y += player[i].speed;
                    if (player[i].frame == 0 || player[i].frame == 1)
                        player[i].frame++;
                    else
                        player[i].frame = 0;
                }
            }
            if (player[i].left) {
                if (!encountersForbiddenTile(player[i].position.x, player[i].position.y + (PLAYER_FRAME_HEIGHT / 2))) {
                    playWalkingSound();
                    // printf("Player: Left\n");
                    // printf("X: %d, Y: %d\n", playerX->position.x, playerX->position.y);
                    player[i].position.x -= player[i].speed;
                    if (player[i].frame == 3 || player[i].frame == 4)
                        player[i].frame++;
                    else
                        player[i].frame = 3;
                }
            }
            else if (player[i].right) {
                if (!encountersForbiddenTile(player[i].position.x + PLAYER_FRAME_WIDTH, player[i].position.y + (PLAYER_FRAME_HEIGHT / 2))) {
                    playWalkingSound();
                    // printf("Player: Right\n");
                    // printf("X: %d, Y: %d", playerX->position.x, playerX->position.y);
                    player[i].position.x += player[i].speed;
                    if (player[i].frame == 6 || player[i].frame == 7)
                        player[i].frame++;
                    else
                        player[i].frame = 6;
                }
            }
        }
    }
}

void renderPlayers(Framework game, Player players[], Camera camera) {
    for (int i = 0; players[i].player != 0; i++) {
        SDL_Rect playerPosition = players[i].position;
        playerPosition.x -= camera.x;
        playerPosition.y -= camera.y;
        SDL_RenderCopyEx(game.renderer, players[i].spriteSheetTexture, &players[i].spriteClip[players[i].frame], &playerPosition, 0, NULL, SDL_FLIP_NONE);
    }
}

static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY, Player *playerZ) {
    SDL_Keycode key = game->event.key.keysym.sym;

    if (playerX->movementKeysReversed) {
        // Reverse movement keys for playerX
        if (key == SDLK_UP) {
            key = SDLK_DOWN;
        }
        else if (key == SDLK_DOWN) {
            key = SDLK_UP;
        }
        else if (key == SDLK_LEFT) {
            key = SDLK_RIGHT;
        }
        else if (key == SDLK_RIGHT) {
            key = SDLK_LEFT;
        }
    }

    if (playerY->movementKeysReversed) {
        // Reverse movement keys for playerY
        if (key == SDLK_w) {
            key = SDLK_s;
        }
        else if (key == SDLK_s) {
            key = SDLK_w;
        }
        else if (key == SDLK_a) {
            key = SDLK_d;
        }
        else if (key == SDLK_d) {
            key = SDLK_a;
        }
    }

    if (playerZ->movementKeysReversed) {
        // Reverse movement keys for playerZ
        if (key == SDLK_u) {
            key = SDLK_j;
        }
        else if (key == SDLK_j) {
            key = SDLK_u;
        }
        else if (key == SDLK_h) {
            key = SDLK_k;
        }
        else if (key == SDLK_k) {
            key = SDLK_h;
        }
    }

    switch (key) {
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
        }
        else {
            Mix_VolumeMusic(MIX_MAX_VOLUME);
        }
        break;
    case SDLK_ESCAPE:
        game->isPaused = true;
        game->menuState = true;
        break;
    default:
        break;
    }
}

static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY, Player *playerZ) {
    SDL_Keycode key = game->event.key.keysym.sym;

    if (playerX->movementKeysReversed) {
        // Reverse movement keys for playerX
        if (key == SDLK_UP) {
            key = SDLK_DOWN;
        }
        else if (key == SDLK_DOWN) {
            key = SDLK_UP;
        }
        else if (key == SDLK_LEFT) {
            key = SDLK_RIGHT;
        }
        else if (key == SDLK_RIGHT) {
            key = SDLK_LEFT;
        }
    }

    if (playerY->movementKeysReversed) {
        // Reverse movement keys for playerY
        if (key == SDLK_w) {
            key = SDLK_s;
        }
        else if (key == SDLK_s) {
            key = SDLK_w;
        }
        else if (key == SDLK_a) {
            key = SDLK_d;
        }
        else if (key == SDLK_d) {
            key = SDLK_a;
        }
    }

    if (playerZ->movementKeysReversed) {
        // Reverse movement keys for playerZ
        if (key == SDLK_u) {
            key = SDLK_j;
        }
        else if (key == SDLK_j) {
            key = SDLK_u;
        }
        else if (key == SDLK_h) {
            key = SDLK_k;
        }
        else if (key == SDLK_k) {
            key = SDLK_h;
        }
    }

    switch (key) {
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

void handleInput(Framework *game, Player player[], int playerNumber) {
    if (playerNumber == -1) {
        playerNumber = 4;
    }
    while (SDL_PollEvent(&game->event)) {
        if (game->event.type == SDL_QUIT) {
            game->quit = true;
        }
        else if (game->event.type == SDL_KEYDOWN) {
            handleKeyPresses(game, &player[playerNumber], &player[playerNumber], &player[playerNumber]);
        }
        else if (game->event.type == SDL_KEYUP) {
            handleKeyReleases(game, &player[playerNumber], &player[playerNumber], &player[playerNumber]);
        }
    }
}

void loadPlayers(SDL_Renderer *renderer, Player players[]) {
    players[0] = createPlayer(renderer, "resources/Runner_1.png", 1, 820, 460);
    players[1] = createPlayer(renderer, "resources/Hunter.png", 2, 1300, 780);
    players[2] = createPlayer(renderer, "resources/Runner_2.png", 3, 900, 1070);
    players[3] = createPlayer(renderer, "resources/Runner_3.png", 4, 2050, 1030);
    players[4] = createPlayer(renderer, "resources/Hunter_2.png", 5, 2050, 1070);
}

void manageCameraAngle(Camera *camera, Player players[], int playerNumber) {
    for (int i = -1; i < 5; i++) {
        if (i == playerNumber && playerNumber == -1) {
            camera->x = players[4].position.x - SCREEN_WIDTH / 2;
            camera->y = players[4].position.y - SCREEN_HEIGHT / 2;
            break;
        }
        else if (i == playerNumber) {
            camera->x = players[i].position.x - SCREEN_WIDTH / 2;
            camera->y = players[i].position.y - SCREEN_HEIGHT / 2;
            break;
        }
    }
}