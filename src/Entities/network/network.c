#include "network.h"

Network setUpNetwork(char IP_address[], int port) {
    Network information;

    /* Open a socket on random port */
    if (!(information.sourcePort = SDLNet_UDP_Open(0))) {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    /* Resolve server name */
    if (SDLNet_ResolveHost(&information.destination, IP_address, port) == -1) {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", IP_address, port, SDLNet_GetError());
        exit(1);
    }

    /* Allocate memory for the packets */
    if (!(information.dataToSend1 = SDLNet_AllocPacket(512))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (!(information.dataToSend2 = SDLNet_AllocPacket(512))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    information.dataToSend1->address.host = information.destination.host;	//Set the destination host
    information.dataToSend1->address.port = information.destination.port;	// And destination port

    return information;
}

void sendData(Network *information, int x_pos, int y_pos) {
    /*
    udpData.x = x_pos;
    udpData.y = y.pos;
    memcpy(p->data, &udpData, sizeof(updData));
    p->len = sizeof(updData);
    */

    sprintf((char *)information->dataToSend1->data, "%d %d\n", (int) x_pos, (int) y_pos);    
    information->dataToSend1->len = strlen((char *)information->dataToSend1->data) + 1;
    SDLNet_UDP_Send(information->sourcePort, -1, information->dataToSend1);
}