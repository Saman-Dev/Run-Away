#include <SDL2/SDL_net.h>

#include "../player/player.h"

typedef struct {
    UDPsocket sourcePort;
	IPaddress destination;
	UDPpacket *packetToSend1;
	UDPpacket *packetToReceive1;
} Network;

typedef struct {
	int positionX;
	int positionY;
	int frame;
} Cargo;

typedef struct {
    Uint32 clientIP1; 
    Uint32 clientPort1; 
    Uint32 clientIP2;
    Uint32 clientPort2;
} addressBook;

Network setUpClient(char IP_address[], int port);
void sendData(Network *information, Cargo *toSend, Player *playerX);
static int checkDifference(Cargo *toSend, Player *playerX);
static void prepareTransfer(Cargo *toSend, Player *playerX);
static void commenceTransfer(Network *information, Cargo *toSend);
void receiveData(Network information, Cargo *toReceive, Player *playerX);
Network setUpServer(int port);
void manageServerDuties(Network *information, addressBook *record, Cargo *toSend1);