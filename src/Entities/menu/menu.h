#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>

typedef struct {
    char** options;
    int numOptions;
    int optionWidth;
    int optionHeight;
    int optionSpacing;
    int menuX;
    int menuY;
} Menu;

int displayMenu(SDL_Renderer* renderer, Menu* menu);

#endif /* MENU_H */
