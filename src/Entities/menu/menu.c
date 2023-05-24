#include "menu.h"

void manageMenu(Framework *game, Network *information, TCPLocalInformation *TCPInformation, ClientID record[]) {
    int scene = 0;
    while (game->menuState) {
        if (scene == 0) {
            handleMenuEntry(&scene, game);
        }
        else if (scene == 1) {
            handleHostGameOption(&scene, game, information, TCPInformation, record);
        }
        else if (scene == 2) {
            handleJoinGameOption(game, information, TCPInformation);
        }
        else if (scene == 3) {
            handleSettingsOption(&scene, game);
        }
    }

/*
    else if (*state == LOBBY) {
        selectedBox = displayOptions(game, menu);
        switch (selectedBox) {
        case 4:
            *state = ONGOING;
            break;
        case 5:
            *state = START;
            printf("Disconnected");
            SDLNet_UDP_Close(information->sourcePort);
            SDLNet_TCP_Close(TCPInformation->socket);
            break;
        }
    }
    else if (*state == GAME_OVER) {
        selectedBox = displayOptions(game, menu);
        switch (selectedBox) {
        case 0:
            *state = START;
            SDLNet_UDP_Close(information->sourcePort);
            SDLNet_TCP_Close(TCPInformation->socket);
            break;
        case 1:
            game->quit = true;
            break;
        default:
            break;
        }
    }
    return selectedBox;
    */
}

static int displayOptions(Framework *game, Menu *menu) {
    SDL_Rect textBoxRectangle[MAX_NUMBER_OF_TEXT_BOXES+1];
    SDL_Texture *textBoxTexture[MAX_NUMBER_OF_TEXT_BOXES+1];
    SDL_Texture *backgroundTexture = IMG_LoadTexture(game->renderer, menu->imageFilePath);
    SDL_Point mousePosition;

    prepareTextBoxesToBeShown(game, menu, textBoxRectangle, textBoxTexture);

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
                            playMenuClickSound();
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

static void prepareTextBoxesToBeShown(Framework *game, Menu *menu, SDL_Rect textBoxRectangle[], SDL_Texture *textBoxTexture[]) {
    SDL_Surface *textBoxSurface[MAX_NUMBER_OF_TEXT_BOXES+1];
    int largestTextBoxWidth = 0;
    int accumulatedTextBoxHeight = 0;

    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxSurface[i] = TTF_RenderText_Solid(game->font, menu->options[i], game->white);
        textBoxRectangle[i].y = accumulatedTextBoxHeight;
        textBoxRectangle[i].w = textBoxSurface[i]->w;
        textBoxRectangle[i].h = textBoxSurface[i]->h;
        accumulatedTextBoxHeight += textBoxRectangle[i].h + menu->optionSpacing;
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
}

static void handleMenuEntry(int *scene, Framework *game) {
    Menu menu;
    char *menuOptions[] = { "Host Game", "Join Game", "Settings", "Quit", "\0"};
    menu.options = menuOptions;
    menu.optionSpacing = 60;
    strcpy(menu.imageFilePath, "resources/start_menu.png");

    int selectedBox = displayOptions(game, &menu);
    switch (selectedBox) {
        case 0:
            *scene = 1;
            break;
        case 1:
            *scene = 2;
            break;
        case 2:
            *scene = 3;
            break;
        case 3:
            game->menuState = false;
            game->quit = true;
            break;
        default:
            break;
    }
}

static void handleHostGameOption(int *scene, Framework *game, Network *information, TCPLocalInformation *TCPInformation, ClientID record[]) {
    initiateServerTCPCapability(TCPInformation);
    TCPInformation->playerNumber = -1;
    setUpServer(information, record, 2000);

    /*
    Menu menu;
    char *lobbyOptions[] = { "Not connected", "Not connected", "Not connected", "Not connected", "Play", "Back", "\0"};
    menu.options = lobbyOptions;
    menu.optionSpacing = 20;
    strcpy(menu.imageFilePath, "resources/lobby_menu.png");

    int selectedBox = displayOptions(game, &menu);
    if (selectedBox == 5) {
        *scene = 0;
    }
    else {
    }
    */
    game->menuState = false;
    changeThemeSong();
}

static void handleJoinGameOption(Framework *game, Network *information, TCPLocalInformation *TCPInformation) {
    InitiateClientTCPCapability(TCPInformation);
    setUpClient(information, "127.0.0.1", 2000);
    changeThemeSong();
    game->menuState = false;
}

static void handleSettingsOption(int *scene, Framework *game) {
    Menu menu;
    char *settingsOptions[] = { "Mute Song", "Back to Menu", "\0"};
    menu.options = settingsOptions;
    menu.optionSpacing = 240;
    strcpy(menu.imageFilePath, "resources/settings_menu.png");
    
    int selectedBox = displayOptions(game, &menu);
    switch (selectedBox) {
        case 0:
            if (!game->isMuted) {
                Mix_VolumeMusic(0);
                game->isMuted = true;
            }
            else {
                Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
                game->isMuted = false;
            }
            break;
        case 1:
            *scene = 0;
            break;
        default:
            break;
    }
}