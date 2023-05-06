#include "perks.h"
#include "../player/player.h"

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
    SDL_Texture* perkTexture = IMG_LoadTexture(renderer, "resources/perk.png");
    if (perkTexture == NULL) {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }
    speedBoostPerk.texture = perkTexture;
    speedBoostPerk.rect.x = 300; 
    speedBoostPerk.rect.y = 300; 
    speedBoostPerk.rect.w = PERK_WIDTH;  
    speedBoostPerk.rect.h = PERK_HEIGHT; 
    speedBoostPerk.available = true;
    return speedBoostPerk;
}