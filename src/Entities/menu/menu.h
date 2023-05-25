#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../foundation/foundation.h"
#include "../network/network.h"

#define MAX_NUMBER_OF_TEXT_BOXES 8
#define SPACING_BETWEEN_TEXT_BOXES 36
#define LOCATION_ADJUSTMENT 120

typedef struct {
    char *options[MAX_NUMBER_OF_TEXT_BOXES];
    char imageFilePath[50];
    bool networkFunctionality;
} Menu;

void manageMenu(Framework *game, NetworkBundle *networkData);
static int displayOptions(Framework *game, Menu *menu);
static void prepareTextBoxesToBeShown(Framework *game, Menu *menu, SDL_Rect textBoxRectangle[], SDL_Texture *textBoxTexture[]);

static void handleMenuEntry(int *scene, Framework *game);
static void handleHostGameOption(int *scene, Framework *game, NetworkBundle *networkData);
static void handleJoinGameOption(int *scene, Framework *game, NetworkBundle *networkData);
static void handleSettingsOption(int *scene, Framework *game);

#endif