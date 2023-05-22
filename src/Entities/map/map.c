#include "map.h"

static int mapGrid[MAP_COLUMNS][MAP_ROWS] = {
{68,68,68,68,68,68,68,68,68,77,91,68,68,68,68,68,69,79,71,86,93,91,68,68,68,68,68,77,91,68,68,79,71,86,87,86,91,68,68,68,77,87,86,91,68,68,68,68,68,68},
{77,91,68,68,68,68,68,68,77,87,86,91,68,68,68,68,77,87,71,71,71,86,91,68,68,68,77,87,86,91,68,87,71,71,71,71,86,68,68,68,87,71,71,86,68,68,68,68,68,68},
{87,89,68,68,93,93,91,68,69,70,79,71,00,01,03,59,01,01,01,01,01,01,02,77,91,77,87,00,01,01,01,01,07,07,07,07,07,07,8,87,71,71,71,88,68,68,68,68,68,68},
{71,89,68,87,71,71,71,86,00,01,01,01,16,16,50,16,16,16,16,03,04,05,16,02,86,87,71,9,16,16,16,16,16,16,16,57,05,16,16,07,8,71,88,90,68,68,68,68,68,68},
{71,86,87,71,71,71,0,1,16,16,16,16,16,16,16,16,16,16,16,21,22,23,16,16,1,1,1,16,16,16,19,19,25,25,16,16,41,16,16,16,17,71,89,68,68,68,68,68,68,68},
{71,71,71,71,88,70,9,16,16,16,3,58,58,59,16,16,16,16,16,16,16,16,16,16,16,16,48,16,16,11,71,71,88,68,15,16,50,16,48,16,17,88,90,68,68,68,68,68,68,68},
{71,71,71,71,89,68,9,16,16,16,41,16,16,16,16,16,16,16,16,16,16,19,19,16,16,3,23,16,16,11,70,90,68,68,15,16,16,16,41,16,17,68,68,68,87,86,91,87,86,91},
{70,70,79,71,89,68,9,16,57,58,23,16,16,16,16,16,16,16,16,0,2,67,67,9,16,41,16,16,16,11,68,77,91,68,57,58,58,58,23,16,16,7,8,77,71,71,86,71,71,86},
{68,80,87,71,86,93,9,16,16,16,16,16,3,5,16,16,16,16,16,9,11,84,81,9,16,50,16,16,19,20,77,87,86,91,15,16,16,16,16,16,16,16,26,87,71,71,88,71,71,88},
{68,87,71,71,71,71,9,16,16,16,16,16,12,14,16,16,16,16,16,9,16,2,16,16,16,16,16,11,77,93,87,71,71,86,15,16,16,16,16,16,16,17,87,71,71,88,90,71,88,90},
{78,71,71,71,71,71,18,16,16,16,16,16,12,14,19,16,16,16,16,18,19,20,16,16,16,16,16,11,87,71,71,71,71,88,15,16,16,3,4,5,16,17,71,71,71,89,68,71,89,68},
{68,79,71,71,71,88,68,18,16,16,10,10,12,23,81,18,19,19,10,10,10,10,10,10,10,10,10,11,71,71,71,71,88,90,15,16,16,12,22,14,16,17,71,71,88,90,68,88,90,68},
{68,69,70,70,70,90,68,68,9,10,10,10,12,47,84,81,82,67,19,19,19,3,5,10,10,10,10,10,1,2,71,88,68,68,15,3,58,23,70,12,5,17,71,88,68,68,68,68,68,68},
{68,68,68,68,68,68,0,1,10,10,10,10,12,83,84,84,84,84,81,67,73,12,23,10,10,10,10,10,10,11,69,90,68,68,15,41,86,91,68,21,23,17,69,90,68,68,77,87,86,91},
{68,68,68,68,68,0,10,10,10,10,10,10,21,58,58,58,58,58,58,58,58,23,10,10,10,10,10,48,10,11,68,68,68,68,15,21,58,5,7,16,16,17,68,68,68,68,87,71,71,86},
{68,68,68,68,68,9,10,10,10,10,10,10,11,66,82,83,84,81,82,82,67,9,10,10,10,57,58,22,59,11,68,68,68,6,16,16,16,41,16,16,16,17,68,68,77,87,71,71,71,88},
{4,4,4,4,4,4,4,4,5,10,10,10,11,83,84,84,84,84,84,84,72,9,10,10,10,10,10,10,10,10,1,1,7,16,16,16,16,50,16,16,16,17,68,77,87,71,71,71,88,90},
{21,22,22,22,22,22,22,22,14,10,10,10,10,1,2,84,84,84,84,84,0,10,10,10,10,10,10,10,10,10,19,19,25,25,16,16,10,10,10,10,10,17,77,87,71,71,71,88,68,68},
{75,64,65,84,84,63,66,67,12,4,5,10,10,10,10,10,10,10,10,10,10,10,10,10,57,58,5,10,10,11,69,79,71,86,15,10,10,10,10,10,10,17,69,79,71,71,71,89,68,68},
{66,67,75,64,64,85,84,81,21,13,14,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,41,10,10,11,68,87,71,71,15,10,10,10,48,10,10,17,68,87,71,71,71,89,68,68},
{84,72,73,66,83,84,84,63,65,12,14,10,10,10,10,10,10,10,10,10,10,10,10,19,10,10,50,10,10,11,87,71,71,71,15,10,57,58,23,10,10,17,87,71,71,71,88,90,77,87},
{65,72,73,74,84,84,84,81,67,12,14,57,58,59,10,10,57,58,58,58,5,10,11,63,18,10,10,10,10,11,71,71,88,90,15,10,10,10,10,10,10,17,71,71,70,90,68,80,87,71},
{74,72,73,74,84,84,84,63,76,12,14,10,10,10,10,10,58,58,58,58,41,58,11,72,74,18,19,58,58,11,71,71,89,68,15,25,25,58,58,25,58,17,71,71,68,77,91,69,79,71},
{74,72,66,83,84,84,84,81,66,12,14,58,58,58,58,10,10,10,10,10,50,10,11,76,74,84,63,9,3,58,59,2,86,91,53,68,68,15,17,68,24,26,71,71,77,87,86,68,87,71},
{83,72,74,84,84,84,84,84,74,12,14,19,19,10,10,10,48,10,10,10,10,10,10,2,83,84,72,9,41,10,10,11,71,89,68,68,6,10,17,68,68,68,79,71,87,71,71,86,69,79},
{84,72,75,65,84,84,63,76,74,12,14,68,68,18,10,10,50,10,10,10,10,10,10,10,2,63,76,9,50,10,10,10,7,7,7,7,10,10,10,8,68,68,78,71,71,71,71,88,68,87},
{84,72,73,75,65,63,76,73,74,12,14,68,68,68,9,10,10,10,10,10,10,10,10,10,10,1,1,10,10,10,10,10,10,10,16,16,16,16,16,26,86,91,69,79,71,71,88,90,69,79},
{84,72,73,73,73,73,3,58,58,22,23,68,93,87,18,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,25,25,25,25,25,25,26,79,71,86,68,68,71,88,68,68,68,87},
{84,81,73,73,73,73,41,10,10,11,77,87,71,71,71,71,88,90,68,77,91,68,69,79,71,86,87,86,91,68,68,68,69,90,68,68,93,93,93,87,71,68,68,68,68,68,93,93,93,87},
{84,84,81,73,73,73,50,10,10,10,2,71,71,71,71,88,68,68,68,87,86,91,68,87,71,71,71,71,86,68,68,68,68,68,68,87,71,71,71,71,71,68,68,68,68,87,71,71,71,71},
};

