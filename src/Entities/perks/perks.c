#include "perks.h"
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk)
{
    static time_t start_time = 0; // Declare start_time as static to retain its value between function calls
    static int active = -1;

    if (perk->available && checkCollision(player->position, perk->rect)) 
    {
        player->speed += SPEED_BOOST_AMOUNT;
        perk->available = false;
        perk->duration = 10;
        start_time = time(NULL); // Set start time when the perk is applied
        active = player->player;
    }
    else if(!perk->available && perk->duration > 0 && active == player->player)
    {
        time_t current_time = time(NULL); // Get the current time
        double elapsed_time = difftime(current_time, start_time); // Calculate elapsed time

        if (elapsed_time >= 10)
        {
            player->speed -= SPEED_BOOST_AMOUNT;
            perk->duration = 0;
            printf("speed %d\n", player->speed);
        }
        else
        {
            int remaining_time = (int)(10 - elapsed_time);
            printf("Time remaining: %d seconds\n", remaining_time);
            printf("Player: %d \n", player->player);

        }
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
    speedBoostPerk.duration = 0; // Initialize duration to 0

    return speedBoostPerk;
}
