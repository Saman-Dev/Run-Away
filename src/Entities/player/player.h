#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../audio/audio.h"
#include "../foundation/foundation.h"
#include "../map/map.h"

#define PLAYER_FRAME_WIDTH 32
#define PLAYER_FRAME_HEIGHT 32
#define Default_Speed 2

typedef struct {
    int player;
    SDL_Texture *spriteSheetTexture;
    SDL_Rect spriteClip[12];
    SDL_Rect position;
    int frame;
    int speed;
    bool captured;
    bool frozen;
    bool up;
    bool down;
    bool left;
    bool right;
    bool hasPerk;
    time_t perkStartTime;
    // int baseSpeed;
    // int boostedSpeed;
    // Uint32 speedBoostStartTime;
} Player;

static Player createPlayer(SDL_Renderer *renderer, char playerModel[], int playerNumber, int positionX, int positionY);
void handlePlayerMovement(Player *playerX);
void renderPlayers(Framework game, Player players[]);
void handleInput(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
void loadPlayers(SDL_Renderer *renderer, Player players[]);
#endif