#include "network.h"

void setUpClient(Network *information, char IP_address[], int port) {
    // Open a socket on random port
    if (!(information->sourcePort = SDLNet_UDP_Open(0))) {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (SDLNet_ResolveHost(&information->destination, IP_address, port) == -1) {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", IP_address, port, SDLNet_GetError());
        exit(1);
    }

    if (!((information->packetToSend = SDLNet_AllocPacket(sizeof(Cargo))) && (information->packetToReceive = SDLNet_AllocPacket(sizeof(Cargo))))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    information->packetToSend->address.host = information->destination.host;
    information->packetToSend->address.port = information->destination.port;
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
    toSend->player = playerX->player;
    toSend->positionX = playerX->position.x;
    toSend->positionY = playerX->position.y;
    toSend->frame = playerX->frame;
}

static void commenceTransfer(Network *information, Cargo *toSend) {
    memcpy(information->packetToSend->data, toSend, sizeof(Cargo)+1);   
    information->packetToSend->len = sizeof(Cargo)+1;
    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
}

void receiveData(Network information, Player *playerX, Player *playerY) {
    if (SDLNet_UDP_Recv(information.sourcePort, information.packetToReceive)) {
        Cargo temporary;
        memcpy(&temporary, (char *) information.packetToReceive->data, sizeof(Cargo));
        if (temporary.player == 1) {
            applyReceivedData(playerX, &temporary);
        }
        else if (temporary.player == 2) {
            applyReceivedData(playerX, &temporary);
        }
        else if (temporary.player == 3) {
            applyReceivedData(playerY, &temporary);  
        }
    }
}

void setUpServer(Network *information, int port) {
	if (SDLNet_Init() < 0) {
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(1);
	}

	if (!(information->sourcePort = SDLNet_UDP_Open(port))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(1);
	}

	if (!((information->packetToReceive = SDLNet_AllocPacket(sizeof(Cargo))) && (information->packetToSend = SDLNet_AllocPacket(sizeof(Cargo))))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(1);
	}
}

void manageServerDuties(Network *information, AddressBook *record, Player *playerX, Player *playerY, Player host, Cargo *toSend) {
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        registerSourceInformation(information, record);
        if (information->packetToReceive->address.port == record->clientPort1) {
            if (record->clientIP2 != 0) {
                sendServerCopy(information, record->clientIP2, record->clientPort2, playerX);
            }
        } 
        else if (information->packetToReceive->address.port == record->clientPort2) {
            sendServerCopy(information, record->clientIP1, record->clientPort1, playerY);
        }
    }
    if (checkDifference(toSend, &host)) {
        sendHostPlayerPacket(information, record, toSend, host);
    }
}

void initiateAddressBook(AddressBook *record) {
    record->clientIP1 = 0;
    record->clientIP2 = 0;
    record->clientPort1 = 0;
    record->clientPort2 = 0;
}

static void sendServerCopy(Network *information, Uint32 clientIP, Uint32 clientPort, Player *player) {
    Cargo temporary;
    information->packetToSend->address.host = clientIP;
    information->packetToSend->address.port = clientPort;
    memcpy(&temporary, (char * ) information->packetToReceive->data, sizeof(Cargo));
    applyReceivedData(player, &temporary); /// Implement received packet changes locally
    memcpy((char *)information->packetToSend->data, &temporary , sizeof(Cargo));
    information->packetToSend->len = sizeof(Cargo)+1;
    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
}

static void sendHostPlayerPacket(Network *information, AddressBook *record, Cargo *toSend, Player host) {
    prepareTransfer(toSend, &host);
    memcpy((char *)information->packetToSend->data, toSend , sizeof(Cargo));
    information->packetToSend->len = sizeof(Cargo)+1;
    if (record->clientIP1) {
        information->packetToSend->address.host = record->clientIP1;	
        information->packetToSend->address.port = record->clientPort1;
        SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
    }
    if (record->clientIP2) {
        information->packetToSend->address.host = record->clientIP2;	
        information->packetToSend->address.port = record->clientPort2;
        SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
    }
}

static void applyReceivedData(Player *playerX, Cargo *toSend) {
    playerX->position.x = toSend->positionX;
    playerX->position.y = toSend->positionY;
    playerX->frame = toSend->frame;
}

static void registerSourceInformation(Network *information, AddressBook *record) {
    if (record->clientIP1 == 0 && record->clientPort1 == 0) {
    printf("Client 1\n");
    record->clientIP1 = information->packetToReceive->address.host;
    record->clientPort1 = information->packetToReceive->address.port;
    }
    else if (information->packetToReceive->address.port != record->clientPort1  && record->clientIP2 == 0) {
    printf("Client 2\n");
    record->clientIP2 = information->packetToReceive->address.host;
    record->clientPort2 = information->packetToReceive->address.port;
    }
}