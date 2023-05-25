#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../foundation/foundation.h"
#include "../network/network.h"

#define MAX_NUMBER_OF_TEXT_BOXES 7
#define SPACING_BETWEEN_TEXT_BOXES 36
#define LOCATION_ADJUSTMENT 120

typedef struct {
    char **options;
    char role[10];
    char imageFilePath[50];
} Menu;

void manageMenu(Framework *game, Network *information, TCPLocalInformation *TCPInformation, ClientID record[]);
static int displayOptions(Framework *game, Menu *menu);
static void prepareTextBoxesToBeShown(Framework *game, Menu *menu, SDL_Rect textBoxRectangle[], SDL_Texture *textBoxTexture[]);

static void handleMenuEntry(int *scene, Framework *game);
static void handleHostGameOption(int *scene, Framework *game, Network *information, TCPLocalInformation *TCPInformation, ClientID record[]);
static void handleJoinGameOption(Framework *game, Network *information, TCPLocalInformation *TCPInformation);
static void handleSettingsOption(int *scene, Framework *game);

#endif