#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Entities/audio/audio.h"
#include "Entities/map/map.h"
#include "Entities/player/player.h"
#include <stdbool.h>

// Player Sprite dimensions
#define FRAME_WIDTH 32
#define FRAME_HEIGHT 32
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
    SDL_Rect rect;
    bool active;
} SpeedBoostPerk;

SpeedBoostPerk speedBoostPerk;

void initSpeedBoostPerk() {
    speedBoostPerk.rect.x = 300; 
    speedBoostPerk.rect.y = 300; 
    speedBoostPerk.rect.w = PERK_WIDTH;  
    speedBoostPerk.rect.h = PERK_HEIGHT; 
    speedBoostPerk.active = true;
}

bool init(SDL_Renderer **renderer);
void loadMedia(SDL_Renderer *renderer, int playerNr, SDL_Texture **spriteSheetTexture, SDL_Rect frameRects[], SDL_Texture **tilesModule, SDL_Rect tilesGraphic[]);
void renderBackground(SDL_Renderer *renderer, SDL_Texture *mTile, SDL_Rect tilesGraphic[]);
void renderSpeedBoostPerk(SDL_Renderer *renderer);
bool checkCollision(SDL_Rect a, SDL_Rect b);


int main(int argc, char *args[])
{

    srand(time(NULL));
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    bool quit = false;

    int collision = 0;

    // Player
    SDL_Texture *spriteSheetTexture = NULL;
    SDL_Rect frameRects[12];
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect spriteRect = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    spriteRect.x = (640 - FRAME_WIDTH) / 2;  // Center horizontally
    spriteRect.y = (480 - FRAME_HEIGHT) / 2; // Center vertically
    int currentFrame = 6;

    // Hunter
    SDL_Texture *spriteSheetTexture2 = NULL;
    SDL_Rect frameRects2[12];
    SDL_RendererFlip flip2 = SDL_FLIP_NONE;
    SDL_Rect spriteRect2 = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    spriteRect2.x = (600 - FRAME_WIDTH) / 2;  // Center horizontally
    spriteRect2.y = (400 - FRAME_HEIGHT) / 2; // Center vertically
    int currentFrame2 = 6;


    // Background
    SDL_Texture *tilesModule = NULL;
    SDL_Rect tilesGraphic[16];

    if (init(&renderer))
    {
        printf("worked\n");
    }

    int playerNr = 1;
    loadMedia(renderer, playerNr, &spriteSheetTexture, frameRects, &tilesModule, tilesGraphic);
    loadMedia(renderer, 2, &spriteSheetTexture2, frameRects2, &tilesModule, tilesGraphic);

    // Boolean array to keep track of which keys are pressed
    bool keysPressed[8] = {false, false, false, false, false, false, false, false};

    // Movement speed of the player
    int PLAYER_SPEED = 2;

    initSpeedBoostPerk(); // Initialize the speed boost perk

    // Game loop - 1. Game Event 2. Game Logic 3. Render Game
    while (!quit)
    {
        // Game event
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                // Handle key presses
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    keysPressed[0] = true;
                    break;
                case SDLK_s:
                    keysPressed[1] = true;
                    break;
                case SDLK_a:
                    keysPressed[2] = true;
                    break;
                case SDLK_d:
                    keysPressed[3] = true;
                    break;
                case SDLK_UP:
                    keysPressed[4] = true;
                    break;
                case SDLK_DOWN:
                    keysPressed[5] = true;
                    break;
                case SDLK_LEFT:
                    keysPressed[6] = true;
                    break;
                case SDLK_RIGHT:
                    keysPressed[7] = true;
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                // Handle key releases
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    keysPressed[0] = false;
                    break;
                case SDLK_s:
                    keysPressed[1] = false;
                    break;
                case SDLK_a:
                    keysPressed[2] = false;
                    break;
                case SDLK_d:
                    keysPressed[3] = false;
                    break;
                case SDLK_UP:
                    keysPressed[4] = false;
                    break;
                case SDLK_DOWN:
                    keysPressed[5] = false;
                    break;
                case SDLK_LEFT:
                    keysPressed[6] = false;
                    break;
                case SDLK_RIGHT:
                    keysPressed[7] = false;
                    break;
                default:
                    break;
                }
            }
        }

        // Move the player based on the state of the boolean array
        if (keysPressed[0])
        {
            // Move up
            collision = encountersForbiddenTile(spriteRect.x, spriteRect.y - 5);
            if (collision == 0)
            {
                printf("UP\n");
                spriteRect.y -= PLAYER_SPEED;
                if (currentFrame == 9 || currentFrame == 10)
                    currentFrame++;
                else
                    currentFrame = 9;
            }
        }
        else if (keysPressed[1])
        {
            // Move down
            collision = encountersForbiddenTile(spriteRect.x, spriteRect.y + 26);
            if (collision == 0)
            {
                spriteRect.y += PLAYER_SPEED;
                if (currentFrame == 0 || currentFrame == 1)
                    currentFrame++;
                else
                    currentFrame = 0;
            }
        }
        if (keysPressed[2])
        {
            // Move left
            collision = encountersForbiddenTile(spriteRect.x - 5, spriteRect.y);
            if (collision == 0)
            {
                printf("LEFT\n");
                spriteRect.x -= PLAYER_SPEED;
                if (currentFrame == 3 || currentFrame == 4)
                    currentFrame++;
                else
                    currentFrame = 3;
            }
        }
        else if (keysPressed[3])
        {
            // Move right
            collision = encountersForbiddenTile(spriteRect.x + 16, spriteRect.y);
            if (collision == 0)
            {
                printf("RIGHT\n");
                spriteRect.x += PLAYER_SPEED;
                if (currentFrame == 6 || currentFrame == 7)
                    currentFrame++;
                else
                    currentFrame = 6;
            }
        }
        if (keysPressed[4])
        {
            // Move up
            collision = encountersForbiddenTile(spriteRect2.x, spriteRect2.y - 5);
            if (collision == 0)
            {
                printf("Player 2: UP\n");
                spriteRect2.y -= PLAYER_SPEED;
                if (currentFrame2 == 9 || currentFrame2 == 10)
                    currentFrame2++;
                else
                    currentFrame2 = 9;
            }
        }
        else if (keysPressed[5])
        {
            // Move down
            collision = encountersForbiddenTile(spriteRect2.x, spriteRect2.y + 26);
            if (collision == 0)
            {
                printf("Player 2: DOWN\n");
                spriteRect2.y += PLAYER_SPEED;
                if (currentFrame2 == 0 || currentFrame2 == 1)
                    currentFrame2++;
                else
                    currentFrame2 = 0;
            }
        }
        if (keysPressed[6])
        {
            // Move left
            collision = encountersForbiddenTile(spriteRect2.x - 5, spriteRect2.y);
            if (collision == 0)
            {
                printf("Player 2: LEFT\n");
                spriteRect2.x -= PLAYER_SPEED;
                if (currentFrame2 == 3 || currentFrame2 == 4)
                    currentFrame2++;
                else
                    currentFrame2 = 3;
            }
        }
        else if (keysPressed[7])
        {
            // Move right
            collision = encountersForbiddenTile(spriteRect2.x + 16, spriteRect2.y);
            if (collision == 0)
            {
                printf("Player 2: RIGHT\n");
                spriteRect2.x += PLAYER_SPEED;
                if (currentFrame2 == 6 || currentFrame2 == 7)
                    currentFrame2++;
                else
                    currentFrame2 = 6;
            }
        }

        if (speedBoostPerk.active && checkCollision(spriteRect, speedBoostPerk.rect) || speedBoostPerk.active && checkCollision(spriteRect2, speedBoostPerk.rect)) 
        {
            // Apply the speed boost effect to the player
            PLAYER_SPEED += 2; // Increase the speed
            speedBoostPerk.active = false; // Deactivate the perk
        }

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
        SDL_RenderCopyEx(renderer, spriteSheetTexture, &frameRects[currentFrame], &spriteRect, 0, NULL, flip);
        SDL_RenderCopyEx(renderer, spriteSheetTexture2, &frameRects2[currentFrame2], &spriteRect2, 0, NULL, flip2);
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

void loadMedia(SDL_Renderer *renderer, int playerNr, SDL_Texture **spriteSheetTexture, SDL_Rect frameRects[], SDL_Texture **tilesModule, SDL_Rect tilesGraphic[])
{

    // Load player sprite
    if (playerNr == 2)
    {
        SDL_Surface *spriteSheetSurface = IMG_Load("resources/Runner_1.PNG");
        *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
    }
    else if (playerNr == 1)
    {
        SDL_Surface *spriteSheetSurface = IMG_Load("resources/Hunter.PNG");
        *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
    }
    
    int frame_count = 0;
    for (int y = 0; y < 4; y++) 
    {
        for (int x = 0; x < 3; x++) 
        {
            frameRects[frame_count].x = x * (32) + 1; // 32 width/height
            frameRects[frame_count].y = y * (32) + 3;
            frameRects[frame_count].w = 32;
            frameRects[frame_count].h = 32;
            frame_count++;
        }
    }

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

bool checkCollision(SDL_Rect a, SDL_Rect b) // check perk collision
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}

void renderSpeedBoostPerk(SDL_Renderer *renderer) 
{
    if (speedBoostPerk.active)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &speedBoostPerk.rect);
    }
}