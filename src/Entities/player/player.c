#include "player.h"
#include "../perks/perks.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int WIDTH = 32;
PRIVATE int HEIGHT = 32;

typedef struct Player_type
{
    int Player_POSITION_X;
    int Player_POSITION_Y;
    int Player_FRAME;
    int speed;
} *Player;

PUBLIC Player createPlayer(int x, int y)
{
    Player s = malloc(sizeof(struct Player_type));
    s->Player_POSITION_Y = y;
    s->Player_POSITION_X = x;
    s->Player_FRAME = 0;
    s->speed = 1;
    return s;
}

PUBLIC int getPlayerHeight()
{
    return HEIGHT;
}

PUBLIC int getPlayerWidth()
{
    return WIDTH;
}

PUBLIC void tick(int direction)
{
}
