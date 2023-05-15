#ifndef NETWORK_H
#define NETWORK_H

#define MAX_CLIENTS 3

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../player/player.h"

typedef struct {
	int player;
	int positionX;
	int positionY;
	int alive;
	bool isConnected;
	int frame;
} PlayerData;

typedef enum {
	READY,
} ClientCommand;

typedef struct{
    ClientCommand command;
    int playerNumber;
} ClientData;

typedef enum {
	START,
	ONGOING,
	GAME_OVER
} GameState;

typedef struct {
    UDPsocket sourcePort;
	IPaddress destination;
	UDPpacket *packetToSend;
	UDPpacket *packetToReceive;
	bool lobbyActive;
	int nrOfClients;
    GameState gState; // add this variable to track the game state
	PlayerData players[MAX_CLIENTS];
	int playerNr;
} Network;

typedef struct {
    Uint32 ip;
    Uint16 port;
} ClientID;

typedef struct {
    ClientID id;
	PlayerData player;
    // add any other relevant information about the client here
} Client;

typedef struct {
    Client clients[3];
} AddressBook;

void setUpClient(Network *information, char IP_address[], int port);
void sendData(Network *information, PlayerData *toSend, Player *playerX);
void receiveData(Network *information, Player players[]);
void setUpServer(Network *information, AddressBook *record, int port);
void manageServerDuties(Network *information, AddressBook *record, Player players[], PlayerData *toSend);

static int checkDifference(PlayerData *toSend, Player *playerX);
static void prepareTransfer(PlayerData *toSend, Player *playerX);
static void commenceTransfer(Network *information, PlayerData *toSend);
static void initiateAddressBook(AddressBook *record);
static void registerSourceInformation(Network *information, PlayerData *receivedData, AddressBook *record);
static void sendServerCopy(Network *information, Uint32 clientIP, Uint16 clientPort, Player *host);
static void sendHostPlayerPacket(Network *information, AddressBook *record, PlayerData *toSend, Player *host);
static void applyReceivedData(Player *player, PlayerData *toSend);
static void forwardreceivedPacket(Network *information, PlayerData *receivedData, AddressBook *record, Player players[]);
static void changeDestination(Network *information, Uint32 clientIP, Uint16 clientPort);

#endif