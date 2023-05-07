#include "perks.h"
#include "../player/player.h"
#include <stdlib.h>
#include <time.h>

#define PERK_WIDTH 32
#define PERK_HEIGHT 32
#define PERK_DURATION 10
#define SPEED_BOOST_AMOUNT 1


void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk)
{
    if (perk->available && checkCollision(player->position, perk->rect)) 
    {
        player->speed += SPEED_BOOST_AMOUNT;
        perk->available = false;
    }

    if (!perk->available) 
    {
        return;
    }
}

void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk perk)
{
    if (perk.available)
    {
        SDL_RenderCopy(renderer, perk.texture, NULL, &perk.rect);
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b) // check perk/player collision
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}

SpeedBoostPerk initializeSpeedBoostPerk(SDL_Renderer *renderer) 
{
    SpeedBoostPerk speedBoostPerk;
    SDL_Texture* perkTexture = IMG_LoadTexture(renderer, "resources/newperk.png");
    if (perkTexture == NULL) {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    srand(time(NULL));
    int randomLocation = rand() % 2; // generate random number to determine perk location between 2 spawnpoints

    speedBoostPerk.texture = perkTexture;

    if(randomLocation == 1) 
    {
        speedBoostPerk.rect.x = 435; 
        speedBoostPerk.rect.y = 350; 
    }
    else 
    {
        speedBoostPerk.rect.x = 750; 
        speedBoostPerk.rect.y = 750; 
    }

    speedBoostPerk.rect.w = PERK_WIDTH;  
    speedBoostPerk.rect.h = PERK_HEIGHT; 
    speedBoostPerk.available = true;
    speedBoostPerk.duration = PERK_DURATION;
    return speedBoostPerk;
}