#include "perks.h"



void applyPerk(Player players[], Perk *perk, SDL_Renderer *renderer )
{
    static time_t start_time[MAX_PLAYERS] = {0}; // Declare start_time as static array to retain its value between function calls
    static int active[MAX_PLAYERS] = {-1};

    SDL_Texture *frozenTexture = IMG_LoadTexture(renderer, "resources/cristal1.png");

    if (perk->available)
    {
        for (int i = 0; i < MAX_PLAYERS; i++) // denna for loop har koll på vilken spelare som tog perk
        {
            Player *player = &players[i];
            if (checkCollision(player->position, perk->rect))
            {
                if (perk->perkID == 1){
                    player->speed += SPEED_BOOST_AMOUNT;
                    perk->available = false;
                    perk->duration = perk_duration;
                    start_time[player->player] = time(NULL); // Set start time when the perk is applied
                    active[player->player] = player->player;
                }else if (perk->perkID = 2){
                    for (int j = 0; j < MAX_PLAYERS; j++){
                        Player *otherPlayer = &players[j];
                        if (i != j)
                        {
                            otherPlayer->frozen = true;
                            otherPlayer->speed = 0;
                        }
                    }
                    perk->available = false;
                    perk->duration = perk_duration;
                    start_time[player->player] = time(NULL); // Set start time when the perk is applied
                    active[player->player] = player->player;
                }
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
                    perk->duration = 0;
                    perk->respawnTime = time(NULL) + 10; // Set the respawn time as 10 seconds from now
                    for (int j = 0; j < MAX_PLAYERS; j++){
                        Player *otherPlayer = &players[j];
                        otherPlayer->frozen = false;
                        otherPlayer->speed = DEFAULT_SPEED;  // Give back the default speed to other players
                    }
                }
                else
                {
                    int remaining_time = (int)(perk->duration - elapsed_time);
                    printf("Player %d - Time remaining: %d seconds\n", player->player, remaining_time);
                    SDL_DestroyTexture(frozenTexture);
                }

            }
        }
    }
    

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Player *player = &players[i];
        if (player->frozen)
        {
            if (frozenTexture == NULL)
            {
                printf("Failed to load frozen texture: %s\n", IMG_GetError());
                exit(1);
            }
            SDL_RenderCopy(renderer, frozenTexture, NULL, &player->position);
        }
    }
}

void renderPerk(SDL_Renderer *renderer, Perk *perk)
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

