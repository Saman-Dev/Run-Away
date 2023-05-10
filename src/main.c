#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

#include "Entities/perks/perks.h"
#include "Entities/audio/audio.h"
#include "Entities/map/map.h"
#include "Entities/player/player.h"
#include "Entities/network/network.h"
#include "Entities/menu/menu.h"

// Screen dimensions
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 955

// Perk dimensions
#define PERK_WIDTH 30
#define PERK_HEIGHT 30
#define SPEED_BOOST_AMOUNT 1
#define PERK_SPAWN_TIME 5000 // in milliseconds
#define PERK_FRAME_COUNT 10
#define PERK_FRAME_DELAY 300

#define FPS 60

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool quit;
} Framework;

void initialize(Framework *game);

void handleInput(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY, Player *playerZ);
void renderPlayers(Framework game, Player players[]);

void HuntAndRevive(Player *player1, Player *player3, Player *hunter,SDL_Renderer *renderer, int *test);

void manageFrameRate(int timeAtLoopBeginning);

int main(int argc, char **argv) {
    int timer_length = 1000; // Sätt timerens längd i sekunder
    int timeAtLoopBeginning;
    /////
    int number;
    if (argc == 1) {
        number = 1;
    }
    else if (argc == 2) {
        number = 2;
    }
    else if (argc == 3) {
        number = 3;
    }
    /////
    int test = 0;
    Framework game;
    Background resources;
    Player players[5] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    initialize(&game);
    initiateMapResources(game.renderer, &resources);
    TTF_Init();
    /////
    AddressBook record;
    initiateAddressBook(&record);
    Cargo toSend = {0, 0, 0, 0};
    Network information;
    /////
    /*
    if (number == 3) {
    setUpServer(&information, 2000);
    }
    else {
    setUpClient(&information, "192.168.0.30", 2000);
    } */
    /////

    SpeedBoostPerk speedBoostPerk = initializeSpeedBoostPerk(game.renderer);

    players[0] = createPlayer(game.renderer, "resources/Runner_1.png", 1, 200, 200);
    players[1] = createPlayer(game.renderer, "resources/Hunter.png", 2, 142, 280);
    players[2] = createPlayer(game.renderer, "resources/Runner_3.png", 3, 200, 400);
    /////
    char* options[] = {"Host Game", "Join Game", "Quit"};
    Menu menu = {
        .options = options,
        .numOptions = 3,
        .optionWidth = 200,
        .optionHeight = 50,
        .optionSpacing = 10,
        .menuX = 480,
        .menuY = 477,
    };
    /////
    int selectedOption = displayMenu(game.renderer, &menu);
    /////
    switch (selectedOption) 
    {
        case 0:
            game.quit = false;
            break;
        case 1:
            // options
            break;
        case 2:     
            game.quit = true;
            break;
        default:
            // Handle error or unexpected option
            break;
    }
    /////
    time_t start_time = time(NULL); // Sätt starttiden till nu
    /////
    while (!game.quit)
    {
        time_t current_time = time(NULL); // Hämta aktuell tid
        double elapsed_time = difftime(current_time, start_time); // Beräkna tiden som har gått
        /////
        timeAtLoopBeginning = SDL_GetTicks();
        // Handle events
        /*if (number == 3) {
            handleInput(&game, &player3, &hunter, &player1);
            handlePlayerMovement(&player3);
            handlePlayerMovement(&hunter);
        }
        else {*/
            handleInput(&game, &players[0], &players[1], &players[2]);
            handlePlayerMovement(&players[0]);
            handlePlayerMovement(&players[1]);
            handlePlayerMovement(&players[2]);
        //}

        // Check for perk collision
        applySpeedBoostPerk(&players[0], &speedBoostPerk);
        applySpeedBoostPerk(&players[1], &speedBoostPerk);

        // Game renderer
        SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(game.renderer);
        renderBackground(game.renderer, resources);

        // Render players
        renderPlayers(game, players);

        // Perk render
        renderSpeedBoostPerk(game.renderer, speedBoostPerk);
        HuntAndRevive(&players[0],&players[2],&players[1],game.renderer, &test);

        // Present the rendered frame
        SDL_RenderPresent(game.renderer);

        /*
        if (number == 1) {
            sendData(&information, &toSend, &player1);
            receiveData(information, &hunter, &player3);
        }
        else if (number == 2) {
            sendData(&information, &toSend, &hunter);
            receiveData(information, &player1, &player3);
        }
        else if (number == 3) {
            manageServerDuties(&information, &record, &player1, &hunter, player3, &toSend);
        } */

        manageFrameRate(timeAtLoopBeginning);
        
        if (elapsed_time >= timer_length) { // Kontrollera om tiden har gått ut
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, NULL, "Tiden är ute!", game.window);
            break;
        }
    }

    // Free resources and close SDL
    SDL_DestroyTexture(speedBoostPerk.texture);
    SDL_DestroyWindow(game.window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void initialize(Framework *game) {
    // Initialize SDL, timer and Mixer Library
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));
    initializeAudio();

    game->window = SDL_CreateWindow("RUN AWAY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (game->window == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (game->renderer == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
}

