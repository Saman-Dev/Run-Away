#include "network.h"

Network setUpNetwork(char IP_address[], char port[]) {
    Network Data;

    /* Open a socket on random port */
    if (!(Data.sourcePort = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    /* Resolve server name */
    if (SDLNet_ResolveHost(&Data.destination, IP_address, port) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", IP_address, port, SDLNet_GetError());
        exit(1);
    }

    /* Allocate memory for the packets */
    if (!(Data.dataToSend1 = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (!(Data.dataToSend2 = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    return Data;
}