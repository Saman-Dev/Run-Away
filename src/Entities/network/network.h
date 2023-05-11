#ifndef NETWORK_H
#define NETWORK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../player/player.h"

typedef struct {
    UDPsocket sourcePort;
	IPaddress destination;
	UDPpacket *packetToSend;
	UDPpacket *packetToReceive;
} Network;

typedef struct {
	int player;
	int positionX;
	int positionY;
	int frame;
} Cargo;

typedef struct {
    Uint32 clientIP1; 
    Uint32 clientPort1; 
    Uint32 clientIP2;
    Uint32 clientPort2;
	Uint32 clientIP3;
    Uint32 clientPort3;
} AddressBook;

void setUpClient(Network *information, char IP_address[], int port);
void sendData(Network *information, Cargo *toSend, Player *playerX);
static int checkDifference(Cargo *toSend, Player *playerX);
static void prepareTransfer(Cargo *toSend, Player *playerX);
static void commenceTransfer(Network *information, Cargo *toSend);
void receiveData(Network *information, Player *player1, Player *player2, Player *player3);
void setUpServer(Network *information, int port);
void manageServerDuties(Network *information, AddressBook *record, Player *player1, Player *player2, Player *player3, Cargo *toSend);
void initiateAddressBook(AddressBook *record);
static void registerSourceInformation(Network *information, AddressBook *record, Player *player1, Player *player2, Player *player3);
static void sendServerCopy(Network *information, Uint32 clientIP, Uint32 clientPort, Player *host);
static void sendHostPlayerPacket(Network *information, AddressBook *record, Cargo *toSend, Player *host);
static void applyReceivedData(Player *player, Cargo *toSend);

#endif