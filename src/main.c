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

SpeedBoostPerk speedBoostPerk;

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

bool init(SDL_Renderer **renderer);
void loadMedia(SDL_Renderer *renderer, int playerNr, SDL_Texture **spriteSheetTexture, SDL_Rect frameRects[], SDL_Texture **tilesModule, SDL_Rect tilesGraphic[]);
void renderBackground(SDL_Renderer *renderer, SDL_Texture *mTile, SDL_Rect tilesGraphic[]);
void renderSpeedBoostPerk(SDL_Renderer *renderer);
bool checkPerkCollision(SDL_Rect a, SDL_Rect b);


int main(int argc, char *args[])
{

    srand(time(NULL));
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    bool quit = false;

    int collision = 0;

    Player player1;

    // Player
    SDL_Texture *spriteSheetTexture = NULL;
    SDL_Rect frameRects[12];
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect spriteRect = {0, 0, PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT};
    spriteRect.x = (640 - PLAYER_FRAME_WIDTH) / 2;  // Center horizontally
    spriteRect.y = (480 - PLAYER_FRAME_HEIGHT) / 2; // Center vertically
    int currentFrame = 6;

    // Hunter
    SDL_Texture *spriteSheetTexture2 = NULL;
    SDL_Rect frameRects2[12];
    SDL_RendererFlip flip2 = SDL_FLIP_NONE;
    SDL_Rect spriteRect2 = {0, 0, PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT};
    spriteRect2.x = (600 - PLAYER_FRAME_WIDTH) / 2;  // Center horizontally
    spriteRect2.y = (400 - PLAYER_FRAME_HEIGHT) / 2; // Center vertically
    int currentFrame2 = 6;

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

    int playerNr = 1;
    loadMedia(renderer, playerNr, &spriteSheetTexture, frameRects, &tilesModule, tilesGraphic);
    // loadMedia(renderer, 2, &spriteSheetTexture2, frameRects2, &tilesModule, tilesGraphic);

    // Boolean array to keep track of which keys are pressed
    bool keysPressed[8] = {false, false, false, false, false, false, false, false};

    // Movement speed of the player
    int PLAYER_SPEED = 2;

    //initSpeedBoostPerk(); // Initialize the speed boost perk

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
        if (player1.up)
        {
            // Move up
            collision = encountersForbiddenTile(player1.position.x, player1.position.y - 5);
            if (collision == 0)
            {
                printf("Player 2: Up\n");
                player1.position.y -= player1.speed;
                if (player1.frame == 9 || player1.frame == 10)
                    player1.frame++;
                else
                    player1.frame = 9;
            }
        }
        else if (player1.down)
        {
            // Move down
            collision = encountersForbiddenTile(player1.position.x, player1.position.y + 26);
            if (collision == 0)
            {
                printf("Player 2: DOWN\n");
                player1.position.y += player1.speed;
                if (player1.frame == 0 || player1.frame == 1)
                    player1.frame++;
                else
                    player1.frame = 0;
            }
        }
        if (player1.left)
        {
            // Move left
            collision = encountersForbiddenTile(player1.position.x - 5, player1.position.y);
            if (collision == 0)
            {
                printf("Player 2: Left\n");
                player1.position.x -= player1.speed;
                if (player1.frame == 3 || player1.frame == 4)
                    player1.frame++;
                else
                    player1.frame = 3;
            }
        }
        else if (player1.right)
        {
            // Move right
            collision = encountersForbiddenTile(player1.position.x + 16, player1.position.y);
            if (collision == 0)
            {
                printf("Player 2: Right\n");
                player1.position.x += player1.speed;
                if (player1.frame == 6 || player1.frame == 7)
                    player1.frame++;
                else
                    player1.frame = 6;
            }
        }

        if (speedBoostPerk.active && checkPerkCollision(spriteRect, speedBoostPerk.rect) || speedBoostPerk.active && checkPerkCollision(spriteRect2, speedBoostPerk.rect)) 
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
        SDL_RenderCopyEx(renderer, player1.spriteSheetTexture, &player1.spriteClip[player1.frame], &player1.position, 0, NULL, SDL_FLIP_NONE);
        
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

    speedBoostPerk.texture = IMG_LoadTexture(renderer, "resources/perk.png");
    
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
    };

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