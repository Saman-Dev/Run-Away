#ifndef PERKS_H
#define PERKS_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "../player/player.h"

#define PERK_WIDTH 32
#define PERK_HEIGHT 32
#define PERK_DURATION 5
#define SPEED_BOOST_AMOUNT 1
#define SPEED_BOOST_DURATION 0

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool available;
    int duration;
    time_t respawnTime;
    int perkID;
} Perk;


Perk initializePerk(SDL_Renderer *renderer, int perkNr);
void renderPerk(SDL_Renderer *renderer, Perk *perk);
bool checkCollision(SDL_Rect a, SDL_Rect b);
void checkPerkRespawn(Perk *perk);
void applyPerk(Player players[], Perk *perk, SDL_Renderer *renderer );


#endif

