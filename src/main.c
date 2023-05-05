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

#undef main


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

void HuntAndRevive(Player *player1, Player *player3, Player *hunter,SDL_Renderer *renderer, int *test);

void manageFrameRate(int timeAtLoopBeginning);

int main(int argc, char **argv) 
{
    int timer_length = 10; // Sätt timerens längd i sekunder
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
    Player player1;
    Player player3;
    Player hunter;

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

    player1 = createPlayer(game.renderer, "resources/Runner_1.png", 1, 200, 200);
    hunter = createPlayer(game.renderer, "resources/Hunter.png", 2, 142, 280);
    player3 = createPlayer(game.renderer, "resources/Runner_3.png", 3, 200, 400);
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
            handleInput(&game, &player3, &hunter);
            handlePlayerMovement(&player3);
            handlePlayerMovement(&hunter);
        }
        else {*/
            handleInput(&game, &player1, &hunter, &player3);
            handlePlayerMovement(&player1);
            handlePlayerMovement(&hunter);
            handlePlayerMovement(&player3);
        //}

        // Check for perk collision
        applySpeedBoostPerk(&player1, &speedBoostPerk);
        applySpeedBoostPerk(&hunter, &speedBoostPerk);
        
        // Game renderer
        SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(game.renderer);
        renderBackground(game.renderer, resources);

        // Render players
        SDL_RenderCopyEx(game.renderer, player1.spriteSheetTexture, &player1.spriteClip[player1.frame], &player1.position, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(game.renderer, hunter.spriteSheetTexture, &hunter.spriteClip[hunter.frame], &hunter.position, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(game.renderer, player3.spriteSheetTexture, &player3.spriteClip[player3.frame], &player3.position, 0, NULL, SDL_FLIP_NONE);

        // Perk render
        renderSpeedBoostPerk(game.renderer, speedBoostPerk);
        HuntAndRevive(&player1,&player3,&hunter,game.renderer, &test);
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
            printf("Tiden ar ute!\n"); // Skriv ut meddelandet
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

void initialize(Framework *game) 
{
    //game->quit = false;
    
    // Initialize SDL and timer
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));

    // Initialize SDL2 Mixer library and play music
    if (init_audio() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2 Mixer");
        exit(1);
    }

    game->window = SDL_CreateWindow("The Alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
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

    }else if (checkCollision(player3->position, hunter->position)) 
    {
        
        player3->speed = 0;

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