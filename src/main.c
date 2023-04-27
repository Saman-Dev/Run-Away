#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "Entities/audio/audio.h"
#include "Entities/map/map.h"
#include "Entities/player/player.h"

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Perk dimensions
#define PERK_WIDTH 20
#define PERK_HEIGHT 20
#define SPEED_BOOST_AMOUNT 1
#define PERK_SPAWN_TIME 5000 // in milliseconds

#undef main

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool active;
} SpeedBoostPerk;


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool quit;
} Framework;

void initialize(Framework *game);

void handleInput(Framework *game, Player *playerX, Player *playerY);
void handleKeyPresses(Framework *game, Player *playerX, Player *playerY);
void handleKeyReleases(Framework *game, Player *playerX, Player *playerY);

void loadMapGrid(SDL_Renderer *renderer, SDL_Texture **tilesModule, SDL_Rect tilesGraphic[]);
void renderBackground(SDL_Renderer *renderer, SDL_Texture *mTile, SDL_Rect tilesGraphic[]);

void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk perk);
bool checkPerkCollision(SDL_Rect a, SDL_Rect b);
void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk);


int main(int argc, char *args[]) 
{
    Framework game;
    Player player1;
    Player hunter;

    // Perk
    SpeedBoostPerk speedBoostPerk;
    speedBoostPerk.rect.x = 300; 
    speedBoostPerk.rect.y = 300; 
    speedBoostPerk.rect.w = PERK_WIDTH;  
    speedBoostPerk.rect.h = PERK_HEIGHT; 
    speedBoostPerk.active = true;

    // Background
    SDL_Texture *tilesModule = NULL;
    SDL_Rect tilesGraphic[16];

    initialize(&game);

    player1 = createPlayer(game.renderer, "resources/Runner_1.png", 50, 50);
    hunter = createPlayer(game.renderer, "resources/Hunter.png", 80, 80);

    loadMapGrid(game.renderer, &tilesModule, tilesGraphic);

    speedBoostPerk.texture = IMG_LoadTexture(game.renderer, "resources/perk.png");
    
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

        // Clear the renderer
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game.renderer);
        
        // Game renderer
        SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(game.renderer);
        renderBackground(game.renderer, tilesModule, tilesGraphic);

        // Perk render
        renderSpeedBoostPerk(game.renderer, speedBoostPerk);

        // Render players
        SDL_RenderCopyEx(game.renderer, player1.spriteSheetTexture, &player1.spriteClip[player1.frame], &player1.position, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(game.renderer, hunter.spriteSheetTexture, &hunter.spriteClip[hunter.frame], &hunter.position, 0, NULL, SDL_FLIP_NONE);

        // Present the rendered frame
        SDL_RenderPresent(game.renderer);
    }

    // Free resources and close SDL
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}

void renderBackground(SDL_Renderer *renderer, SDL_Texture *tilesModule, SDL_Rect tilesGraphic[])
{
    SDL_Rect position;
    position.y = 0;
    position.x = 0;
    position.h = getTheHeightOfTheTile();
    position.w = getTheWidthOfTheTile();
    
    for (int i = 0; i < getNumberOfColumnsInMap(); i++)
    {
        for (int j = 0; j < getNumberOfRowsInMap(); j++)
        {
            position.y = i * getTheHeightOfTheTile();
            position.x = j * getTheWidthOfTheTile();
            SDL_RenderCopyEx(renderer, tilesModule, &tilesGraphic[getTileInformation(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
}

void loadMapGrid(SDL_Renderer *renderer, SDL_Texture **tilesModule, SDL_Rect tilesGraphic[])
{

    SDL_Surface *gTilesSurface = IMG_Load("resources/Map.JPG");
    *tilesModule = SDL_CreateTextureFromSurface(renderer, gTilesSurface);
    for (int i = 0; i < 5; i++)
    {
        tilesGraphic[i].x = i * getTheWidthOfTheTile();
        tilesGraphic[i].y = 0;
        tilesGraphic[i].w = getTheWidthOfTheTile();
        tilesGraphic[i].h = getTheHeightOfTheTile();
    }
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

void renderSpeedBoostPerk(SDL_Renderer *renderer, SpeedBoostPerk perk) 
{
    if (perk.active)
    {
        SDL_RenderCopy(renderer, perk.texture, NULL, &perk.rect);
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

void handleKeyPresses(Framework *game, Player *playerX, Player *playerY) {
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

void handleKeyReleases(Framework *game, Player *playerX, Player *playerY) {
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
    if (perk->active && checkPerkCollision(player->position, perk->rect)) 
    {
        player->speed += SPEED_BOOST_AMOUNT;
        perk->active = false;
    }
}
