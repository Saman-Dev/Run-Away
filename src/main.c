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

// Player Sprite dimensions
#define PLAYER_FRAME_WIDTH 32
#define PLAYER_FRAME_HEIGHT 32
#define FRAME_COUNT 12

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
} Foundation;

typedef struct {
    SDL_Texture *spriteSheetTexture;
    SDL_Rect spriteClip[12];
    SDL_Rect position;
    int frame;
    int speed;
    bool up;
    bool down;
    bool left;
    bool right;
} Player;

Player createPlayer(SDL_Renderer *renderer, char playerModel[], int positionX, int positionY) {
    Player playerX;

    playerX.spriteSheetTexture = IMG_LoadTexture(renderer, playerModel);
    if (playerX.spriteSheetTexture == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }

    int frame_count = 0;
    for (int y = 0; y < 4; y++) 
    {
        for (int x = 0; x < 3; x++) 
        {
            playerX.spriteClip[frame_count].x = x * (PLAYER_FRAME_HEIGHT) + 1; // 32 width/height
            playerX.spriteClip[frame_count].y = y * (PLAYER_FRAME_WIDTH) + 3;
            playerX.spriteClip[frame_count].w = PLAYER_FRAME_WIDTH;
            playerX.spriteClip[frame_count].h = PLAYER_FRAME_HEIGHT;
            frame_count++;
        }
    }

    playerX.down = false;
    playerX.up = false;
    playerX.left = false;
    playerX.right = false;

    playerX.position.x = (positionX - PLAYER_FRAME_WIDTH) / 2;
    playerX.position.y = (positionY - PLAYER_FRAME_HEIGHT) / 2;
    playerX.position.w = PLAYER_FRAME_WIDTH;
    playerX.position.h = PLAYER_FRAME_HEIGHT;

    playerX.frame = 6;
    playerX.speed = 2;

    return playerX;
}

void handlePlayerMovement(Player *playerX);

bool init(SDL_Renderer **renderer);
void loadMapGrid(SDL_Renderer *renderer, SDL_Texture **tilesModule, SDL_Rect tilesGraphic[]);
void renderBackground(SDL_Renderer *renderer, SDL_Texture *mTile, SDL_Rect tilesGraphic[]);
void renderSpeedBoostPerk(SDL_Renderer *renderer);
bool checkPerkCollision(SDL_Rect a, SDL_Rect b);

SpeedBoostPerk speedBoostPerk;

