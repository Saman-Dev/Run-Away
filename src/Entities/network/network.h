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
	int frame;
	bool connectedStatus;
} PlayerData;

typedef enum {
	READY,
} ClientCommand;

typedef struct{
    ClientCommand command;
    int playerNumber;
} TCPPacket;

typedef struct {
    TCPsocket socket;
    bool active;
} TCPClientInformation;

typedef struct {
    IPaddress ip;
    TCPsocket socket;
    SDLNet_SocketSet set;
	int playerNumber;
} TCPLocalInformation;

typedef enum {
	START,
	ONGOING,
	GAME_OVER,
	SETTINGS,
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
	bool connectedStatus;
} ClientID;

void setUpClient(Network *information, char IP_address[], int port);
void sendData(Network *information, PlayerData *toSend, Player *playerX);
void receiveData(Network *information, Player players[]);
void setUpServer(Network *information, ClientID record[], int port);
void manageServerDuties(Network *information, ClientID record[], Player players[], PlayerData *toSend);

static int checkDifference(PlayerData *toSend, Player *playerX);
static void prepareTransfer(PlayerData *toSend, Player *playerX);
static void commenceTransfer(Network *information, PlayerData *toSend);
static void initiateAddressBook(ClientID record[]);
static void registerSourceInformation(Network *information, PlayerData *receivedData, ClientID record[]);
static void sendServerCopy(Network *information, Uint32 clientIP, Uint16 clientPort, Player *host);
static void sendHostPlayerPacket(Network *information, ClientID record[], PlayerData *toSend, Player *host);
static void applyReceivedData(Player *player, PlayerData *toSend);
static void forwardreceivedPacket(Network *information, PlayerData *receivedData, ClientID record[], Player players[]);
static void changeDestination(Network *information, Uint32 clientIP, Uint16 clientPort);

///// TCP /////
void addClient(TCPLocalInformation *TCPInformation, TCPClientInformation client[]);
void sendClientNumber(TCPsocket clientSocket, int numberToAssign);
void receiveTCPData(TCPLocalInformation *TCPInformation, TCPClientInformation client[], int clientNumber);
void sendTCPData(TCPClientInformation client[], TCPPacket toSend);
void removeClient(TCPLocalInformation *TCPInformation, TCPClientInformation *client, int clientNumber);
void receiveClientNumber(TCPLocalInformation *TCPInformation);
void initiateServerTCPCapability(TCPLocalInformation *TCPInformation);
void InitiateClientTCPCapability(TCPLocalInformation *TCPInformation);
void manageServerTCPActivity(TCPLocalInformation *TCPInformation, TCPClientInformation client[]);

#endif