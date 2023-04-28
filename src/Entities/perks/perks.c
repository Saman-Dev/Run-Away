#include "perks.h"
#include "../player/player.h"

#define PERK_WIDTH 32
#define PERK_HEIGHT 32
#define PERK_DURATION 10

Perk* create_perk(int type, int value, SDL_Rect *perkRect, SDL_Texture* texture) {
    Perk* s = malloc(sizeof(Perk));
    s->type = type;
    s->value = value;
    s->duration = PERK_DURATION;
    s->perkRect = *perkRect;
    s->texture = texture;
    return s;
}

/*void apply_perk(Player player, Perk* perk) 
{
    switch(perk->type) 
    { 
        case 1: // speed boost
            player -> speed += perk -> value;
            printf("Speed boost: %d \n", player -> speed);
            break;
        case 2: // future perks
        default:
            break;
    }
}*/

void destroy_perk(Perk* perk) 
{
    SDL_DestroyTexture(perk -> texture);
    free(perk);
}