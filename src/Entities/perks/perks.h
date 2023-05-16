#ifndef PERKS_H
#define PERKS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../player/player.h"

#define SPEED_BOOST_FACTOR 2

typedef struct Perk {
    SDL_Texture *texture;
    SDL_Rect position;
    bool active;
    int duration;
} Perk;

Perk createSpeedPerk(SDL_Renderer *renderer, char* spritePath, int posX, int posY, int duration);
void renderPerk(SDL_Renderer *renderer, Perk *perk);
void applyPerk(Perk *perk, Player *player);
void checkPerkCollision(Player *player, Perk *perk);
void decrementPerkDuration(Perk *perk, Player *player);
bool checkCollision(SDL_Rect a, SDL_Rect b);


#endif 
