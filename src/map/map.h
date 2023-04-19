#ifndef world_h
#define world_h
#include <stdio.h>

#define MAP_ROWS 100
#define MAP_COLUMNS 75
#define TILE_WIDTH 8
#define TILE_HEIGHT 8

int getNumberOfRowsInMap();
int getNumberOfColumnsInMap();
int getTheWidthOfTheTile();
int getTheHeightOfTheTile();
int getTileInformation(int x, int y);
int encountersForbiddenTile(int x, int y);

#endif /* world_h */