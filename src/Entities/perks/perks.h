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
    int activationTime;
    int respawnTime;
} SpeedBoostPerk;

typedef struct {
    SDL_Texture *texture;
    SDL_Rect rect;
    bool available;
    int duration;
    time_t respawnTime;
} FreezePerk;


SpeedBoostPerk initializeSpeedBoostPerk(SDL_Renderer *renderer);
void applySpeedBoostPerk(Player players[], SpeedBoostPerk *perk);
void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk *perk);
bool checkCollision(SDL_Rect a, SDL_Rect b);
void checkPerkRespawn(SpeedBoostPerk *perk);
FreezePerk initializeFreezePerk(SDL_Renderer *renderer);
void applyFreezePerk(Player players[], FreezePerk *perk);
void checkFreezPerkRespawn(FreezePerk *perk);
void renderFreezPerk(SDL_Renderer *renderer, FreezePerk *perk);


#endif

