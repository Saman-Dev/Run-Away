#ifndef PLAYER_H
#define PLAYER_H

//#include "Entities/perks/perks.h"
#include <stdlib.h>

typedef struct Player_type *Player;

Player createPlayer(int x, int y);
int getPlayerHeight();
int getPlayerWidth();
//void apply_perk(Player* player, Perk* perk);

#endif
