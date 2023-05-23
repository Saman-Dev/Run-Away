#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../foundation/foundation.h"
#include "../network/network.h"

typedef struct {
    char **options;
    int numOptions;
    int optionWidth;
    int optionHeight;
    int optionSpacing;
    int menuX;
    int menuY;
    char img[50];
} Menu;

int manageMenu(Framework *game, Menu *menu, Network *information, TCPLocalInformation *TCPInformation, GameState *state, ClientID record[]);
int displayMenu(SDL_Renderer *renderer, Menu *menu);

#endif