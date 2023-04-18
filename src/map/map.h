#ifndef world_h
#define world_h
#include <stdio.h>

int getTileRows();
int getTileColumns();
int getTileWidth();
int getTileHeight();
int getGridOfTiles(int x, int y);
int collidesWithImpassableTile(int x, int y);

#endif /* world_h */
