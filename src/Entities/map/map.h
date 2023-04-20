
#ifndef MAP_H
#define MAP_H
#include <stdio.h>

#define MAP_ROWS 100   // 100
#define MAP_COLUMNS 75 // 75
#define TILE_WIDTH 8
#define TILE_HEIGHT 8

int getNumberOfRowsInMap();
int getNumberOfColumnsInMap();
int getTheWidthOfTheTile();
int getTheHeightOfTheTile();
int getTileInformation(int x, int y);
int encountersForbiddenTile(int x, int y);

#endif /* MAP_H */