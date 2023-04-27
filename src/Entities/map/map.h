#ifndef MAP_H
#define MAP_H

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAP_ROWS 100
#define MAP_COLUMNS 75
#define TILE_WIDTH 8
#define TILE_HEIGHT 8

typedef struct {
    SDL_Texture *tileAtlas;
    SDL_Rect tilesGraphic[16];
} Background;

void initiateMapResources(SDL_Renderer *renderer, Background *resources);
void renderBackground(SDL_Renderer *renderer, Background resources);

int getNumberOfRowsInMap();
int getNumberOfColumnsInMap();
int getTheWidthOfTheTile();
int getTheHeightOfTheTile();
int getTileInformation(int x, int y);
int encountersForbiddenTile(int x, int y);

#endif 