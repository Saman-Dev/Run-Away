#include <SDL2/SDL_net.h>

#include "../player/player.h"

typedef struct {
    UDPsocket sourcePort;
	IPaddress destination;
	UDPpacket *packetToSend1;
	UDPpacket *packetToSend2;
} Network;

typedef struct {
	int positionX;
	int positionY;
} Cargo;

Network setUpNetwork(char IP_address[], int port);
void sendData(Network *information, Cargo *toSend, Player *playerX);
int checkDifference(Cargo *toSend, Player *playerX);
void prepareTransfer(Cargo *toSend, Player *playerX);
void commenceTransfer(Network *information, Cargo *toSend);

//void sendData(Network *information, int x_pos, int y_pos);