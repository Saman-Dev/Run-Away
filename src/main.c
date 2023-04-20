#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "map/map.h"
#include <SDL2/SDL_mixer.h>

// Sprite dimensions
#define FRAME_WIDTH 32
#define FRAME_HEIGHT 32
#define FRAME_COUNT 12

// Screen dimensions
#define SCREEN_WIDTH 810
#define SCREEN_HEIGHT 800

#undef main

bool init(SDL_Renderer **renderer);
void loadMedia(SDL_Renderer *renderer, int playerNr, SDL_Texture **spriteSheetTexture, SDL_Rect frameRects[], SDL_Texture **tilesModule, SDL_Rect tilesGraphic[]);
void renderBackground(SDL_Renderer *renderer, SDL_Texture *mTile, SDL_Rect tilesGraphic[]);

int main(int argc, char *args[])
{

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

    //Hunter
    SDL_Texture *spriteSheetTexture2 = NULL;
    SDL_Rect frameRects2[12];
    SDL_RendererFlip flip2 = SDL_FLIP_NONE;
    SDL_Rect spriteRect2 = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    spriteRect2.x = (640 - FRAME_WIDTH) / 2;  // Center horizontally
    spriteRect2.y = (480 - FRAME_HEIGHT) / 2; // Center vertically
    int currentFrame2 = 6;


    // Background
    SDL_Texture *tilesModule = NULL;
    SDL_Rect tilesGraphic[16];

    //------------------------------------------------------------------------------------------------------------------
    // Initialize SDL2 Mixer library
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer: %s", Mix_GetError());
        return -1;
    }

    // Load OGG file
    Mix_Music *music = Mix_LoadMUS("resources/main_theme.wav");
    if (!music)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load OGG file: %s", Mix_GetError());
        Mix_CloseAudio();
        return -1;
    }

    // Set the music volume to 20%
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);

    // Play music in a loop
    Mix_PlayMusic(music, -1);
    //------------------------------------------------------------------------------------------------------------------

    if (init(&renderer))
    {
        printf("worked\n");
    }

    int playerNr = 1;
    loadMedia(renderer, playerNr, &spriteSheetTexture, frameRects, &tilesModule, tilesGraphic);
    loadMedia(renderer, 2, &spriteSheetTexture2, frameRects2, &tilesModule, tilesGraphic);


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
                    case SDLK_UP:
                        collision = encountersForbiddenTile(spriteRect2.x, spriteRect2.y-5);
                        if (collision == 0)
                        {
                            printf("Player 1: UP\n");
                            // Increment the current frame
                            if (currentFrame2 == 9 || currentFrame2 == 10)
                                currentFrame2++;
                            else
                                currentFrame2 = 9;

                            spriteRect2.y -= 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_DOWN:
                        collision = encountersForbiddenTile(spriteRect2.x, spriteRect2.y+26);
                        if (collision == 0)
                        {
                            printf("Player 1: DOWN\n");
                            // Increment the current frame
                            if (currentFrame2 == 0 || currentFrame2 == 1)
                                currentFrame2++;
                            else
                                currentFrame2 = 0;

                            spriteRect2.y += 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_LEFT:
                        collision = encountersForbiddenTile(spriteRect2.x-5, spriteRect2.y);
                        if (collision == 0)
                        {
                            printf("Player 1: LEFT\n");
                            // Increment the current frame
                            if (currentFrame2 == 3 || currentFrame2 == 4)
                                currentFrame2++;
                            else
                                currentFrame2 = 3;

                            spriteRect2.x -= 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_RIGHT:
                        collision = encountersForbiddenTile(spriteRect2.x+16, spriteRect2.y);
                        if (collision == 0)
                        {
                            printf("Player 1: RIGHT\n");
                            // Increment the current frame
                            if (currentFrame2 == 6 || currentFrame2 == 7)
                                currentFrame2++;
                            else
                                currentFrame2 = 6;

                            spriteRect2.x += 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_w:
                        collision = encountersForbiddenTile(spriteRect.x, spriteRect.y-5);
                        if (collision == 0)
                        {
                            printf("UP\n");
                            // Increment the current currentFrame
                            if (currentFrame == 9 || currentFrame == 10)
                                currentFrame++;
                            else
                                currentFrame = 9;

                            spriteRect.y -= 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_s:
                        collision = encountersForbiddenTile(spriteRect.x, spriteRect.y+26);
                        if (collision == 0)
                        {
                            printf("DOWN\n");
                            // Increment the current currentFrame
                            if (currentFrame == 0 || currentFrame == 1)
                                currentFrame++;
                            else
                                currentFrame = 0;

                            spriteRect.y += 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_a:
                        collision = encountersForbiddenTile(spriteRect.x-5, spriteRect.y);
                        if (collision == 0)
                        {
                            printf("LEFT\n");
                            // Increment the current currentFrame
                            if (currentFrame == 3 || currentFrame == 4)
                                currentFrame++;
                            else
                                currentFrame = 3;

                            spriteRect.x -= 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_d:
                        collision = encountersForbiddenTile(spriteRect.x+16, spriteRect.y);
                        if (collision == 0)
                        {
                            printf("RIGHT\n");
                            // Increment the current currentFrame
                            if (currentFrame == 6 || currentFrame == 7)
                                currentFrame++;
                            else
                                currentFrame = 6;

                            spriteRect.x += 4;
                            // Add a delay to slow down the animation
                            SDL_Delay(100);
                        }
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
        }

        // Game renderer
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        renderBackground(renderer, tilesModule, tilesGraphic);
        SDL_RenderCopyEx(renderer, spriteSheetTexture, &frameRects[currentFrame], &spriteRect, 0, NULL, flip);
        SDL_RenderCopyEx(renderer, spriteSheetTexture2, &frameRects2[currentFrame], &spriteRect2, 0, NULL, flip2);
        SDL_RenderPresent(renderer);
    }

    //-------------------------------
    // Free resources and close SDL
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    //-------------------------------
    return 0;
}

void renderBackground(SDL_Renderer *renderer, SDL_Texture *tilesModule, SDL_Rect tilesGraphic[])
{

    SDL_Rect possition;
    possition.y = 0;
    possition.x = 0;
    possition.h = getTheHeightOfTheTile();
    possition.w = getTheWidthOfTheTile();

    for (int i = 0; i < getNumberOfColumnsInMap(); i++)
    {
        for (int j = 0; j < getNumberOfRowsInMap(); j++)
        {
            possition.y = i * getTheHeightOfTheTile();
            possition.x = j * getTheWidthOfTheTile();
            SDL_RenderCopyEx(renderer, tilesModule, &tilesGraphic[getTileInformation(i, j)], &possition, 0, NULL, SDL_FLIP_NONE);
        }
    }
}

void loadMedia(SDL_Renderer *renderer, int playerNr, SDL_Texture **spriteSheetTexture, SDL_Rect frameRects[], SDL_Texture **tilesModule, SDL_Rect tilesGraphic[])
{

    // Load player sprite
    if (playerNr == 2)
    {
        SDL_Surface *spriteSheetSurface = IMG_Load("resources/Runner_2.PNG");
        *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
    }
    else if (playerNr == 1)
    {
        SDL_Surface *spriteSheetSurface = IMG_Load("resources/Hunter.PNG");
        *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
    }
        frameRects[0].x = 1;
        frameRects[0].y = 3;
        frameRects[0].w = 32;
        frameRects[0].h = 32;

        frameRects[1].x = 33;
        frameRects[1].y = 3;
        frameRects[1].w = 32;
        frameRects[1].h = 32;

        frameRects[2].x = 66;
        frameRects[2].y = 3;
        frameRects[2].w = 32;
        frameRects[2].h = 32;

        frameRects[3].x = 1;
        frameRects[3].y = 35;
        frameRects[3].w = 32;
        frameRects[3].h = 32;

        frameRects[4].x = 33;
        frameRects[4].y = 35;
        frameRects[4].w = 32;
        frameRects[4].h = 32;

        frameRects[5].x = 66;
        frameRects[5].y = 35;
        frameRects[5].w = 32;
        frameRects[5].h = 32;

        frameRects[6].x = 1;
        frameRects[6].y = 67;
        frameRects[6].w = 32;
        frameRects[6].h = 32;

        frameRects[7].x = 33;
        frameRects[7].y = 67;
        frameRects[7].w = 32;
        frameRects[7].h = 32;

        frameRects[8].x = 66;
        frameRects[8].y = 67;
        frameRects[8].w = 32;
        frameRects[8].h = 32;

        frameRects[9].x = 1;
        frameRects[9].y = 99;
        frameRects[9].w = 32;
        frameRects[9].h = 32;

        frameRects[10].x = 33;
        frameRects[10].y = 99;
        frameRects[10].w = 32;
        frameRects[10].h = 32;

        frameRects[11].x = 66;
        frameRects[11].y = 99;
        frameRects[11].w = 32;
        frameRects[11].h = 32;

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
    SDL_Init(SDL_INIT_VIDEO);
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