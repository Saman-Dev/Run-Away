#include "menu.h"

int manageMenu(Framework *game, Menu *menu, Network *information, TCPLocalInformation *TCPInformation, GameState *state, ClientID record[]) {
    int selectedOption;
    if (*state == START) {
        selectedOption = displayOptions(game, menu);
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
    }
    else if (*state == SETTINGS) {
        selectedOption = displayOptions(game, menu);
        switch (selectedOption) {
        case 0:
            playMenuClickSound();
            game->isMuted = !game->isMuted;
            if (game->isMuted) {
                Mix_VolumeMusic(0);
            }
            else {
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
    }
    else if (*state == LOBBY) {
        selectedOption = displayOptions(game, menu);
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
    }
    else if (*state == GAME_OVER) {
        selectedOption = displayOptions(game, menu);
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

int displayOptions(Framework *game, Menu *menu) {
    int largestTextBoxWidth = 0;
    int totalTextBoxHeight = 0;
    SDL_Surface *textBoxSurface[MAX_NUMBER_OF_TEXT_BOXES+1];
    SDL_Rect textBoxRectangle[MAX_NUMBER_OF_TEXT_BOXES+1];
    SDL_Texture *textBoxTexture[MAX_NUMBER_OF_TEXT_BOXES+1];
    SDL_Texture *backgroundTexture = IMG_LoadTexture(game->renderer, menu->imageFilePath);
    SDL_Point mousePosition;

    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxSurface[i] = TTF_RenderText_Solid(game->font, menu->options[i], game->white);
        textBoxRectangle[i].y = totalTextBoxHeight;
        textBoxRectangle[i].w = textBoxSurface[i]->w;
        textBoxRectangle[i].h = textBoxSurface[i]->h;
        totalTextBoxHeight += textBoxRectangle[i].h + menu->optionSpacing;
        if (textBoxRectangle[i].w > largestTextBoxWidth) {
            largestTextBoxWidth = textBoxRectangle[i].w;
        }
    }
    
    int menuX = (SCREEN_WIDTH / 2) - (largestTextBoxWidth / 2);
    int menuY = (SCREEN_HEIGHT / 2) - 52;
    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxRectangle[i].x = menuX + (largestTextBoxWidth - textBoxRectangle[i].w) / 2;
        textBoxRectangle[i].y = menuY + textBoxRectangle[i].y;
    }

    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxTexture[i] = SDL_CreateTextureFromSurface(game->renderer, textBoxSurface[i]);
        SDL_FreeSurface(textBoxSurface[i]);
    }

    int selectedOption = -1;
    while (selectedOption == -1) {
        while (SDL_PollEvent(&game->event)) {
            switch (game->event.type) {
                case SDL_QUIT:
                    selectedOption = 1; // FIX!
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
                    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
                        if (SDL_PointInRect(&mousePosition, &textBoxRectangle[i])) {
                            selectedOption = i;
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_RenderCopy(game->renderer, backgroundTexture, NULL, NULL);
        for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
            SDL_RenderCopy(game->renderer, textBoxTexture[i], NULL, &textBoxRectangle[i]);
        }
        SDL_RenderPresent(game->renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        SDL_DestroyTexture(textBoxTexture[i]);
    }

    return selectedOption;
}