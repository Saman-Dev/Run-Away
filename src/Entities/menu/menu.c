#include "menu.h"

int manageMenu(Framework *game, Menu* menu, Network *information, TCPLocalInformation *TCPInformation, GameState *state, ClientID record[]) {
    int selectedOption;
    if (*state == START){
        selectedOption = displayMenu(game->renderer, menu);
        switch (selectedOption) {
            case 0:
                playMenuClickSound();
                setUpServer(information, record, 2000);
                *state = ONGOING;
                break;
            case 1:
                playMenuClickSound();
                setUpClient(information, "127.0.0.1", 2000);
                *state = ONGOING;
                break;
            case 2:
                playMenuClickSound();
                *state = SETTINGS;
                break;
            case 3:    
                game->quit = true;
                break;
            default:
                break;
        }
    }else if (*state == SETTINGS){
        selectedOption = displayMenu(game->renderer, menu);
        switch (selectedOption) {
            case 0:
                playMenuClickSound();
                game->isMuted = !game->isMuted;
                if (game->isMuted) {
                    Mix_VolumeMusic(0);
                } else {
                    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
                }
                break;
            case 1:
                playMenuClickSound();
                *state = START;
                break;
            default:
                break;
        }
    }else if (*state == LOBBY){
        selectedOption = displayMenu(game->renderer, menu);
        switch (selectedOption) {
            case 4:
                playMenuClickSound();
                *state = ONGOING;
                break;
            case 5:
                playMenuClickSound();
                *state = START;
                printf("Disconnected");
                SDLNet_UDP_Close(information->sourcePort);
                SDLNet_TCP_Close(TCPInformation->socket);
                break;
        }
    }else if (*state == GAME_OVER){
        selectedOption = displayMenu(game->renderer, menu);
        switch (selectedOption) {
            case 0:
                playMenuClickSound();
                *state = START;
                SDLNet_UDP_Close(information->sourcePort);
                SDLNet_TCP_Close(TCPInformation->socket);
                break;
            case 1:
                playMenuClickSound();
                game->quit = true;
                break;
            default:
                break;
        }
    }
    return selectedOption;
}

int displayMenu(SDL_Renderer* renderer, Menu* menu) 
{
    TTF_Font* font = TTF_OpenFont("resources/font.ttf", 24);
    SDL_Surface* optionSurfaces[menu->numOptions];
    SDL_Rect optionRects[menu->numOptions];

    SDL_Color textColor = {255, 255, 255};
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
    int maxOptionWidth = 0;
    int totalOptionHeight = 0;
    
    for (int i = 0; i < menu->numOptions; i++) 
    {
        optionSurfaces[i] = TTF_RenderText_Solid(font, menu->options[i], textColor);
        optionRects[i].x = 0;
        optionRects[i].y = totalOptionHeight;
        optionRects[i].w = optionSurfaces[i]->w;
        optionRects[i].h = optionSurfaces[i]->h;
        totalOptionHeight += optionRects[i].h + menu->optionSpacing;
        if (optionRects[i].w > maxOptionWidth) {
            maxOptionWidth = optionRects[i].w;
        }
    }

    SDL_Texture* imageTexture = IMG_LoadTexture(renderer, menu->img); 

    int menuWidth = maxOptionWidth + menu->optionSpacing * 2;
    int menuHeight = totalOptionHeight - menu->optionSpacing;
    int menuX = menu->menuX;//(SCREEN_WIDTH - menuWidth) / 2 + 200; // Adjust center position to the side
    int menuY = menu->menuY;//(SCREEN_HEIGHT - menuHeight) / 2;

    for (int i = 0; i < menu->numOptions; i++) 
    {
        optionRects[i].x = menuX + (maxOptionWidth - optionRects[i].w) / 2;
        optionRects[i].y = menuY + optionRects[i].y;
    }

    int mouseX=0, mouseY=0; 
    int selectedOption = -1;
    SDL_Event event;
    
    while (selectedOption == -1) 
    {
        while (SDL_PollEvent(&event)) 
        {
            
            switch (event.type) 
            {
                case SDL_QUIT:
                    selectedOption = menu->numOptions - 1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    
                    SDL_GetMouseState(&mouseX, &mouseY);
                    for (int i = 0; i < menu->numOptions; i++) 
                    {
                        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &optionRects[i])) 
                        {
                            selectedOption = i;
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, imageTexture, NULL, NULL);

        for (int i = 0; i < menu->numOptions; i++) 
        {
            SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(renderer, optionSurfaces[i]);
            SDL_RenderCopy(renderer, optionTexture, NULL, &optionRects[i]);
            SDL_DestroyTexture(optionTexture);
        }

        SDL_RenderPresent(renderer);
    }

    

    for (int i = 0; i < menu->numOptions; i++) 
    {
        SDL_FreeSurface(optionSurfaces[i]);
    }

    TTF_CloseFont(font);

    return selectedOption;
}