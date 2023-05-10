#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../foundation/foundation.h"

typedef struct {
    char** options;
    int numOptions;
    int optionWidth;
    int optionHeight;
    int optionSpacing;
    int menuX;
    int menuY;
} Menu;

void manageMenu(Framework *game, Menu* menu);
int displayMenu(SDL_Renderer* renderer, Menu* menu);

#endif