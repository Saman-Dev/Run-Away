#include "perks.h"

void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk)
{
    if (perk->available && checkCollision(player->position, perk->rect)) 
    {
        player->boostedSpeed = player->baseSpeed + SPEED_BOOST_AMOUNT;
        player->speedBoostStartTime = SDL_GetTicks();
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

void updateSpeedBoost(Player *player)
{
    Uint32 now = SDL_GetTicks();
    if (player->speedBoostStartTime && now - player->speedBoostStartTime >= SPEED_BOOST_DURATION) 
    {
        player->boostedSpeed = player->baseSpeed; // reset speed
        player->speedBoostStartTime = 0; // reset time
    }
    player->speed = player->boostedSpeed; // use the boosted speed
}
