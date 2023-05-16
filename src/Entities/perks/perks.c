#include "perks.h"

Perk createSpeedPerk(SDL_Renderer *renderer, char* spritePath, int posX, int posY, int duration){
    Perk perk;
    perk.texture = IMG_LoadTexture(renderer, spritePath);
    if(perk.texture == NULL){
        printf("Error loading texture: %s\n", SDL_GetError());
        exit(1);
    }

    perk.position.x = posX;
    perk.position.y = posY;
    SDL_QueryTexture(perk.texture, NULL, NULL, &perk.position.w, &perk.position.h);

    perk.active = true;
    perk.duration = duration;

    return perk;
}

void renderPerk(SDL_Renderer *renderer, Perk *perk){
    if(perk->active){
        SDL_RenderCopy(renderer, perk->texture, NULL, &perk->position);
    }
}

void applyPerk(Perk *perk, Player *player){
    if(perk->active){
        player->speed *= SPEED_BOOST_FACTOR;
        perk->active = false;
    }
}

void checkPerkCollision(Player *player, Perk *perk){
    if(perk->active){
        if(SDL_HasIntersection(&player->position, &perk->position)){
            applyPerk(perk, player);
        }
    }
}

void decrementPerkDuration(Perk *perk, Player *player){
    if(perk->duration > 0){
        perk->duration--;
    }
    else if(perk->duration == 0){
        player->speed /= SPEED_BOOST_FACTOR; // Reset speed
        perk->duration = -1; // Set duration to -1 so this block won't run again
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b) // check perk/player collision
{
    return (a.x + a.w > b.x && a.x < b.x + b.w) && (a.y + a.h > b.y && a.y < b.y + b.h);
}