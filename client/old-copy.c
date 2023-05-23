                
main.c 

players[0].inverseMovement = false; // Initialize inverseMovement to false
players[1].inverseMovement = false; // Initialize inverseMovement to false
players[2].inverseMovement = false; // Initialize inverseMovement to false



applySpeedBoostPerk(&players[0], &speedBoostPerk, players);
applySpeedBoostPerk(&players[1], &speedBoostPerk, players);

//----------------

player.h

bool inverseMovement;

//----------------

player.c

if (playerX->inverseMovement)
{
    playerX->position.y += playerX->speed;
}else{
    playerX->position.y -= playerX->speed;
}

//----------------

perk.h

void applySpeedBoostPerk(Player *player, SpeedBoostPerk *perk, Player players[])


perk->duration = 0;
if (player->player == 2)
{
    players[0].inverseMovement = false; // Reset movement input to original values
    players[2].inverseMovement = false; // Reset movement input to original values
}

//----------------