int main(int argc, char *args[]) {
    srand(time(NULL));
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    bool quit = false;

    Player player1;
    Player hunter;

    // Perk
    speedBoostPerk.rect.x = 300; 
    speedBoostPerk.rect.y = 300; 
    speedBoostPerk.rect.w = PERK_WIDTH;  
    speedBoostPerk.rect.h = PERK_HEIGHT; 
    speedBoostPerk.active = true;

    // Background
    SDL_Texture *tilesModule = NULL;
    SDL_Rect tilesGraphic[16];

    if (init(&renderer))
    {
        printf("worked\n");
    }

    player1 = createPlayer(renderer, "resources/Runner_1.png", 50, 50);
    hunter = createPlayer(renderer, "resources/Hunter.png", 80, 80);

    loadMapGrid(renderer, &tilesModule, tilesGraphic);

    //initSpeedBoostPerk(); // Initialize the speed boost perk

    speedBoostPerk.texture = IMG_LoadTexture(renderer, "resources/perk.png");

    // Game loop - 1. Game Event 2. Game Logic 3. Render Game

    while (!quit) {
        // Game event
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    hunter.up = true;
                    break;
                case SDLK_s:
                    hunter.down = true;
                    break;
                case SDLK_a:
                    hunter.left = true;
                    break;
                case SDLK_d:
                    hunter.right = true;
                    break;
                case SDLK_UP:
                    player1.up = true;
                    break;
                case SDLK_DOWN:
                    player1.down = true;
                    break;
                case SDLK_LEFT:
                    player1.left = true;
                    break;
                case SDLK_RIGHT:
                    player1.right = true;
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    hunter.up = false;
                    break;
                case SDLK_s:
                    hunter.down = false;
                    break;
                case SDLK_a:
                    hunter.left = false;
                    break;
                case SDLK_d:
                    hunter.right = false;
                    break;
                case SDLK_UP:
                    player1.up = false;
                    break;
                case SDLK_DOWN:
                    player1.down = false;
                    break;
                case SDLK_LEFT:
                    player1.left = false;
                    break;
                case SDLK_RIGHT:
                    player1.right = false;
                    break;
                default:
                    break;
                }
            }
        }

        handlePlayerMovement(&player1);
        handlePlayerMovement(&hunter);

        /*

        if (speedBoostPerk.active && checkPerkCollision(spriteRect, speedBoostPerk.rect)) 
        {
            // Apply the speed boost effect to the player
            PLAYER_SPEED += 2; // Increase the speed
            speedBoostPerk.active = false; // Deactivate the perk
        }

        */

        // Add a delay to control the speed of the player
        SDL_Delay(16);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Game renderer
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        renderBackground(renderer, tilesModule, tilesGraphic);

        // Perk render
        renderSpeedBoostPerk(renderer);

        // Render players
        SDL_RenderCopyEx(renderer, player1.spriteSheetTexture, &player1.spriteClip[player1.frame], &player1.position, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, hunter.spriteSheetTexture, &hunter.spriteClip[hunter.frame], &hunter.position, 0, NULL, SDL_FLIP_NONE);

        // Present the rendered frame
        SDL_RenderPresent(renderer);
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

bool init(SDL_Renderer **renderer)
{
    bool test = true;
    SDL_Window *gWindow = NULL;

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize SDL2 Mixer library and play music
    if (init_audio() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer");
        SDL_Quit();
        return -1;
    }

    gWindow = SDL_CreateWindow("The Alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("Fungerar ej\n");
        test = false;
    }
    *renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL)
    {
        printf("Fungerar ej\n");
        test = false;
    }
    return test;
}

bool checkPerkCollision(SDL_Rect a, SDL_Rect b) // check perk collision
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}

void renderSpeedBoostPerk(SDL_Renderer *renderer) 
{
    if (speedBoostPerk.active)
    {
        SDL_RenderCopy(renderer, speedBoostPerk.texture, NULL, &speedBoostPerk.rect);
    }
}

void handlePlayerMovement(Player *playerX) {
    if (playerX->up) {
        if (!encountersForbiddenTile(playerX->position.x, playerX->position.y - 5)) {
            printf("Player: Up\n");
            playerX->position.y -= playerX->speed;
            if (playerX->frame == 9 || playerX->frame == 10)
                playerX->frame++;
            else
                playerX->frame = 9;
        }
    }
    else if (playerX->down) {
        if (!encountersForbiddenTile(playerX->position.x, playerX->position.y + 26)) {
            printf("Player: Down\n");
            playerX->position.y += playerX->speed;
            if (playerX->frame == 0 || playerX->frame == 1)
                playerX->frame++;
            else
                playerX->frame = 0;
        }
    }
    if (playerX->left) {
        if (!encountersForbiddenTile(playerX->position.x - 5, playerX->position.y)) {
            printf("Player: Left\n");
            playerX->position.x -= playerX->speed;
            if (playerX->frame == 3 || playerX->frame == 4)
                playerX->frame++;
            else
                playerX->frame = 3;
        }
    }
    else if (playerX->right) {
        if (!encountersForbiddenTile(playerX->position.x + 16, playerX->position.y)) {
            printf("Player: Right\n");
            playerX->position.x += playerX->speed;
            if (playerX->frame == 6 || playerX->frame == 7)
                playerX->frame++;
            else
                playerX->frame = 6;
        }
    }
}