void handleInput(Framework *game, Player *playerX, Player *playerY,Player *playerZ) {
    while (SDL_PollEvent(&game->event)) {
        if (game->event.type == SDL_QUIT) {
            game->quit = true;
        }
        else if (game->event.type == SDL_KEYDOWN) {
            handleKeyPresses(game, playerX, playerY, playerZ);
        }
        else if (game->event.type == SDL_KEYUP) {
            handleKeyReleases(game, playerX, playerY, playerZ);
        }
    }
}

static void handleKeyPresses(Framework *game, Player *playerX, Player *playerY, Player *playerZ) {
    switch (game->event.key.keysym.sym) {
        case SDLK_UP:
            playerX->up = true;
            break;
        case SDLK_DOWN:
            playerX->down = true;
            break;
        case SDLK_LEFT:
            playerX->left = true;
            break;
        case SDLK_RIGHT:
            playerX->right = true;
            break;
        case SDLK_w:
            playerY->up = true;
            break;
        case SDLK_s:
            playerY->down = true;
            break;
        case SDLK_a:
            playerY->left = true;
            break;
        case SDLK_d:
            playerY->right = true;
            break;
            case SDLK_u:
            playerZ->up = true;
            break;
        case SDLK_j:
            playerZ->down = true;
            break;
        case SDLK_h:
            playerZ->left = true;
            break;
        case SDLK_k:
            playerZ->right = true;
            break;
        case SDLK_ESCAPE:
            game->quit = true;
            break;
        default:
            break;        
    }
}

static void handleKeyReleases(Framework *game, Player *playerX, Player *playerY, Player *playerZ) {
    switch (game->event.key.keysym.sym) {
        case SDLK_UP:
            playerX->up = false;
            break;
        case SDLK_DOWN:
            playerX->down = false;
            break;
        case SDLK_LEFT:
            playerX->left = false;
            break;
        case SDLK_RIGHT:
            playerX->right = false;
            break;
        case SDLK_w:
            playerY->up = false;
            break;
        case SDLK_s:
            playerY->down = false;
            break;
        case SDLK_a:
            playerY->left = false;
            break;
        case SDLK_d:
            playerY->right = false;
            break;
            case SDLK_u:
            playerZ->up = false;
            break;
        case SDLK_j:
            playerZ->down = false;
            break;
        case SDLK_h:
           playerZ->left = false;
            break;
        case SDLK_k:
            playerZ->right = false;
            break;
        default:
            break;
    }
}


void HuntAndRevive(Player *player1, Player *player3, Player *hunter,SDL_Renderer *renderer, int *test)
{
    if (checkCollision(player1->position, hunter->position)) 
    {
        
        player1->speed = 0;
        player1->frame = 0;

    }else if (checkCollision(player3->position, hunter->position)) 
    {
        
        player3->speed = 0;
        player3->frame = 0;

    }else if(checkCollision(player1->position, player3->position)){
        player1->speed = 2;
        player3->speed = 2;

    }if( player1->speed == 0){
            SDL_Texture* cage = IMG_LoadTexture(renderer,"resources/cage.png");
            SDL_Rect cage1;
            cage1.x = (player1->position.x-7); // -7 så att spelaren blir exakt i mitten av "cage"
            cage1.y = (player1->position.y-2);
            cage1.w = 40;
            cage1.h = 40;
            *test = 1;
            SDL_RenderCopy(renderer,cage,NULL,&cage1);
        }
        if( player3->speed == 0){
            SDL_Texture* cage = IMG_LoadTexture(renderer,"resources/cage.png");
            SDL_Rect cage1;
            cage1.x = (player3->position.x-7); // -7 så att spelaren blir exakt i mitten av "cage"
            cage1.y = (player3->position.y-2);
            cage1.w = 40;
            cage1.h = 40;
            *test = 1;
            SDL_RenderCopy(renderer,cage,NULL,&cage1);
        }
}

void manageFrameRate(int timeAtLoopBeginning) {
    int endOfLoopTime;
    endOfLoopTime = (SDL_GetTicks()) - timeAtLoopBeginning;
    if (endOfLoopTime < (1000 / FPS)) {
        SDL_Delay((1000 / FPS) - endOfLoopTime);
    }
}

void renderPlayers(Framework game, Player players[]) {
    for (int i = 0; players[i].player != 0; i++) {
        SDL_RenderCopyEx(game.renderer, players[i].spriteSheetTexture, &players[i].spriteClip[players[i].frame], &players[i].position, 0, NULL, SDL_FLIP_NONE);
    }
}