#include "perks.h"
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#define MAX_PLAYERS 5
#define perk_duration 10

void applySpeedBoostPerk(Player players[], SpeedBoostPerk *perk)
{
    static time_t start_time[MAX_PLAYERS] = {0};
    static int active[MAX_PLAYERS] = {-1};
    static bool respawned = true;
    static time_t respawn_time = 0;

    if (perk->available)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            Player *player = &players[i];
            if (checkCollision(player->position, perk->rect))
            {
                player->speed += SPEED_BOOST_AMOUNT;
                perk->available = false;
                perk->duration = perk_duration;
                start_time[player->player] = time(NULL);
                active[player->player] = player->player;
                respawned = false;
            }
        }
    }
    else
    {
        bool activePlayer = false;
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            Player *player = &players[i];
            if (!perk->available && perk->duration > 0 && active[player->player] == player->player)
            {
                activePlayer = true;
                time_t current_time = time(NULL);
                double elapsed_time = difftime(current_time, start_time[player->player]);

                if (elapsed_time >= perk_duration)
                {
                    player->speed -= SPEED_BOOST_AMOUNT;
                    perk->duration = 0;
                    active[player->player] = -1;
                    respawned = false;
                }
                else
                {
                    int remaining_time = (int)(perk_duration - elapsed_time);
                    printf("Player %d - Time remaining: %d seconds\n", player->player, remaining_time);
                }
            }
        }

        if (!respawned && !activePlayer)
        {
            time_t current_time = time(NULL);
            double elapsed_time = difftime(current_time, start_time[0]); // Use the first player's start time for consistency
            if (elapsed_time >= 15)
            {
                perk->available = true;
                respawned = true;
                respawn_time = 0;
                printf("respwan time %d", elapsed_time);
            }
        }
        else if (respawned && respawn_time == 0)
        {
            respawn_time = time(NULL);
        }
    }
}

void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk *perk)
{
    if (perk->available)
    {
        SDL_RenderCopy(renderer, perk->texture, NULL, &perk->rect);
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}

SpeedBoostPerk initializeSpeedBoostPerk(SDL_Renderer *renderer)
{
    SpeedBoostPerk speedBoostPerk;
    SDL_Texture *perkTexture = IMG_LoadTexture(renderer, "resources/newperk.png");
    if (perkTexture == NULL)
    {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    srand(time(NULL));
    int randomLocation = rand() % 2; // generate random number to determine perk location between 2 spawnpoints

    speedBoostPerk.texture = perkTexture;

    if (randomLocation == 1)
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
    speedBoostPerk.duration = 0;

    return speedBoostPerk;
}
