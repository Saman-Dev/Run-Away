#ifndef PERKS_H
#define PERKS_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../player/player.h"

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool available;
} SpeedBoostPerk;

SpeedBoostPerk initializeSpeedBoostPerk(SDL_Renderer *renderer);
void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk);
void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk perk);
bool checkCollision(SDL_Rect a, SDL_Rect b);

#endif