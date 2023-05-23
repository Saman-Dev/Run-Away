#include "perks.h"

void applyPerk(Player players[], Perk *perk, SDL_Renderer *renderer) {
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
            }
        }
    }
    else {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            Player *player = &players[i];
            if (!perk->available && perk->duration > 0 && player->hasPerk) {
                time_t current_time = time(NULL);
                double elapsed_time = difftime(current_time, player->perkStartTime);

                if (elapsed_time >= perk->duration) {
                    perk->duration = 0;
                    perk->respawnTime = time(NULL) + 10;
                    for (int j = 0; j < MAX_PLAYERS; j++) {
                        Player *otherPlayer = &players[j];
                        otherPlayer->frozen = false;
                        otherPlayer->speed = DEFAULT_SPEED;
                    }
                    player->hasPerk = false;
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
            SDL_RenderCopy(renderer, frozenTexture, NULL, &player->position);
        }
    }

    SDL_DestroyTexture(frozenTexture);
}

void renderPerk(SDL_Renderer *renderer, Perk *perk) {
    if (perk->available) {
        SDL_RenderCopy(renderer, perk->texture, NULL, &perk->rect);
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b) // check perk/player collision
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}

Perk initializePerk(SDL_Renderer *renderer, int perkNr) {
    Perk speedBoostPerk;
    Perk freezePerk;
    SDL_Texture *perkTextureSpeed = IMG_LoadTexture(renderer, "resources/newperk.png");
    SDL_Texture *perkTextureFreeze = IMG_LoadTexture(renderer, "resources/newperk2.png");
    if (perkTextureSpeed == NULL || perkTextureFreeze == NULL) {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    srand(time(NULL));
    int randomLocation = rand() % 2; // generate random number to determine perk location between 2 spawnpoints

    speedBoostPerk.texture = perkTextureSpeed;
    freezePerk.texture = perkTextureFreeze;

    speedBoostPerk.rect.x = 860;
    speedBoostPerk.rect.y = 750;
    freezePerk.rect.x = 400;
    freezePerk.rect.y = 200;

    /*if (randomLocation == 1)
    {
        speedBoostPerk.rect.x = 500;
        speedBoostPerk.rect.y = 740;
        freezePerk.rect.x = 900;
        freezePerk.rect.y = 400;
    }
    else
    {
        speedBoostPerk.rect.x = 860;
        speedBoostPerk.rect.y = 750;
        freezePerk.rect.x = 400;
        freezePerk.rect.y = 200;
    }*/

    speedBoostPerk.rect.w = freezePerk.rect.w = PERK_WIDTH;
    speedBoostPerk.rect.h = freezePerk.rect.h = PERK_HEIGHT;
    speedBoostPerk.available = freezePerk.available = true;
    speedBoostPerk.duration = freezePerk.duration = 0;    // Initialize duration to 0
    speedBoostPerk.respawnTime = freezePerk.duration = 0; // Initialize respawn time to 0

    if (perkNr == 1) {
        speedBoostPerk.perkID = perkNr;
        return speedBoostPerk;
    }
    else {
        freezePerk.perkID = perkNr;
        return freezePerk;
    }
}

void checkPerkRespawn(Perk *perk) {
    time_t current_time = time(NULL); // Get the current time

    if (!perk->available && perk->respawnTime > 0 && current_time >= perk->respawnTime) {
        perk->available = true;
        perk->respawnTime = 0; // Reset the respawn time
    }
}
