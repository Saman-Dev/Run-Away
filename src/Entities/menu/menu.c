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

static void updateBoxContent(Framework *game, Menu *menu, SDL_Rect textBoxRectangle[], SDL_Texture *textBoxTexture[], NetworkBundle *networkData) {
    int boxesChanged = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (networkData->TCPRecord[i].active == true && strcmp(menu->options[i+1], "Spot Available") == 0) {
            menu->options[i+1] = "Player connected";
            boxesChanged++;
        }
        else if (networkData->TCPRecord[i].active == false && strcmp(menu->options[i+1], "Player connected") == 0)  {
            menu->options[i+1] = "Spot Available";
            boxesChanged++;
        }
    }
    if (boxesChanged > 0) {
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
    char IPAddress[MAX_INPUT_LENGTH];
    while (networkData->TCPInformation.socket == NULL) {
        IPAddress[0] = '\0';
        askForIPAddressToConnectTo(game, IPAddress);
        InitiateClientTCPCapability(&networkData->TCPInformation, networkData->TCPRecord, IPAddress);
    }
    setUpClient(&networkData->UDPInformation, IPAddress, 2000); // "127.0.0.1"
    if (networkData->TCPInformation.inLobby) {
        Menu menu = {{"Host Connected", "Spot Available", "Spot Available", "Spot Available", "Spot Available", " ", "Back", "\0"}, {"resources/lobby_menu.png"}, true};
        int selectedBox = 0;
        while (selectedBox != 6 && networkData->TCPInformation.inLobby) {
            selectedBox = displayOptions(game, &menu, networkData, players);
        }
        if (selectedBox == 6) {
            playMenuClickSound();
            resetClientNetwork(networkData);
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

static void askForIPAddressToConnectTo(Framework *game, char IPAddress[]) {
    SDL_Texture *background = IMG_LoadTexture(game->renderer, "resources/start_menu.png");
    SDL_Surface *textBoxSurface = NULL;
    SDL_Texture *textBoxTexture = NULL;
    SDL_Rect textBoxRectangle = {0 , SCREEN_HEIGHT / 2 - LOCATION_ADJUSTMENT, 0, 0};
    bool done = false;
    bool renderText = true;
    SDL_StartTextInput();
    while (!done) {
        if (SDL_PollEvent(&game->event)) {
            switch (game->event.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_TEXTINPUT:
                    if (strlen(IPAddress) < (MAX_INPUT_LENGTH - 1)) {
                        strcat(IPAddress, game->event.text.text);
                        renderText = true;
                    }
                    break;
                default:
                    break;
            }
            if (game->event.key.keysym.sym == SDLK_RETURN && game->event.key.state == 0) {
                done = true;
            }
            else if (game->event.key.keysym.sym == SDLK_BACKSPACE && game->event.key.state == 0) {
                if (strcmp(IPAddress, "\0") != 0) {
                    IPAddress[strlen(IPAddress)-1] = '\0';
                    renderText = true;
                }
            }
        }
        SDL_RenderCopy(game->renderer, background, NULL, NULL);
        if (renderText) {
            if (strcmp(IPAddress, "\0") == 0) {
                if (textBoxSurface != NULL) {
                    SDL_FreeSurface(textBoxSurface);
                }
                textBoxSurface = TTF_RenderText_Solid(game->font, " ", game->white);
            }
            else {
                SDL_FreeSurface(textBoxSurface);
                textBoxSurface = TTF_RenderText_Solid(game->font, IPAddress, game->white);
            }
            textBoxRectangle.x = (SCREEN_WIDTH - textBoxRectangle.w) / 2;
            textBoxRectangle.w = textBoxSurface->w;
            textBoxRectangle.h = textBoxSurface->h;
            if (textBoxTexture != NULL) {
                SDL_DestroyTexture(textBoxTexture);
            }
            textBoxTexture = SDL_CreateTextureFromSurface(game->renderer, textBoxSurface);
            renderText = false;
        }
        SDL_RenderCopy(game->renderer, textBoxTexture, NULL, &textBoxRectangle);
        SDL_RenderPresent(game->renderer);
    }
    SDL_FreeSurface(textBoxSurface);
    SDL_DestroyTexture(textBoxTexture);
    SDL_DestroyTexture(background);
    SDL_StopTextInput();
}