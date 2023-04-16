#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "player/player.h"

// Sprite dimensions
#define FRAME_WIDTH 32
#define FRAME_HEIGHT 32
#define FRAME_COUNT 12

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char *argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window and renderer
    SDL_Window *window = SDL_CreateWindow("Sprite walking test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the sprite sheet image
    SDL_Surface *spriteSheetSurface = IMG_Load("resources/Runner_1.png");
    SDL_Texture *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
    SDL_FreeSurface(spriteSheetSurface);

    // Initialize the source rectangles for each frame of animation
    SDL_Rect frameRects[FRAME_COUNT];
    frameRects[0] = (SDL_Rect){1, 3, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[1] = (SDL_Rect){33, 3, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[2] = (SDL_Rect){66, 3, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[3] = (SDL_Rect){1, 35, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[4] = (SDL_Rect){33, 35, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[5] = (SDL_Rect){66, 35, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[6] = (SDL_Rect){1, 67, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[7] = (SDL_Rect){33, 67, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[8] = (SDL_Rect){66, 67, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[9] = (SDL_Rect){1, 99, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[10] = (SDL_Rect){33, 99, FRAME_WIDTH, FRAME_HEIGHT};
    frameRects[11] = (SDL_Rect){66, 99, FRAME_WIDTH, FRAME_HEIGHT};

    // Initialize variables for the main loop
    bool quit = false;
    SDL_Event event;
    const int FPS = 60;
    int currentFrame = 0;
    SDL_Rect spriteRect = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    spriteRect.x = (640 - FRAME_WIDTH) / 2;  // Center horizontally
    spriteRect.y = (480 - FRAME_HEIGHT) / 2; // Center vertically

    // Main loop
    while (!quit)
    {
        // Handle game events
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
                    // Increment the current frame
                    if (currentFrame == 9 || currentFrame == 10)
                        currentFrame++;
                    else
                        currentFrame = 9;

                    spriteRect.y -= 4;
                    // Add a delay to slow down the animation
                    SDL_Delay(100);
                    break;
                case SDLK_DOWN:
                    // Increment the current frame
                    if (currentFrame == 0 || currentFrame == 1)
                        currentFrame++;
                    else
                        currentFrame = 0;

                    spriteRect.y += 4;
                    // Add a delay to slow down the animation
                    SDL_Delay(100);
                    break;
                case SDLK_LEFT:
                    // Increment the current frame
                    if (currentFrame == 3 || currentFrame == 4)
                        currentFrame++;
                    else
                        currentFrame = 3;

                    spriteRect.x -= 4;
                    // Add a delay to slow down the animation
                    SDL_Delay(100);
                    break;
                case SDLK_RIGHT:
                    // Increment the current frame
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

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the sprite
        SDL_RenderCopy(renderer, spriteSheetTexture, &frameRects[currentFrame], &spriteRect);

        // Update the screen
        SDL_RenderPresent(renderer);
        // Delay to control the frame rate
        SDL_Delay(1000 / FPS);
    }

    // Clean up resources
    SDL_DestroyTexture(spriteSheetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
