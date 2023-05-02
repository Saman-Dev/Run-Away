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
    toSend->positionX = playerX->position.x;
    toSend->positionY = playerX->position.y;
    toSend->frame = playerX->frame;
}

static void commenceTransfer(Network *information, Cargo *toSend) {
    memcpy(information->packetToSend->data, toSend, sizeof(Cargo)+1);   
    information->packetToSend->len = sizeof(Cargo)+1;
    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
}

void receiveData(Network information, Cargo *toReceive, Player *playerX) {
    if (SDLNet_UDP_Recv(information.sourcePort, information.packetToReceive)) {
        memcpy(toReceive, (char *) information.packetToReceive->data, sizeof(Cargo));
        playerX->position.x = toReceive->positionX;
        playerX->position.y = toReceive->positionY;
        playerX->frame = toReceive->frame;
        printf("UDP packet received: %d %d\n", toReceive->positionX, toReceive->positionY);
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

void manageServerDuties(Network *information, AddressBook *record, Cargo *toSend) {
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
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
        else {
            if (information->packetToReceive->address.port == record->clientPort1) {
                if (record->clientIP2 != 0) {
                    printf("Send to Client 2\n");
                    information->packetToSend->address.host = record->clientIP2;
                    information->packetToSend->address.port = record->clientPort2;
                    memcpy(toSend, (char * ) information->packetToReceive->data, sizeof(Cargo));
                    printf("UDP Packet data %d %d\n", toSend->positionX, toSend->positionY);
                    memcpy((char *)information->packetToSend->data, toSend , sizeof(Cargo)+1);
                    information->packetToSend->len = sizeof(Cargo)+1;
                    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
                }
            } 
            else if (information->packetToReceive->address.port == record->clientPort2) {
                printf("Send to Client 1\n");    
                information->packetToSend->address.host = record->clientIP1;	
                information->packetToSend->address.port = record->clientPort1;
                memcpy(toSend, (char * ) information->packetToReceive->data, sizeof(Cargo));
                printf("UDP Packet data %d %d\n", toSend->positionX, toSend->positionY);
                memcpy((char *)information->packetToSend->data, toSend , sizeof(Cargo)+1);
                information->packetToSend->len = sizeof(Cargo)+1;
                SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
            }
        }		
    }
}

void initiateAddressBook(AddressBook *record) {
    record->clientIP1 = 0;
    record->clientIP2 = 0;
    record->clientPort1 = 0;
    record->clientPort2 = 0;
    record->clientIP1 = 0;
}