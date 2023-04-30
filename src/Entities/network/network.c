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

    if (!(information.packetToReceive1 = SDLNet_AllocPacket(sizeof(Cargo)))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

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

static int checkDifference(Cargo *toSend, Player *playerX) {
    if (toSend->positionX != playerX->position.x || toSend->positionY != playerX->position.y) {
        return 1;
    }
    else {
        return 0;
    }
}

static void prepareTransfer(Cargo *toSend, Player *playerX) {
    toSend->positionX = playerX->position.x;
    toSend->positionY = playerX->position.y;
    toSend->frame = playerX->frame;
}

static void commenceTransfer(Network *information, Cargo *toSend) {
    memcpy(information->packetToSend1->data, toSend, sizeof(Cargo)+1);   
    information->packetToSend1->len = sizeof(Cargo)+1;
    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend1);
}

void receiveData(Network information, Cargo *toReceive, Player *playerX) {
    if (SDLNet_UDP_Recv(information.sourcePort, information.packetToReceive1)) {
        memcpy(toReceive, (char *) information.packetToReceive1->data, sizeof(Cargo));
        playerX->position.x = toReceive->positionX;
        playerX->position.y = toReceive->positionY;
        playerX->frame = toReceive->frame;
        printf("UDP packet received: %d %d\n", toReceive->positionX, toReceive->positionY);
    }
}