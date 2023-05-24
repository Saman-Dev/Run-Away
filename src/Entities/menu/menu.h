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

void manageMenu(Framework *game, Network *information, TCPLocalInformation *TCPInformation, ClientID record[]);
static int displayOptions(Framework *game, Menu *menu);

static void handleMenuEntry(int *scene, Framework *game);
static void handleHostGameOption(Framework *game, Network *information, TCPLocalInformation *TCPInformation, ClientID record[]);
static void handleJoinGameOption(Framework *game, Network *information, TCPLocalInformation *TCPInformation);
static void handleSettingsOption(int *scene, Framework *game);

#endif