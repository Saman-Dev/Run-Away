#ifndef player_h
#define player_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Player_type *Player;

int getSpacemanPositionX(Player s);
int getSpacemanPositionY(Player s);
Player createPlayer(int x, int y);

#endif
