#include "perks.h"

void applyPerk(Player players[], Perk *perk, SDL_Renderer *renderer, Camera *camera) {
    SDL_Texture *frozenTexture = IMG_LoadTexture(renderer, "resources/cristal1.png");

    if (perk->available) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            Player *player = &players[i];
            if (checkCollision(player->position, perk->rect)) {
                if (perk->perkID == 1) {
                    player->speed += SPEED_BOOST_AMOUNT;
                    perk->available = false;
                    perk->duration = perk_duration;
                    player->hasPerk = true;
                    player->perkStartTime = time(NULL);
                }
                else if (perk->perkID == 2) {
                    for (int j = 0; j < MAX_PLAYERS; j++) {
                        Player *otherPlayer = &players[j];
                        if (i != j) {
                            otherPlayer->frozen = true;
                            otherPlayer->speed = 0;
                        }
                    }
                    perk->available = false;
                    perk->duration = perk_duration;
                    player->hasPerk = true;
                    player->perkStartTime = time(NULL);
                }
                else if (perk->perkID == 3) {
                    for (int i = 0; i < rand() % MAX_PLAYERS; i++) {
                        // Generate a random index
                        int randomIndex = rand() % MAX_PLAYERS;

                        // Apply the movementKeysReversed property to the randomly selected player
                        Player *randomPlayer = &players[randomIndex];
                        randomPlayer->movementKeysReversed = true;
                    }
                    perk->available = false;
                    perk->duration = perk_duration;
                    player->hasPerk = true;
                    player->perkStartTime = time(NULL);
                }

                // Random new spawnpoint for perk
                int randomLocationX;
                int randomLocationY;
                do {
                    randomLocationX = rand() % 2280;
                    randomLocationY = rand() % 720;
                } while (encountersForbiddenTile(randomLocationX, randomLocationY) && encountersForbiddenTile(randomLocationX+20, randomLocationY+20) && encountersForbiddenTile(randomLocationX-20, randomLocationY-20));
                perk->rect.x = randomLocationX;
                perk->rect.y = randomLocationY;
            }
        }
    }
    else {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            Player *player = &players[i];
            if (!perk->available && (&perk->duration > 0) && player->hasPerk) {
                time_t current_time = time(NULL);
                double elapsed_time = difftime(current_time, player->perkStartTime);

                if (elapsed_time >= perk->duration) {
                    perk->duration = 0;
                    perk->respawnTime = time(NULL) + 10;
                    for (int j = 0; j < MAX_PLAYERS; j++) {
                        Player *otherPlayer = &players[j];
                        otherPlayer->movementKeysReversed = false;
                        otherPlayer->frozen = false;
                        otherPlayer->speed = DEFAULT_SPEED;
                    }
                    player->hasPerk = false;

                    // Random new spawnpoint for perk
                    int randomLocationX;
                    int randomLocationY;
                    do {
                        randomLocationX = rand() % 1280;
                        randomLocationY = rand() % 720;
                } while (encountersForbiddenTile(randomLocationX, randomLocationY) || encountersForbiddenTile(randomLocationX+20, randomLocationY+20) || encountersForbiddenTile(randomLocationX-20, randomLocationY-20));
                    perk->rect.x = randomLocationX;
                    perk->rect.y = randomLocationY;
                }
                else {
                    int remaining_time = (int)(perk->duration - elapsed_time);
                    printf("Player %d - Time remaining: %d seconds\n", player->player, remaining_time);
                }

            }
        }
    }
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *player = &players[i];
        if (player->frozen) {
            if (frozenTexture == NULL) {
                printf("Failed to load frozen texture: %s\n", IMG_GetError());
                exit(1);
            }
            SDL_Rect destRect = {
                player->position.x - camera->x,
                player->position.y - camera->y,
                player->position.w,
                player->position.h
            };
            SDL_RenderCopy(renderer, frozenTexture, NULL, &destRect);
        }
    }

    SDL_DestroyTexture(frozenTexture);
}

void renderPerk(SDL_Renderer *renderer, Perk *perk, Camera *camera) {
    if (perk->available) {
        SDL_Rect destRect = {
            perk->rect.x - camera->x,
            perk->rect.y - camera->y,
            perk->rect.w,
            perk->rect.h
        };
        SDL_RenderCopy(renderer, perk->texture, NULL, &destRect);
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b) // check perk/player collision
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}

Perk initializePerk(SDL_Renderer *renderer, int perkNr) {
    Perk speedBoostPerk;
    Perk freezePerk;
    Perk reverseKeysPerk;
    SDL_Texture *perkTextureSpeed = IMG_LoadTexture(renderer, "resources/perk_1.png");
    SDL_Texture *perkTextureFreeze = IMG_LoadTexture(renderer, "resources/perk_2.png");
    SDL_Texture *perkTextureReverse = IMG_LoadTexture(renderer, "resources/perk_3.png");
    if (perkTextureSpeed == NULL || perkTextureFreeze == NULL || perkTextureReverse == NULL) {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    srand(time(NULL));

    speedBoostPerk.texture = perkTextureSpeed;
    freezePerk.texture = perkTextureFreeze;
    reverseKeysPerk.texture = perkTextureReverse;

    srand(time(NULL));
    int randomLocationX[3];
    int randomLocationY[3];

    for (int i = 0; i < 3; i++)
    {
        do {
            randomLocationX[i] = rand() % 2280;
            randomLocationY[i] = rand() % 720;
        } while (encountersForbiddenTile(randomLocationX[i], randomLocationY[i]) || encountersForbiddenTile(randomLocationX[i] + 20, randomLocationY[i] + 20) || encountersForbiddenTile(randomLocationX[i] - 20, randomLocationY[i] - 20));
    }

    speedBoostPerk.rect.x = randomLocationX[0];
    speedBoostPerk.rect.y = randomLocationY[0];
    freezePerk.rect.x = randomLocationX[1];
    freezePerk.rect.y = randomLocationY[1];
    reverseKeysPerk.rect.x = randomLocationX[2];
    reverseKeysPerk.rect.y = randomLocationY[2];

    speedBoostPerk.rect.w = freezePerk.rect.w = reverseKeysPerk.rect.w = PERK_WIDTH;
    speedBoostPerk.rect.h = freezePerk.rect.h = reverseKeysPerk.rect.h = PERK_HEIGHT;
    speedBoostPerk.available = freezePerk.available = reverseKeysPerk.available = true;
    speedBoostPerk.duration = freezePerk.duration = reverseKeysPerk.duration = 0;    // Initialize duration to 0
    speedBoostPerk.respawnTime = freezePerk.duration = reverseKeysPerk.duration = 0; // Initialize respawn time to 0

    if (perkNr == 1) {
        speedBoostPerk.perkID = perkNr;
        return speedBoostPerk;
    }
    else if (perkNr == 2) {
        freezePerk.perkID = perkNr;
        return freezePerk;
    }
    else if (perkNr == 3) {
        reverseKeysPerk.perkID = perkNr;
        return reverseKeysPerk;
    }
    else {
        // Return a default perk if perkNr is invalid
        speedBoostPerk.perkID = perkNr;
        return speedBoostPerk;
    }
}


void checkPerkRespawn(Perk *perk) {
    time_t current_time = time(NULL); // Get the current time

    if (!perk->available && perk->respawnTime > 0 && current_time >= perk->respawnTime) {
        perk->available = true;
        perk->respawnTime = 0; // Reset the respawn time
    }
}