static int dangerTiles[MAP_COLUMNS][MAP_ROWS] = {
{8,95,95,95,95,95,95,95,95,77,91,95,95,95,95,95,69,79,71,86,93,91,95,95,95,95,95,77,91,95,95,79,71,86,87,86,91,95,95,95,77,87,86,91,95,95,68,68,68,68},
{77,91,68,68,68,68,68,68,77,87,86,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,87,71,71,86,68,68,68,68,68,68},
{87,89,68,68,93,93,91,95,95,95,95,95,0,1,95,95,1,1,1,1,1,1,2,95,95,95,95,0,1,1,1,1,7,7,7,7,7,7,8,95,95,95,71,88,68,68,68,68,68,68},
{71,89,68,87,71,95,95,95,0,1,1,1,10,10,95,10,10,10,10,95,95,95,10,2,95,95,95,9,10,10,10,10,16,16,16,95,95,16,16,7,8,95,88,90,95,95,95,95,95,95},
{71,86,87,71,71,95,0,1,10,10,10,10,10,10,10,10,10,10,10,95,95,95,10,10,1,1,1,10,10,10,19,19,25,25,16,16,95,16,16,16,17,95,89,95,95,95,95,95,95,95},
{71,71,71,71,88,95,9,10,10,10,95,95,95,95,10,10,10,10,10,10,10,10,10,10,10,10,95,10,10,11,95,95,95,95,15,16,95,16,95,16,17,95,90,95,95,95,95,95,95,95},
{71,71,71,71,89,95,9,10,10,10,95,10,10,10,10,10,10,10,10,10,10,19,19,10,10,95,95,10,10,11,95,95,95,95,15,16,16,16,95,16,17,95,95,95,87,86,91,87,86,91},
{70,70,79,71,89,95,9,10,95,95,95,10,10,10,10,10,10,10,10,95,95,95,95,9,10,95,10,10,10,11,95,95,95,95,95,95,95,95,95,16,16,7,8,95,71,71,86,71,71,86},
{95,80,87,71,86,95,9,10,10,10,10,10,95,95,10,10,10,10,10,95,95,95,95,9,10,95,10,10,19,20,95,95,95,95,15,16,16,16,16,16,16,16,26,95,71,71,88,71,71,88},
{95,87,71,71,71,95,9,10,10,10,10,10,95,95,10,10,10,10,10,95,95,95,10,10,10,10,10,11,95,95,95,95,95,95,15,16,16,16,16,16,16,17,95,95,71,88,90,71,88,90},
{78,71,71,71,71,95,18,10,10,10,10,10,95,95,19,10,10,10,10,95,95,95,10,10,10,10,10,11,95,95,95,95,95,95,15,16,16,95,95,95,16,17,95,71,71,89,95,71,89,95},
{95,79,71,71,71,95,95,18,10,10,10,10,95,95,95,18,19,19,10,10,10,10,10,10,10,10,10,11,95,95,95,95,95,95,15,16,16,95,95,95,16,17,95,71,88,90,95,88,90,95},
{95,69,70,70,70,95,95,95,9,10,10,10,95,95,95,95,95,95,19,19,19,95,95,10,10,10,10,10,1,2,95,95,95,95,15,95,95,95,95,95,95,17,95,88,95,95,95,95,95,95},
{95,95,95,95,95,95,0,1,10,10,10,10,95,95,95,95,95,95,95,95,95,95,95,10,10,10,10,10,10,11,95,95,95,95,15,95,95,95,95,95,95,17,95,90,95,68,77,87,86,91},
{68,68,68,68,95,0,10,10,10,10,10,10,95,95,95,95,95,95,95,95,95,95,10,10,10,10,10,95,10,11,95,95,95,95,15,95,95,95,7,16,16,17,95,68,68,68,87,71,71,86},
{68,68,68,68,95,9,10,10,10,10,10,10,11,95,95,95,95,95,95,95,95,9,10,10,10,95,95,95,95,11,95,95,95,6,16,16,16,95,16,16,16,17,95,68,77,87,71,71,71,88},
{4,4,4,4,95,95,95,95,95,10,10,10,11,95,95,95,95,95,95,95,95,9,10,10,10,10,10,10,10,10,1,1,7,16,16,16,16,95,16,16,16,17,95,77,87,71,71,71,88,90},
{21,22,22,22,22,22,22,22,95,10,10,10,10,1,2,95,95,95,95,95,0,10,10,10,10,10,10,10,10,10,19,19,25,25,16,16,16,16,16,16,16,17,95,87,71,71,71,88,68,68},
{75,64,65,84,84,63,66,67,95,95,95,10,10,10,10,10,10,10,10,10,10,10,10,10,95,95,95,10,10,11,95,95,95,95,15,16,16,16,16,16,16,17,95,79,71,71,71,89,68,68},
{66,67,75,64,64,85,84,81,21,13,95,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,95,10,10,11,95,95,95,95,15,16,16,16,95,16,16,17,95,87,71,71,71,89,68,68},
{84,72,73,66,83,84,84,63,65,12,95,10,10,10,10,10,10,10,10,10,10,10,10,19,10,10,95,10,10,11,95,95,95,95,15,16,95,95,95,16,16,17,95,71,71,71,88,90,77,87},
{65,72,73,74,84,84,84,81,67,12,95,95,95,95,10,10,95,95,95,95,95,10,11,95,18,10,10,10,10,11,95,95,95,95,15,16,16,16,16,16,16,17,95,71,70,90,68,80,87,71},
{74,72,73,74,84,84,84,63,76,12,95,10,10,10,10,10,10,10,10,10,95,10,11,95,95,18,19,10,10,11,95,95,95,95,15,25,25,16,16,25,16,17,95,71,68,77,91,69,79,71},
{74,72,66,83,84,84,84,81,66,12,95,10,10,10,10,10,10,10,10,10,95,10,11,95,95,95,95,9,95,95,95,2,95,95,53,95,95,15,17,95,24,26,95,71,77,87,86,68,87,71},
{83,72,74,84,84,84,84,84,74,12,95,19,19,10,10,10,95,10,10,10,10,10,10,2,95,95,95,9,95,10,10,11,95,95,95,95,6,16,17,95,95,95,79,71,87,71,71,86,69,79},
{84,72,75,65,84,84,63,76,74,12,95,95,95,18,10,10,95,10,10,10,10,10,10,10,2,95,95,9,95,10,10,10,7,7,7,7,16,16,16,8,95,95,78,71,71,71,71,88,68,87},
{84,72,73,75,65,63,76,73,74,12,95,68,95,95,9,10,10,10,10,10,10,10,10,10,10,1,1,10,10,10,10,10,16,16,16,16,16,16,16,26,95,91,69,79,71,71,88,90,69,79},
{84,72,73,73,73,73,3,58,58,22,23,68,93,95,18,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,25,25,25,25,25,25,26,95,95,86,68,68,71,88,68,68,68,87},
{84,81,73,73,73,73,41,10,10,11,77,87,71,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,71,68,68,68,68,68,93,93,93,87},
{84,84,81,73,73,73,50,10,10,10,2,71,71,71,71,88,68,68,68,87,86,91,68,87,71,71,71,71,86,68,68,68,68,68,68,87,71,71,71,71,71,68,68,68,68,87,71,71,71,71},
};

