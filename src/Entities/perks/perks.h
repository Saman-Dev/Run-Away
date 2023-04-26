#ifndef PERKS_H
#define PERKS_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "../player/player.h"
#include <stdbool.h>

typedef struct {
    int type;
    int value;
    int duration;
    SDL_Rect perkRect;
    SDL_Texture* texture;
} Perk;

Perk* create_perk(int type, int value, SDL_Rect* perkRect, SDL_Texture* texture);
void apply_perk(Player player, Perk* perk);
void destroy_perk(Perk* perk);

#endif
