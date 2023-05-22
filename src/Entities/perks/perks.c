#include "perks.h"
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#define MAX_PLAYERS 5
#define perk_duration 5
#define DEFAULT_SPEED 2

void applySpeedBoostPerk(Player players[], SpeedBoostPerk *perk)
{
    static time_t start_time[MAX_PLAYERS] = {0}; // Declare start_time as static array to retain its value between function calls
    static int active[MAX_PLAYERS] = {-1};

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
                start_time[player->player] = time(NULL); // Set start time when the perk is applied
                active[player->player] = player->player;
            }
        }
    }
    else
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            Player *player = &players[i];
            if (!perk->available && perk->duration > 0 && active[player->player] == player->player)
            {
                time_t current_time = time(NULL); // Get the current time
                double elapsed_time = difftime(current_time, start_time[player->player]); // Calculate elapsed time

                if (elapsed_time >= perk->duration)
                {
                    player->speed -= SPEED_BOOST_AMOUNT;
                    perk->duration = 0;
                    perk->respawnTime = time(NULL) + 10; // Set the respawn time as 10 seconds from now
                }
                else
                {
                    int remaining_time = (int)(perk->duration - elapsed_time);
                    printf("Player %d - Time remaining: %d seconds\n", player->player, remaining_time);
                }
            }
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

bool checkCollision(SDL_Rect a, SDL_Rect b) // check perk/player collision
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
        speedBoostPerk.rect.x = 500;
        speedBoostPerk.rect.y = 740;
    }
    else
    {
        speedBoostPerk.rect.x = 860;
        speedBoostPerk.rect.y = 750;
    }

    speedBoostPerk.rect.w = PERK_WIDTH;
    speedBoostPerk.rect.h = PERK_HEIGHT;
    speedBoostPerk.available = true;
    speedBoostPerk.duration = 0;       // Initialize duration to 0
    speedBoostPerk.respawnTime = 0;    // Initialize respawn time to 0

    return speedBoostPerk;
}

void checkPerkRespawn(SpeedBoostPerk *perk)
{
    time_t current_time = time(NULL); // Get the current time
    
    if (!perk->available && perk->respawnTime > 0 && current_time >= perk->respawnTime)
    {
        perk->available = true;
        perk->respawnTime = 0; // Reset the respawn time
    }
}

FreezePerk initializeFreezePerk(SDL_Renderer *renderer)
{
    FreezePerk freezePerk;
    SDL_Texture *perkTexture = IMG_LoadTexture(renderer, "resources/newperk2.png");
    if (perkTexture == NULL)
    {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    srand(time(NULL));
    int randomLocation = rand() % 2; // generate random number to determine perk location between 2 spawnpoints

    freezePerk.texture = perkTexture;

    if (randomLocation == 1)
    {
        freezePerk.rect.x = 400;
        freezePerk.rect.y = 200;
    }
    else
    {
        freezePerk.rect.x = 400;
        freezePerk.rect.y = 200;
    }

    freezePerk.rect.w = PERK_WIDTH;
    freezePerk.rect.h = PERK_HEIGHT;
    freezePerk.available = true;
    freezePerk.duration = 0;       // Initialize duration to 0
    freezePerk.respawnTime = 0;    // Initialize respawn time to 0

    return freezePerk;
}
void renderFreezPerk(SDL_Renderer *renderer, FreezePerk *perk)
{
    if (perk->available)
    {
        SDL_RenderCopy(renderer, perk->texture, NULL, &perk->rect);
    }
}

void applyFreezePerk(Player players[], FreezePerk *perk)
{
    static time_t start_time[MAX_PLAYERS] = {0};
    static int active[MAX_PLAYERS] = {-1};

    if (perk->available)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            Player *player = &players[i];
            if (checkCollision(player->position, perk->rect))
            {
                for (int j = 0; j < MAX_PLAYERS; j++)
                {
                    Player *otherPlayer = &players[j];
                    if (i != j)
                    {
                        otherPlayer->frozen = true;
                        otherPlayer->speed = 0;
                    }
                }
                perk->available = false;
                perk->duration = 2;
                start_time[player->player] = time(NULL);
                active[player->player] = player->player;
            }
        }
    }
    else
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            Player *player = &players[i];
            if (!perk->available && perk->duration > 0 && active[player->player] == player->player)
            {
                time_t current_time = time(NULL);
                double elapsed_time = difftime(current_time, start_time[player->player]);

                if (elapsed_time >= perk->duration)
                {
                    for (int j = 0; j < MAX_PLAYERS; j++)
                    {
                        Player *otherPlayer = &players[j];
                        if (i != j)
                        {
                            otherPlayer->frozen = false;
                            otherPlayer->speed = DEFAULT_SPEED;  // Give back the default speed to other players
                        }
                    }
                    perk->duration = 0;
                    perk->respawnTime = time(NULL) + 10;
                }
                else
                {
                    int remaining_time = (int)(perk->duration - elapsed_time);
                    printf("Player %d - Time remaining: %d seconds\n", player->player, remaining_time);
                }
            }
        }
    }
}


void checkFreezPerkRespawn(FreezePerk *perk)
{
    time_t current_time = time(NULL); // Get the current time
    
    if (!perk->available && perk->respawnTime > 0 && current_time >= perk->respawnTime)
    {
        perk->available = true;
        perk->respawnTime = 0; // Reset the respawn time
    }
}

