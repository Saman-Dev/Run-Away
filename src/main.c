#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "map/map.h"

// Sprite dimensions
#define FRAME_WIDTH 32
#define FRAME_HEIGHT 32
#define FRAME_COUNT 12

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#undef main

bool init(SDL_Renderer **renderer);
void loadMedia(SDL_Renderer *renderer, SDL_Texture **spriteSheetTexture, SDL_Rect frameRects[], SDL_Texture **tilesModule, SDL_Rect tilesGraphic[]);
void renderBackground(SDL_Renderer *renderer, SDL_Texture *mTile, SDL_Rect tilesGraphic[]);

int main(int argc, char *args[])
{

    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    bool quit = false;

    // Player
    SDL_Texture *spriteSheetTexture = NULL;
    SDL_Rect frameRects[12];
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect spriteRect = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    spriteRect.x = (640 - FRAME_WIDTH) / 2;  // Center horizontally
    spriteRect.y = (480 - FRAME_HEIGHT) / 2; // Center vertically
    int currentFrame = 6;

    // Background
    SDL_Texture *tilesModule = NULL;
    SDL_Rect tilesGraphic[16];

    if (init(&renderer))
    {
        printf("worked\n");
    }

    loadMedia(renderer, &spriteSheetTexture, frameRects, &tilesModule, tilesGraphic);

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
                    printf("UP\n");
                    // Increment the current currentFrame
                    if (currentFrame == 9 || currentFrame == 10)
                        currentFrame++;
                    else
                        currentFrame = 9;

                    spriteRect.y -= 4;
                    // Add a delay to slow down the animation
                    SDL_Delay(100);
                    break;
                case SDLK_DOWN:
                    printf("DOWN\n");
                    // Increment the current currentFrame
                    if (currentFrame == 0 || currentFrame == 1)
                        currentFrame++;
                    else
                        currentFrame = 0;

                    spriteRect.y += 4;
                    // Add a delay to slow down the animation
                    SDL_Delay(100);
                    break;
                case SDLK_LEFT:
                    printf("LEFT\n");
                    // Increment the current currentFrame
                    if (currentFrame == 3 || currentFrame == 4)
                        currentFrame++;
                    else
                        currentFrame = 3;

                    spriteRect.x -= 4;
                    // Add a delay to slow down the animation
                    SDL_Delay(100);
                    break;
                case SDLK_RIGHT:
                    printf("RIGHT\n");
                    // Increment the current currentFrame
                    if (currentFrame == 6 || currentFrame == 7)
                        currentFrame++;
                    else
                        currentFrame = 6;

                    spriteRect.x += 4;
                    // Add a delay to slow down the animation
                    SDL_Delay(100);
                    break;
                default:
                    break;
                }
            }
        }

        int foo = collidesWithImpassableTile(spriteRect.x, spriteRect.y);

        // Game renderer
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        renderBackground(renderer, tilesModule, tilesGraphic);
        SDL_RenderCopyEx(renderer, spriteSheetTexture, &frameRects[currentFrame], &spriteRect, 0, NULL, flip);
        SDL_RenderPresent(renderer);
    }

    return 0;
}

void renderBackground(SDL_Renderer *renderer, SDL_Texture *tilesModule, SDL_Rect tilesGraphic[])
{

    SDL_Rect possition;
    possition.y = 0;
    possition.x = 0;
    possition.h = getTileHeight();
    possition.w = getTileWidth();

    for (int i = 0; i < getTileColumns(); i++)
    {
        for (int j = 0; j < getTileRows(); j++)
        {
            possition.y = i * getTileHeight();
            possition.x = j * getTileWidth();
            SDL_RenderCopyEx(renderer, tilesModule, &tilesGraphic[getGridOfTiles(i, j)], &possition, 0, NULL, SDL_FLIP_NONE);
        }
    }
}

void loadMedia(SDL_Renderer *renderer, SDL_Texture **spriteSheetTexture, SDL_Rect frameRects[], SDL_Texture **tilesModule, SDL_Rect tilesGraphic[])
{

    SDL_Surface *spriteSheetSurface = IMG_Load("resources/Runner_1.PNG");
    *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);

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
        tilesGraphic[i].x = i * getTileWidth();
        tilesGraphic[i].y = 0;
        tilesGraphic[i].w = getTileWidth();
        tilesGraphic[i].h = getTileHeight();
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
