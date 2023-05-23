#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../foundation/foundation.h"
#include "../network/network.h"

#define MAX_NUMBER_OF_TEXT_BOXES 6

typedef struct {
    char **options;
    int optionWidth;
    int optionHeight;
    int optionSpacing;
    int menuX;
    int menuY;
    char imageFilePath[50];
} Menu;

int manageMenu(Framework *game, Menu *menu, Network *information, TCPLocalInformation *TCPInformation, GameState *state, ClientID record[]);
int displayOptions(Framework *game, Menu *menu);

#endif