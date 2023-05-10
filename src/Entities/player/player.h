#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../map/map.h"
#include "../audio/audio.h"
#include "../foundation/foundation.h"

#define PLAYER_FRAME_WIDTH 32
#define PLAYER_FRAME_HEIGHT 32

typedef struct {
    int player;
    SDL_Texture *spriteSheetTexture;
    SDL_Rect spriteClip[12];
    SDL_Rect position;
    int frame;
    int speed;
    bool captured;
    bool up;
    bool down;
    bool left;
    bool right;
} Player;

Player createPlayer(SDL_Renderer *renderer, char playerModel[], int playerNumber, int positionX, int positionY);
void handlePlayerMovement(Player *playerX);
void renderPlayers(Framework game, Player players[]);

#endif