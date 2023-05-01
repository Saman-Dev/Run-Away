#include "menu.h"
#include <SDL2/SDL_ttf.h>

int displayMenu(SDL_Renderer* renderer, Menu* menu) 
{
    TTF_Font* font = TTF_OpenFont("resources/font.ttf", 24);
    SDL_Surface* optionSurfaces[menu->numOptions];
    SDL_Rect optionRects[menu->numOptions];

    SDL_Color textColor = {255, 255, 255};
    for (int i = 0; i < menu->numOptions; i++) 
    {
        optionSurfaces[i] = TTF_RenderText_Solid(font, menu->options[i], textColor);
        optionRects[i].x = menu->menuX;
        optionRects[i].y = menu->menuY + (i * (menu->optionHeight + menu->optionSpacing));
        optionRects[i].w = menu->optionWidth;
        optionRects[i].h = menu->optionHeight;
    }
    int mouseX=0, mouseY=0; 
    int selectedOption = -1;
    SDL_Event event;
    
    while (selectedOption == -1) {
        while (SDL_PollEvent(&event)) {
            
            switch (event.type) {
                case SDL_QUIT:
                    selectedOption = menu->numOptions - 1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    
                    SDL_GetMouseState(&mouseX, &mouseY);
                    for (int i = 0; i < menu->numOptions; i++) {
                        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &optionRects[i])) {
                            selectedOption = i;
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < menu->numOptions; i++) {
            SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(renderer, optionSurfaces[i]);
            SDL_RenderCopy(renderer, optionTexture, NULL, &optionRects[i]);
            SDL_DestroyTexture(optionTexture);
        }

        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < menu->numOptions; i++) {
        SDL_FreeSurface(optionSurfaces[i]);
    }

    TTF_CloseFont(font);

    return selectedOption;
}
