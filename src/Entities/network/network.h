#include <SDL2/SDL_net.h>

typedef struct {
    UDPsocket sourcePort;
	IPaddress destination;
	UDPpacket *dataToSend1;
	UDPpacket *dataToSend2;
} Network;

Network setUpNetwork(char IP_address[], int port);
void sendData(Network *information, int x_pos, int y_pos);