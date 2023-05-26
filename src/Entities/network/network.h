#ifndef UDPLocalInformation_H
#define UDPLocalInformation_H

#define MAX_CLIENTS 3

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../player/player.h"

typedef struct {
	UDPsocket sourcePort;
	IPaddress destination;
	UDPpacket *packetToSend;
	UDPpacket *packetToReceive;
} UDPLocalInformation;

typedef struct {
	Uint32 ip;
	Uint16 port;
	bool active;
} UDPClientInformation;

typedef struct {
	int player;
	int positionX;
	int positionY;
	int frame;
} PlayerData; // UDPPacket

typedef struct {
	IPaddress ip;
	TCPsocket socket;
	SDLNet_SocketSet set;
	int playerNumber;
	bool inLobby;
} TCPLocalInformation;

typedef struct {
	TCPsocket socket;
	bool active;
} TCPClientInformation;

typedef struct {
	bool inLobby;
	int playerNumber;
	TCPClientInformation TCPRecord[MAX_CLIENTS];
} TCPPacket;

typedef struct {
	UDPLocalInformation UDPInformation;
	UDPClientInformation UDPRecord[MAX_CLIENTS];
	PlayerData toSend;
	TCPLocalInformation TCPInformation;
	TCPClientInformation TCPRecord[MAX_CLIENTS];
} NetworkBundle;

void manageNetwork(NetworkBundle *networkData, Player players[]);
static void manageUDPClientConnection(UDPLocalInformation *information, PlayerData *toSend, Player players[], int playerNumber);
void setUpClient(UDPLocalInformation *information, char IP_address[], int port);
void setUpServer(UDPLocalInformation *information, UDPClientInformation UDPRecord[], int port);
static void manageServerDuties(UDPLocalInformation *information, UDPClientInformation UDPRecord[], Player players[], PlayerData *toSend);
static void sendData(UDPLocalInformation *information, PlayerData *toSend, Player *playerX);
static void receiveData(UDPLocalInformation *information, Player players[]);

static int checkDifference(PlayerData *toSend, Player *playerX);
static void prepareTransfer(PlayerData *toSend, Player *playerX);
static void commenceTransfer(UDPLocalInformation *information, PlayerData *toSend);
static void initiateUDPAddressBook(UDPClientInformation UDPRecord[]);
static void registerSourceInformation(UDPLocalInformation *information, UDPClientInformation UDPRecord[]);
static void sendServerCopy(UDPLocalInformation *information, Uint32 clientIP, Uint16 clientPort, Player *host);
static void sendHostPlayerPacket(UDPLocalInformation *information, UDPClientInformation UDPRecord[], PlayerData *toSend, Player *host);
static void applyReceivedData(Player *player, PlayerData *toSend);
static void forwardreceivedPacket(UDPLocalInformation *information, PlayerData *receivedData, UDPClientInformation UDPRecord[], Player players[]);
static void changeDestination(UDPLocalInformation *information, Uint32 clientIP, Uint16 clientPort);

///// TCP /////
static void addClient(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[]);
static void sendClientNumber(TCPsocket clientSocket, int numberToAssign);
static void receiveTCPData(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[], UDPClientInformation UDPRecord[], int clientNumber);
static void sendTCPData(TCPClientInformation TCPRecord[], TCPPacket toSend);
static void removeTCPEntry(TCPLocalInformation *TCPInformation, TCPClientInformation *TCPRecord, int clientNumber);
static void receiveClientNumber(TCPLocalInformation *TCPInformation);
static void removeUDPEntry(UDPClientInformation UDPRecord[], int clientNumber);
void initiateServerTCPCapability(TCPLocalInformation *TCPInformation);
void InitiateClientTCPCapability(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[]);
static void manageServerTCPActivity(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[], UDPClientInformation UDPRecord[]);
static void manageLobbyStatus(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[]);
static void sendNumberOfPlayersConnected(TCPClientInformation TCPRecord[], int entryToSendUpdateTo);
static void checkLobbyStatus(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[]);
static void tellTheClientsTheGameHasStarted(TCPClientInformation TCPRecord[]);

#endif