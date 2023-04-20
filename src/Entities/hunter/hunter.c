#include "hunter.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int WIDTH = 32;
PRIVATE int HEIGHT = 32;

struct Player_type
{
    int Player_POSITION_X;
    int Player_POSITION_Y;
    int Player_FRAME;
    int speed;
};

PUBLIC hunter createHunter(int x, int y)
{
    hunter s = malloc(sizeof(struct Player_type));
    s->Player_POSITION_Y = y;
    s->Player_POSITION_X = x;
    s->Player_FRAME = 0;
    s->speed = 1;
    return s;
}

PUBLIC int getHunterHeight()
{
    return HEIGHT;
}

PUBLIC int getHunterWidth()
{
    return WIDTH;
}

PUBLIC void tick(int direction)
{
}