int getNumberOfRowsInMap()
{
    return MAP_ROWS;
}

int getNumberOfColumnsInMap()
{
    return MAP_COLUMNS;
}

int getTheWidthOfTheTile()
{
    return TILE_WIDTH;
}

int getTheHeightOfTheTile()
{
    return TILE_HEIGHT;
}

int getTileInformation(int x, int y)
{
    
    return mapGrid[x][y];
   
    
}

int encountersForbiddenTile(int x, int y)
{
    //The code is used to find the tile that a pixel is in. It takes the x and y position of the pixel and divides each by 3, and then adds 32 to each result. It then returns the result of each division. The result is the tile that the pixel is in. 
    int tileX = y / 32;
    int tileY = x / 32;
    //printf("TileX: %d TileY: %d TileValue:%d\n", tileX, tileY,dangerTiles[tileX][tileY]);
    if (dangerTiles[tileX][tileY] == 95 )
    {
        //printf("Collision!\n");
        return 1;
    }

    return 0;
}

void renderBackground(SDL_Renderer *renderer, Background resources) {
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    position.w = getTheWidthOfTheTile();
    position.h = getTheHeightOfTheTile();
    
    for (int i = 0; i < getNumberOfColumnsInMap(); i++)
    {
        for (int j = 0; j < getNumberOfRowsInMap(); j++)
        {
            position.x = j * getTheWidthOfTheTile();
            position.y = i * getTheHeightOfTheTile();
            SDL_RenderCopyEx(renderer, resources.tileAtlas, &resources.tilesGraphic[getTileInformation(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
}
void initiateMapResources(SDL_Renderer *renderer, Background *resources) {
    resources->tileAtlas = IMG_LoadTexture(renderer, "resources/Tiles3.png");
    int tileIndex = 0;
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 9; x++) {
            resources->tilesGraphic[tileIndex].x = x * 32;
            resources->tilesGraphic[tileIndex].y = y * 32;
            resources->tilesGraphic[tileIndex].w = 32;
            resources->tilesGraphic[tileIndex].h = 32;
            tileIndex++;
        }
    }
}

//############################################################################################################################################################
/*
static int mapGrid[MAP_COLUMNS][MAP_ROWS] = {
{68,68,68,68,68,68,68,68,68,77,91,68,68,68,68,68,69,79,71,86,93,91,68,68,68,68,68,77,91,68,68,79,71,86,87,86,91,68,68,68,77,87,86,91,68,68,68,68,68,68},
{77,91,68,68,68,68,68,68,77,87,86,91,68,68,68,68,77,87,71,71,71,86,91,68,68,68,77,87,86,91,68,87,71,71,71,71,86,68,68,68,87,71,71,86,68,68,68,68,68,68},
{87,89,68,68,93,93,91,68,69,70,79,71,0,1,3,59,1,1,1,1,1,1,2,77,91,77,87,0,1,1,1,1,7,7,7,7,7,7,8,87,71,71,71,88,68,68,68,68,68,68},
{71,89,68,87,71,71,71,86,0,1,1,1,10,10,50,10,10,10,10,3,4,5,10,2,86,87,71,9,10,10,10,10,16,16,16,57,5,16,16,7,8,71,88,90,68,68,68,68,68,68},
{71,86,87,71,71,71,0,1,10,10,10,10,10,10,10,10,10,10,10,21,22,23,10,10,1,1,1,10,10,10,19,19,25,25,16,16,41,16,16,16,17,71,89,68,68,68,68,68,68,68},
{71,71,71,71,88,70,9,10,10,10,3,58,58,59,10,10,10,10,10,10,10,10,10,10,10,10,48,10,10,11,71,71,88,68,15,16,50,16,48,16,17,88,90,68,68,68,68,68,68,68},
{71,71,71,71,89,68,9,10,10,10,41,10,10,10,10,10,10,10,10,10,10,19,19,10,10,3,23,10,10,11,70,90,68,68,15,16,16,16,41,16,17,68,68,68,87,86,91,87,86,91},
{70,70,79,71,89,68,9,10,57,58,23,10,10,10,10,10,10,10,10,0,2,67,67,9,10,41,10,10,10,11,68,77,91,68,57,58,58,58,23,16,16,7,8,77,71,71,86,71,71,86},
{68,80,87,71,86,93,9,10,10,10,10,10,3,5,10,10,10,10,10,9,11,84,81,9,10,50,10,10,19,20,77,87,86,91,15,16,16,16,16,16,16,16,26,87,71,71,88,71,71,88},
{68,87,71,71,71,71,9,10,10,10,10,10,12,14,10,10,10,10,10,9,10,2,10,10,10,10,10,11,77,93,87,71,71,86,15,16,16,16,16,16,16,17,87,71,71,88,90,71,88,90},
{78,71,71,71,71,71,18,10,10,10,10,10,12,14,19,10,10,10,10,18,19,20,10,10,10,10,10,11,87,71,71,71,71,88,15,16,16,3,4,5,16,17,71,71,71,89,68,71,89,68},
{68,79,71,71,71,88,68,18,10,10,10,10,12,23,81,18,19,19,10,10,10,10,10,10,10,10,10,11,71,71,71,71,88,90,15,16,16,12,22,14,16,17,71,71,88,90,68,88,90,68},
{68,69,70,70,70,90,68,68,9,10,10,10,12,47,84,81,82,67,19,19,19,3,5,10,10,10,10,10,1,2,71,88,68,68,15,3,58,23,70,12,5,17,71,88,68,68,68,68,68,68},
{68,68,68,68,68,68,0,1,10,10,10,10,12,83,84,84,84,84,81,67,73,12,23,10,10,10,10,10,10,11,69,90,68,68,15,41,86,91,68,21,23,17,69,90,68,68,77,87,86,91},
{68,68,68,68,68,0,10,10,10,10,10,10,21,58,58,58,58,58,58,58,58,23,10,10,10,10,10,48,10,11,68,68,68,68,15,21,58,5,7,16,16,17,68,68,68,68,87,71,71,86},
{68,68,68,68,68,9,10,10,10,10,10,10,11,66,82,83,84,81,82,82,67,9,10,10,10,57,58,22,59,11,68,68,68,6,16,16,16,41,16,16,16,17,68,68,77,87,71,71,71,88},
{4,4,4,4,4,4,4,4,5,10,10,10,11,83,84,84,84,84,84,84,72,9,10,10,10,10,10,10,10,10,1,1,7,16,16,16,16,50,16,16,16,17,68,77,87,71,71,71,88,90},
{21,22,22,22,22,22,22,22,14,10,10,10,10,1,2,84,84,84,84,84,0,10,10,10,10,10,10,10,10,10,19,19,25,25,16,16,16,16,16,16,16,17,77,87,71,71,71,88,68,68},
{75,64,65,84,84,63,66,67,12,4,5,10,10,10,10,10,10,10,10,10,10,10,10,10,57,58,5,10,10,11,69,79,71,86,15,16,16,16,16,16,16,17,69,79,71,71,71,89,68,68},
{66,67,75,64,64,85,84,81,21,13,14,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,41,10,10,11,68,87,71,71,15,16,16,16,48,16,16,17,68,87,71,71,71,89,68,68},
{84,72,73,66,83,84,84,63,65,12,14,10,10,10,10,10,10,10,10,10,10,10,10,19,10,10,50,10,10,11,87,71,71,71,15,16,57,58,23,16,16,17,87,71,71,71,88,90,77,87},
{65,72,73,74,84,84,84,81,67,12,14,57,58,59,10,10,57,58,58,58,5,10,11,63,18,10,10,10,10,11,71,71,88,90,15,16,16,16,16,16,16,17,71,71,70,90,68,80,87,71},
{74,72,73,74,84,84,84,63,76,12,14,10,10,10,10,10,10,10,10,10,41,10,11,72,74,18,19,10,10,11,71,71,89,68,15,25,25,16,16,25,16,17,71,71,68,77,91,69,79,71},
{74,72,66,83,84,84,84,81,66,12,14,10,10,10,10,10,10,10,10,10,50,10,11,76,74,84,63,9,3,58,59,2,86,91,53,68,68,15,17,68,24,26,71,71,77,87,86,68,87,71},
{83,72,74,84,84,84,84,84,74,12,14,19,19,10,10,10,48,10,10,10,10,10,10,2,83,84,72,9,41,10,10,11,71,89,68,68,6,16,17,68,68,68,79,71,87,71,71,86,69,79},
{84,72,75,65,84,84,63,76,74,12,14,68,68,18,10,10,50,10,10,10,10,10,10,10,2,63,76,9,50,10,10,10,7,7,7,7,16,16,16,8,68,68,78,71,71,71,71,88,68,87},
{84,72,73,75,65,63,76,73,74,12,14,68,68,68,9,10,10,10,10,10,10,10,10,10,10,1,1,10,10,10,10,10,16,16,16,16,16,16,16,26,86,91,69,79,71,71,88,90,69,79},
{84,72,73,73,73,73,3,58,58,22,23,68,93,87,18,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,25,25,25,25,25,25,26,79,71,86,68,68,71,88,68,68,68,87},
{84,81,73,73,73,73,41,10,10,11,77,87,71,71,71,71,88,90,68,77,91,68,69,79,71,86,87,86,91,68,68,68,69,90,68,68,93,93,93,87,71,68,68,68,68,68,93,93,93,87},
{84,84,81,73,73,73,50,10,10,10,2,71,71,71,71,88,68,68,68,87,86,91,68,87,71,71,71,71,86,68,68,68,68,68,68,87,71,71,71,71,71,68,68,68,68,87,71,71,71,71},
};

static int dangerTiles[MAP_COLUMNS][MAP_ROWS] = {
{8,68,68,68,68,68,68,68,68,77,91,68,68,68,68,68,69,79,71,86,93,91,68,68,68,68,68,77,91,68,68,79,71,86,87,86,91,68,68,68,77,87,86,91,68,68,68,68,68,68},
{77,91,68,68,68,68,68,68,77,87,86,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,87,71,71,86,68,68,68,68,68,68},
{87,89,68,68,93,93,91,95,95,95,95,95,0,1,95,95,1,1,1,1,1,1,2,95,95,95,95,0,1,1,1,1,7,7,7,7,7,7,8,95,95,95,71,88,68,68,68,68,68,68},
{71,89,68,87,71,95,95,95,0,1,1,1,10,10,95,10,10,10,10,95,95,95,10,2,95,95,95,9,10,10,10,10,16,16,16,95,95,16,16,7,8,95,88,90,68,68,68,68,68,68},
{71,86,87,71,71,95,0,1,10,10,10,10,10,10,10,10,10,10,10,95,95,95,10,10,1,1,1,10,10,10,19,19,25,25,16,16,95,16,16,16,17,95,89,68,68,68,68,68,68,68},
{71,71,71,71,88,95,9,10,10,10,95,95,95,95,10,10,10,10,10,10,10,10,10,10,10,10,95,10,10,11,95,95,95,95,15,16,95,16,95,16,17,95,90,68,68,68,68,68,68,68},
{71,71,71,71,89,95,9,10,10,10,95,10,10,10,10,10,10,10,10,10,10,19,19,10,10,95,95,10,10,11,95,95,95,95,15,16,16,16,95,16,17,95,95,95,87,86,91,87,86,91},
{70,70,79,71,89,95,9,10,95,95,95,10,10,10,10,10,10,10,10,95,95,95,95,9,10,95,10,10,10,11,95,95,95,95,95,95,95,95,95,16,16,7,8,95,71,71,86,71,71,86},
{68,80,87,71,86,95,9,10,10,10,10,10,95,95,10,10,10,10,10,95,95,95,95,9,10,95,10,10,19,20,95,95,95,95,15,16,16,16,16,16,16,16,26,95,71,71,88,71,71,88},
{68,87,71,71,71,95,9,10,10,10,10,10,95,95,10,10,10,10,10,95,95,95,10,10,10,10,10,11,95,95,95,95,95,95,15,16,16,16,16,16,16,17,95,95,71,88,90,71,88,90},
{78,71,71,71,71,95,18,10,10,10,10,10,95,95,19,10,10,10,10,95,95,95,10,10,10,10,10,11,95,95,95,95,95,95,15,16,16,95,95,95,16,17,95,71,71,89,68,71,89,68},
{68,79,71,71,71,95,95,18,10,10,10,10,95,95,95,18,19,19,10,10,10,10,10,10,10,10,10,11,95,95,95,95,95,95,15,16,16,95,95,95,16,17,95,71,88,90,68,88,90,68},
{68,69,70,70,70,95,95,95,9,10,10,10,95,95,95,95,95,95,19,19,19,95,95,10,10,10,10,10,1,2,95,95,95,95,15,95,95,95,95,95,95,17,95,88,68,68,68,68,68,68},
{68,68,68,68,95,95,0,1,10,10,10,10,95,95,95,95,95,95,95,95,95,95,95,10,10,10,10,10,10,11,95,95,95,95,15,95,95,95,95,95,95,17,95,90,68,68,77,87,86,91},
{68,68,68,68,95,0,10,10,10,10,10,10,95,95,95,95,95,95,95,95,95,95,10,10,10,10,10,95,10,11,95,95,95,95,15,95,95,95,7,16,16,17,95,68,68,68,87,71,71,86},
{68,68,68,68,95,9,10,10,10,10,10,10,11,95,95,95,95,95,95,95,95,9,10,10,10,95,95,95,95,11,95,95,95,6,16,16,16,95,16,16,16,17,95,68,77,87,71,71,71,88},
{4,4,4,4,95,95,95,95,95,10,10,10,11,95,95,95,95,95,95,95,95,9,10,10,10,10,10,10,10,10,1,1,7,16,16,16,16,95,16,16,16,17,95,77,87,71,71,71,88,90},
{21,22,22,22,22,22,22,22,95,10,10,10,10,1,2,95,95,95,95,95,0,10,10,10,10,10,10,10,10,10,19,19,25,25,16,16,16,16,16,16,16,17,95,87,71,71,71,88,68,68},
{75,64,65,84,84,63,66,67,95,95,95,10,10,10,10,10,10,10,10,10,10,10,10,10,95,95,95,10,10,11,95,95,95,95,15,16,16,16,16,16,16,17,95,79,71,71,71,89,68,68},
{66,67,75,64,64,85,84,81,21,13,95,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,95,10,10,11,95,95,95,95,15,16,16,16,95,16,16,17,95,87,71,71,71,89,68,68},
{84,72,73,66,83,84,84,63,65,12,95,10,10,10,10,10,10,10,10,10,10,10,10,19,10,10,95,10,10,11,95,95,95,95,15,16,95,95,95,16,16,17,95,71,71,71,88,90,77,87},
{65,72,73,74,84,84,84,81,67,12,95,95,95,95,10,10,95,95,95,95,95,10,11,95,18,10,10,10,10,11,95,95,95,95,15,16,16,16,16,16,16,17,95,71,70,90,68,80,87,71},
{74,72,73,74,84,84,84,63,76,12,95,10,10,10,10,10,10,10,10,10,95,10,11,95,95,18,19,10,10,11,95,95,95,95,15,25,25,16,16,25,16,17,95,71,68,77,91,69,79,71},
{74,72,66,83,84,84,84,81,66,12,95,10,10,10,10,10,10,10,10,10,95,10,11,95,95,95,95,9,95,95,95,2,95,95,53,95,95,15,17,95,24,26,95,71,77,87,86,68,87,71},
{83,72,74,84,84,84,84,84,74,12,95,19,19,10,10,10,95,10,10,10,10,10,10,2,95,95,95,9,95,10,10,11,95,95,95,95,6,16,17,95,95,95,79,71,87,71,71,86,69,79},
{84,72,75,65,84,84,63,76,74,12,95,95,95,18,10,10,95,10,10,10,10,10,10,10,2,95,95,9,95,10,10,10,7,7,7,7,16,16,16,8,95,95,78,71,71,71,71,88,68,87},
{84,72,73,75,65,63,76,73,74,12,95,68,95,95,9,10,10,10,10,10,10,10,10,10,10,1,1,10,10,10,10,10,16,16,16,16,16,16,16,26,95,91,69,79,71,71,88,90,69,79},
{84,72,73,73,73,73,3,58,58,22,23,68,93,95,18,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,25,25,25,25,25,25,26,95,95,86,68,68,71,88,68,68,68,87},
{84,81,73,73,73,73,41,10,10,11,77,87,71,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,71,68,68,68,68,68,93,93,93,87},
{84,84,81,73,73,73,50,10,10,10,2,71,71,71,71,88,68,68,68,87,86,91,68,87,71,71,71,71,86,68,68,68,68,68,68,87,71,71,71,71,71,68,68,68,68,87,71,71,71,71},
};
*/
//##########################################################################################################################################################