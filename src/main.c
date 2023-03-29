#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

// Define pixels/second
#define SCROLL_SPEED (30) // pixels/second
#define SPEED 200

// Define screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argv, char **args)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *pWindow = SDL_CreateWindow("Jaga muspekaren", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!pWindow)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!pRenderer)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Surface *pSurface = IMG_Load("resources/Ship.png");
    if (!pSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Rect shipRect;
    SDL_QueryTexture(pTexture, NULL, NULL, &shipRect.w, &shipRect.h);
    shipRect.w /= 4;
    shipRect.h /= 4;

    float shipX = (SCREEN_WIDTH - shipRect.w) / 2;  // left side
    float shipY = (SCREEN_HEIGHT - shipRect.h) / 2; // upper side
    float shipVelocityX = 0;
    float shipVelocityY = 0;

    bool close_requested = false;

    while (!close_requested)
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                close_requested = true;
        }

        int mouseX, mouseY;
        int buttons = SDL_GetMouseState(&mouseX, &mouseY);

        int deltaX = mouseX - (shipX + shipRect.w / 2); // compare to center of ship
        int deltaY = mouseY - (shipY + shipRect.h / 2);
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distance < 5)
        {
            shipVelocityX = shipVelocityY = 0;
        }
        else
        {
            shipVelocityX = deltaX * SPEED / distance;
            shipVelocityY = deltaY * SPEED / distance;
        }
        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            shipVelocityX = -shipVelocityX;
            shipVelocityY = -shipVelocityY;
        }
        shipX += shipVelocityX / 60;
        shipY += shipVelocityY / 60;
        if (shipX < 0)
            shipX = 0;
        if (shipY < 0)
            shipY = 0;
        if (shipX > SCREEN_WIDTH - shipRect.w)
            shipX = SCREEN_WIDTH - shipRect.w;
        if (shipY > SCREEN_HEIGHT - shipRect.h)
            shipY = SCREEN_HEIGHT - shipRect.h;
        shipRect.x = shipX;
        shipRect.y = shipY;
        SDL_SetRenderDrawColor(pRenderer, 0, 255, 255, 255);
        SDL_RenderClear(pRenderer);
        SDL_RenderCopy(pRenderer, pTexture, NULL, &shipRect);
        SDL_RenderPresent(pRenderer);
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(pTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}