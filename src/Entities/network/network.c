#include "network.h"

Network setUpNetwork(char IP_address[], int port) {
    Network information;

    // Open a socket on random port
    if (!(information.sourcePort = SDLNet_UDP_Open(0))) {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (SDLNet_ResolveHost(&information.destination, IP_address, port) == -1) {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", IP_address, port, SDLNet_GetError());
        exit(1);
    }

    if (!(information.packetToSend1 = SDLNet_AllocPacket(sizeof(Cargo)))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    /*
    if (!(information.dataToSend2 = SDLNet_AllocPacket(512))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }
    */

    information.packetToSend1->address.host = information.destination.host;
    information.packetToSend1->address.port = information.destination.port;

    return information;
}

void sendData(Network *information, Cargo *toSend, Player *playerX) {
    if (checkDifference(toSend, playerX)) {
        prepareTransfer(toSend, playerX);
        commenceTransfer(information, toSend);
    }
}

int checkDifference(Cargo *toSend, Player *playerX) {
    if (toSend->positionX != playerX->position.x || toSend->positionY != playerX->position.y) {
        return 1;
    }
    else {
        return 0;
    }
}

void prepareTransfer(Cargo *toSend, Player *playerX) {
    toSend->positionX = playerX->position.x;
    toSend->positionY = playerX->position.y;
}

void commenceTransfer(Network *information, Cargo *toSend) {
    memcpy(information->packetToSend1->data, toSend, sizeof(Cargo)+1);   
    information->packetToSend1->len = sizeof(Cargo)+1;
    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend1);
}