#ifndef MECHANICS_H
#define MECHANICS_H

#include <SDL2/SDL.h>

#include "../player/player.h"
#include "../mechanics/mechanics.h"
#include "../perks/perks.h"

void HuntAndRevive(SDL_Renderer *renderer, Player players[]);
static void checkCapturedStatus(Player players[]);
static void handleCage(SDL_Renderer *renderer, Image *cage, Player players[]);

#endif