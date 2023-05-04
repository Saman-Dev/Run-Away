#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../map/map.h"

#define PLAYER_FRAME_WIDTH 32
#define PLAYER_FRAME_HEIGHT 32

typedef struct {
    int player;
    SDL_Texture *spriteSheetTexture;
    SDL_Rect spriteClip[12];
    SDL_Rect position;
    int frame;
    int speed;
    bool up;
    bool down;
    bool left;
    bool right;
} Player;

Player createPlayer(SDL_Renderer *renderer, char playerModel[], int playerNumber, int positionX, int positionY);
void handlePlayerMovement(Player *playerX);

#endif