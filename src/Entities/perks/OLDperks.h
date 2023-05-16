/*#ifndef PERKS_H
#define PERKS_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "../player/player.h"

#define PERK_WIDTH 32
#define PERK_HEIGHT 32
#define PERK_DURATION 10
#define SPEED_BOOST_AMOUNT 1
#define SPEED_BOOST_DURATION 4

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool available;
    int duration;
    int activationTime;
} SpeedBoostPerk;

SpeedBoostPerk initializeSpeedBoostPerk(SDL_Renderer *renderer);
void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk);
void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk perk);
bool checkCollision(SDL_Rect a, SDL_Rect b);


#endif

*/