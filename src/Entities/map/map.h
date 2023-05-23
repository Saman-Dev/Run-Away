#ifndef MAP_H
#define MAP_H

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAP_ROWS 50
#define MAP_COLUMNS 30
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

typedef struct {
    SDL_Texture *tileAtlas;
    SDL_Rect tilesGraphic[99];
} Background;

void initiateMapResources(SDL_Renderer *renderer, Background *resources);
void renderBackground(SDL_Renderer *renderer, Background resources);
void renderBackground2(SDL_Renderer *renderer, Background resources);

int getNumberOfRowsInMap();
int getNumberOfColumnsInMap();
int getTheWidthOfTheTile();
int getTheHeightOfTheTile();
int getTileInformation(int x, int y);
int encountersForbiddenTile(int x, int y);

#endif