Perk initializePerk(SDL_Renderer *renderer, int perkNr)
{
    Perk speedBoostPerk;
    Perk freezePerk;
    SDL_Texture *perkTextureSpeed = IMG_LoadTexture(renderer, "resources/newperk.png");
    SDL_Texture *perkTextureFreeze = IMG_LoadTexture(renderer, "resources/newperk2.png");
    if (perkTextureSpeed == NULL || perkTextureFreeze == NULL)
    {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    srand(time(NULL));
    int randomLocation = rand() % 2; // generate random number to determine perk location between 2 spawnpoints

    speedBoostPerk.texture = perkTextureSpeed;
    freezePerk.texture = perkTextureFreeze;


    speedBoostPerk.rect.x = 860;
        speedBoostPerk.rect.y = 750;
        freezePerk.rect.x = 400;
        freezePerk.rect.y = 200;

    /*if (randomLocation == 1)
    {
        speedBoostPerk.rect.x = 500;
        speedBoostPerk.rect.y = 740;
        freezePerk.rect.x = 900;
        freezePerk.rect.y = 400;
    }
    else
    {
        speedBoostPerk.rect.x = 860;
        speedBoostPerk.rect.y = 750;
        freezePerk.rect.x = 400;
        freezePerk.rect.y = 200;
    }*/

    speedBoostPerk.rect.w = freezePerk.rect.w = PERK_WIDTH;
    speedBoostPerk.rect.h = freezePerk.rect.h = PERK_HEIGHT;
    speedBoostPerk.available  = freezePerk.available = true;
    speedBoostPerk.duration = freezePerk.duration = 0;       // Initialize duration to 0
    speedBoostPerk.respawnTime = freezePerk.duration = 0;    // Initialize respawn time to 0
    
    if (perkNr == 1)
    {   
        speedBoostPerk.perkID = perkNr;
        return speedBoostPerk;
    }
    else
    {
        freezePerk.perkID = perkNr;
        return freezePerk;
    }
}

void checkPerkRespawn(Perk *perk)
{
    time_t current_time = time(NULL); // Get the current time
    
    if (!perk->available && perk->respawnTime > 0 && current_time >= perk->respawnTime)
    {
        perk->available = true;
        perk->respawnTime = 0; // Reset the respawn time
    }
}



/*
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

void applyPerk(Player players[], Perk *perk, SDL_Renderer *renderer )
{
    static time_t start_time[MAX_PLAYERS] = {0}; // Declare start_time as static array to retain its value between function calls
    static int active[MAX_PLAYERS] = {-1};

    SDL_Texture *frozenTexture = IMG_LoadTexture(renderer, "resources/cristal1.png");
    if (frozenTexture == NULL)
    {
        printf("Failed to load frozen texture: %s\n", IMG_GetError());
        exit(1);
    }

    if (perk->available)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            Player *player = &players[i];
            if (checkCollision(player->position, perk->rect))
            {
                if (perk->perkID == 1){
                    player->speed += SPEED_BOOST_AMOUNT;
                    perk->available = false;
                    perk->duration = perk_duration;
                    start_time[player->player] = time(NULL); // Set start time when the perk is applied
                    active[player->player] = player->player;
                }else if (perk->perkID = 2){
                    for (int j = 0; j < MAX_PLAYERS; j++){
                        Player *otherPlayer = &players[j];
                        if (i != j)
                        {
                            otherPlayer->frozen = true;
                            otherPlayer->speed = 0;
                            SDL_RenderCopy(renderer, frozenTexture, NULL, &otherPlayer->position);
                        }
                    }
                    perk->available = false;
                    perk->duration = perk_duration;
                    start_time[player->player] = time(NULL);
                    active[player->player] = player->player;
                }
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
                    perk->duration = 0;
                    perk->respawnTime = time(NULL) + 10; // Set the respawn time as 10 seconds from now
                    for (int j = 0; j < MAX_PLAYERS; j++){
                        Player *otherPlayer = &players[j];
                        if (i != j)
                        {
                            otherPlayer->frozen = false;
                            otherPlayer->speed = DEFAULT_SPEED;  // Give back the default speed to other players
                        }
                    }
                    printf("%d", players->speed);
                    
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

void renderPerk(SDL_Renderer *renderer, Perk *perk)
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

Perk initializePerk(SDL_Renderer *renderer, int perkNr)
{
    Perk speedBoostPerk;
    Perk freezePerk;
    SDL_Texture *perkTextureSpeed = IMG_LoadTexture(renderer, "resources/newperk.png");
    SDL_Texture *perkTextureFreeze = IMG_LoadTexture(renderer, "resources/newperk2.png");
    if (perkTextureSpeed == NULL || perkTextureFreeze == NULL)
    {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    srand(time(NULL));
    int randomLocation = rand() % 2; // generate random number to determine perk location between 2 spawnpoints

    speedBoostPerk.texture = perkTextureSpeed;
    freezePerk.texture = perkTextureFreeze;

    if (randomLocation == 1)
    {
        speedBoostPerk.rect.x = 500;
        speedBoostPerk.rect.y = 740;
        freezePerk.rect.x = 900;
        freezePerk.rect.y = 400;
    }
    else
    {
        speedBoostPerk.rect.x = 860;
        speedBoostPerk.rect.y = 750;
        freezePerk.rect.x = 400;
        freezePerk.rect.y = 200;
    }

    speedBoostPerk.rect.w = freezePerk.rect.w = PERK_WIDTH;
    speedBoostPerk.rect.h = freezePerk.rect.h = PERK_HEIGHT;
    speedBoostPerk.available  = freezePerk.available = true;
    speedBoostPerk.duration = freezePerk.duration = 0;       // Initialize duration to 0
    speedBoostPerk.respawnTime = freezePerk.duration = 0;    // Initialize respawn time to 0
    
    if (perkNr == 1)
    {   
        speedBoostPerk.perkID = perkNr;
        return speedBoostPerk;
    }
    else
    {
        freezePerk.perkID = perkNr;
        return freezePerk;
    }
}

void checkPerkRespawn(Perk *perk)
{
    time_t current_time = time(NULL); // Get the current time
    
    if (!perk->available && perk->respawnTime > 0 && current_time >= perk->respawnTime)
    {
        perk->available = true;
        perk->respawnTime = 0; // Reset the respawn time
    }
}

*/