#include "menu.h"

void manageMenu(Framework *game, NetworkBundle *networkData, Player players[]) {
    int scene = 0;
    while (game->menuState) {
        if (scene == 0) {
            handleMenuEntry(&scene, game);
        }
        else if (scene == 1) {
            handleHostGameOption(&scene, game, networkData, players);
        }
        else if (scene == 2) {
            handleJoinGameOption(&scene, game, networkData, players);
        }
        else if (scene == 3) {
            handleSettingsOption(&scene, game);
        }
    }
}

static int displayOptions(Framework *game, Menu *menu, NetworkBundle *networkData, Player players[]) {
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
                    game->menuState = false;
                    game->quit = true;
                    selectedOption = -2;
                    break;
                case SDL_MOUSEBUTTONDOWN:
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

        if (menu->networkFunctionality) {
            manageNetwork(networkData, players);
            updateBoxContent(game, menu, textBoxRectangle, textBoxTexture, networkData);
            if (!networkData->TCPInformation.inLobby) {
                return -2;
            }
        }
    }

    SDL_DestroyTexture(backgroundTexture);
    destroyBoxTextures(menu, textBoxTexture);

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
        accumulatedTextBoxHeight += textBoxRectangle[i].h + SPACING_BETWEEN_TEXT_BOXES;
        if (textBoxRectangle[i].w > largestTextBoxWidth) {
            largestTextBoxWidth = textBoxRectangle[i].w;
        }
    }
    
    int menuX = (SCREEN_WIDTH / 2) - (largestTextBoxWidth / 2);
    int menuY = (SCREEN_HEIGHT / 2) - LOCATION_ADJUSTMENT;
    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxRectangle[i].x = menuX + (largestTextBoxWidth - textBoxRectangle[i].w) / 2;
        textBoxRectangle[i].y = menuY + textBoxRectangle[i].y;
    }

    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxTexture[i] = SDL_CreateTextureFromSurface(game->renderer, textBoxSurface[i]);
        SDL_FreeSurface(textBoxSurface[i]);
    }
}

static void destroyBoxTextures(Menu *menu, SDL_Texture *textBoxTexture[]) {
    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
    SDL_DestroyTexture(textBoxTexture[i]);
    }
}

static void updateBoxContent(Framework *game, Menu *menu, SDL_Rect textBoxRectangle[], SDL_Texture *textBoxTexture[], NetworkBundle *network) {
    static int connectedPlayersLastTime = 0;
    int connectedPlayersNow = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (network->TCPRecord[i].active) {
            connectedPlayersNow++;
        }
    }
    if (connectedPlayersLastTime != connectedPlayersNow) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (network->TCPRecord[i].active) {
                menu->options[i+1] = "Player connected";
            }
            else {
                menu->options[i+1] = "Spot Available";
            }
        }
        connectedPlayersLastTime = connectedPlayersNow;
        destroyBoxTextures(menu, textBoxTexture);
        prepareTextBoxesToBeShown(game, menu, textBoxRectangle, textBoxTexture);
    }
}

static void handleMenuEntry(int *scene, Framework *game) {
    Menu menu = {{"Host Game", "Join Game", "Settings", "Quit", "\0"}, {"resources/start_menu.png"}, false};
    int selectedBox = displayOptions(game, &menu, NULL, NULL);
    switch (selectedBox) {
        case 0:
            playMenuClickSound();
            *scene = 1;
            break;
        case 1:
            playMenuClickSound();
            *scene = 2;
            break;
        case 2:
            playMenuClickSound();
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

static void handleHostGameOption(int *scene, Framework *game, NetworkBundle *networkData, Player players[]) {
    initiateServerTCPCapability(&networkData->TCPInformation);
    setUpServer(&networkData->UDPInformation, networkData->UDPRecord, 2000);

    Menu menu = {{"Host Connected", "Spot Available", "Spot Available", "Spot Available", "Spot Available", "Play", "Back", "\0"}, {"resources/lobby_menu.png"}, true};
    int selectedBox = 0;
    while (selectedBox != 5 && selectedBox != 6) {
        selectedBox = displayOptions(game, &menu, networkData, players);
    }
    if (selectedBox == 5) {
        playMenuClickSound();
        changeThemeSong();
        networkData->TCPInformation.inLobby = false;
        game->menuState = false;
    }
    else {
        playMenuClickSound();
        *scene = 0;
    }
}

static void handleJoinGameOption(int *scene, Framework *game, NetworkBundle *networkData, Player players[]) {
    InitiateClientTCPCapability(&networkData->TCPInformation, networkData->TCPRecord);
    setUpClient(&networkData->UDPInformation, "127.0.0.1", 2000);

    if (networkData->TCPInformation.inLobby) {
        Menu menu = {{"Host Connected", "Spot Available", "Spot Available", "Spot Available", "Spot Available", " ", "Back", "\0"}, {"resources/lobby_menu.png"}, true};
        int selectedBox = 0;
        while (selectedBox != 6 && networkData->TCPInformation.inLobby) {
            selectedBox = displayOptions(game, &menu, networkData, players);
        }
        if (selectedBox == 6) {
            playMenuClickSound();
            *scene = 0;
        }
        else {
            changeThemeSong();
            game->menuState = false;
        }
    }
    else {
        changeThemeSong();
        game->menuState = false;
    }
}

static void handleSettingsOption(int *scene, Framework *game) {
    Menu menu = {{"Mute Song", " ", " ", "Back to Menu", "\0"}, {"resources/settings_menu.png"}, false};
    int selectedBox = displayOptions(game, &menu, NULL, NULL);
    switch (selectedBox) {
        case 0:
            playMenuClickSound();
            if (!game->isMuted) {
                Mix_VolumeMusic(0);
                game->isMuted = true;
            }
            else {
                Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
                game->isMuted = false;
            }
            break;
        case 3:
            playMenuClickSound();
            *scene = 0;
            break;
        default:
            break;
    }
}

/*
    SDLNet_UDP_Close(information->sourcePort);
    SDLNet_TCP_Close(TCPInformation->socket);
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