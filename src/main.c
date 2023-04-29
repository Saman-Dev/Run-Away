#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#include "Entities/audio/audio.h"
#include "Entities/map/map.h"
#include "Entities/player/player.h"
#include "Entities/network/network.h"

// Screen dimensions
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 955

// Perk dimensions
#define PERK_WIDTH 30
#define PERK_HEIGHT 30
#define SPEED_BOOST_AMOUNT 1
#define PERK_SPAWN_TIME 5000 // in milliseconds
#define PERK_FRAME_COUNT 10
#define PERK_FRAME_DELAY 300

#undef main

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool available;
    int currentFrame;
    int frameTimer;
} SpeedBoostPerk;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool quit;
} Framework;

void initialize(Framework *game);

void handleInput(Framework *game, Player *playerX, Player *playerY);
static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY);
static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY);

void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk perk, SDL_Rect* perkFrames);
bool checkPerkCollision(SDL_Rect a, SDL_Rect b);
void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk);

int main(int argc, char *args[]) 
{
    Framework game;
    Background resources;
    Player player1;
    Player hunter;

    initialize(&game);
    initiateMapResources(game.renderer, &resources);

    /////

    //Network information;
    //information = setUpNetwork("192.168.0.30", 2000);

    /////


    SDL_Texture* perkTexture = IMG_LoadTexture(game.renderer, "resources/perk.png");
    if (perkTexture == NULL) 
    {
        printf("Failed to load perk sprite sheet: %s\n", IMG_GetError());
        exit(1);
    }

    // Perk
    SpeedBoostPerk speedBoostPerk;
    speedBoostPerk.texture = perkTexture;
    speedBoostPerk.rect.x = 300; 
    speedBoostPerk.rect.y = 300; 
    speedBoostPerk.rect.w = PERK_WIDTH;  
    speedBoostPerk.rect.h = PERK_HEIGHT; 
    speedBoostPerk.available = true;

    // Perk animation frames
    SDL_Rect perkFrames[PERK_FRAME_COUNT];
    for (int i = 0; i < PERK_FRAME_COUNT; i++) 
    {
        perkFrames[i].x = i * (30);  
        perkFrames[i].y = i % 2 * PERK_HEIGHT;
        perkFrames[i].w = PERK_WIDTH;
        perkFrames[i].h = PERK_HEIGHT;
    }
    

    player1 = createPlayer(game.renderer, "resources/Runner_1.png", 400, 400);
    hunter = createPlayer(game.renderer, "resources/Hunter.png", 470, 470);
    
    while (!game.quit) 
    {
        // Handle events
        handleInput(&game, &player1, &hunter);
        handlePlayerMovement(&player1);
        handlePlayerMovement(&hunter);

        // Check for perk collision
        applySpeedBoostPerk(&player1, &speedBoostPerk);
        applySpeedBoostPerk(&hunter, &speedBoostPerk);

        // Add a delay to control the speed of the player
        SDL_Delay(16);

        // Clear the renderer (Nödvändig?)
        // SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        // SDL_RenderClear(game.renderer);
        
        // Game renderer
        SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(game.renderer);
        renderBackground(game.renderer, resources);

        // Render players
        SDL_RenderCopyEx(game.renderer, player1.spriteSheetTexture, &player1.spriteClip[player1.frame], &player1.position, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(game.renderer, hunter.spriteSheetTexture, &hunter.spriteClip[hunter.frame], &hunter.position, 0, NULL, SDL_FLIP_NONE);

        // Perk render
        renderSpeedBoostPerk(game.renderer, speedBoostPerk, perkFrames);

        // Present the rendered frame
        SDL_RenderPresent(game.renderer);

        /////
        //sendData(&information, 60, 60);
        /////


    }

    // Free resources and close SDL
    SDL_DestroyTexture(perkTexture);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}

void initialize(Framework *game) 
{
    game->quit = false;
    
    // Initialize SDL and timer
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));

    // Initialize SDL2 Mixer library and play music
    if (init_audio() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer");
        exit(1);
    }

    game->window = SDL_CreateWindow("The Alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (game->window == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (game->renderer == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
}

bool checkPerkCollision(SDL_Rect a, SDL_Rect b) // check perk collision
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}

void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk perk, SDL_Rect* perkFrames)
{
    if (perk.available)
    {
        SDL_Rect clipRect = perkFrames[perk.currentFrame];
        SDL_RenderCopy(renderer, perk.texture, &clipRect, &perk.rect);
    }
}

void handleInput(Framework *game, Player *playerX, Player *playerY) {
    while (SDL_PollEvent(&game->event)) {
        if (game->event.type == SDL_QUIT) {
            game->quit = true;
        }
        else if (game->event.type == SDL_KEYDOWN) {
            handleKeyPresses(game, playerX, playerY);
        }
        else if (game->event.type == SDL_KEYUP) {
            handleKeyReleases(game, playerX, playerY);
        }
    }
}

static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY) {
    switch (game->event.key.keysym.sym) {
        case SDLK_UP:
            playerX->up = true;
            break;
        case SDLK_DOWN:
            playerX->down = true;
            break;
        case SDLK_LEFT:
            playerX->left = true;
            break;
        case SDLK_RIGHT:
            playerX->right = true;
            break;
        case SDLK_w:
            playerY->up = true;
            break;
        case SDLK_s:
            playerY->down = true;
            break;
        case SDLK_a:
            playerY->left = true;
            break;
        case SDLK_d:
            playerY->right = true;
            break;
        case SDLK_ESCAPE:
            game->quit = true;
            break;
        default:
            break;        
    }
}

static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY) {
    switch (game->event.key.keysym.sym) {
        case SDLK_UP:
            playerX->up = false;
            break;
        case SDLK_DOWN:
            playerX->down = false;
            break;
        case SDLK_LEFT:
            playerX->left = false;
            break;
        case SDLK_RIGHT:
            playerX->right = false;
            break;
        case SDLK_w:
            playerY->up = false;
            break;
        case SDLK_s:
            playerY->down = false;
            break;
        case SDLK_a:
            playerY->left = false;
            break;
        case SDLK_d:
            playerY->right = false;
            break;
        default:
            break;
    }
}

void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk)
{
    if (perk->available && checkPerkCollision(player->position, perk->rect)) 
    {
        player->speed += SPEED_BOOST_AMOUNT;
        perk->available = false;
    }

    if (!perk->available) 
    {
        return;
    }

    // Update perk animation frame
    perk->frameTimer += 16;
    if (perk->frameTimer >= PERK_FRAME_DELAY) 
    {
        perk->currentFrame = (perk->currentFrame + 1) % PERK_FRAME_COUNT;
        perk->frameTimer = 0;
    }
}