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
    printf(
        "Client connected to server at %s on port %d.\n", IP_address, port
    );
    
    
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
    // Serialize the struct into a byte array
    char buf[sizeof(Cargo)];
    memcpy(buf, toSend, sizeof(Cargo));

    // Send the byte array as the payload of the UDP packet
    information->packetToSend->data = (Uint8*)buf;
    information->packetToSend->len = sizeof(Cargo);
    information->packetToSend->address = information->destination;
    information->packetToSend->channel = -1;

    if (SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend))
    {
        //printf("Sent: %d %d %d %d\n", toSend->player, toSend->positionX, toSend->positionY, toSend->frame);
    }
}

void receiveData(Network *information, Player *player1, Player *player2, Player *player3) {
    
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        Cargo temporary;
        memcpy(&temporary, (char *) information->packetToReceive->data, sizeof(Cargo));
        //printf("receiveData-Received: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
        if (temporary.player == 1) {
            applyReceivedData(player1, &temporary);
        }
        else if (temporary.player == 2) {
            applyReceivedData(player2, &temporary);
        }
        else if (temporary.player == 3) {
            applyReceivedData(player3, &temporary);  
        }
    }
}

void setUpServer(Network *information, int port) {
	if (SDLNet_Init() < 0) {
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(1);
	}else{
        //printf("SDLNet initialised.\n");
    }

	if (!(information->sourcePort = SDLNet_UDP_Open(port))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(1);
	}else{
        //printf("Server opened on port %d.\n", port);
    }

	if (!((information->packetToReceive = SDLNet_AllocPacket(sizeof(Cargo))) && (information->packetToSend = SDLNet_AllocPacket(sizeof(Cargo))))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(1);
	}else{
        //printf("Packets allocated.\n");
    }
    printf(
        "Server opened on port %d.\n",
        port
    );
}

void manageServerDuties(Network *information, AddressBook *record, Player *player1, Player *player2, Player *player3, Cargo *toSend) {
    
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        // Assuming `buf` is the received byte array
        Cargo *toRecv = (Cargo *) information->packetToReceive->data;
        memcpy(toRecv, information->packetToReceive->data, sizeof(Cargo));
        /*printf(
            "UDP Packet incoming: %d %d %d %d\n", 
            toRecv->player, 
            toRecv->positionX, 
            toRecv->positionY, 
            toRecv->frame
        );*/

        registerSourceInformation(information, record, player1, player2, player3);

        /*if (checkDifference(toSend, player3)){
            sendHostPlayerPacket(information, record, toSend, player3);
        }*/
    }
}

void initiateAddressBook(AddressBook *record) {
    record->clientIP1 = 0;
    record->clientIP2 = 0;
    record->clientIP3 = 0;
    record->clientPort1 = 0;
    record->clientPort2 = 0;
    record->clientPort3 = 0;
}

static void sendServerCopy(Network *information, Uint32 clientIP, Uint32 clientPort, Player *player) {
    Cargo temporary;

    // Serialize the struct into a byte array
    char buf[sizeof(Cargo)];
    memcpy(&temporary, (char * ) information->packetToReceive->data, sizeof(Cargo));
    applyReceivedData(player, &temporary); /// Implement received packet changes locally
    memcpy(buf, &temporary, sizeof(Cargo));

    // Send the byte array as the payload of the UDP packet
    information->packetToSend->data = (Uint8*)buf;
    information->packetToSend->len = sizeof(Cargo);
    information->packetToSend->address.host = clientIP;
    information->packetToSend->address.port = clientPort;
    information->packetToSend->channel = -1;

    if (SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend))
    {
        printf("Sent: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
    }
}

static void sendHostPlayerPacket(Network *information, AddressBook *record, Cargo *toSend, Player *host) {
    prepareTransfer(toSend, host);
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
    if (record->clientIP3) {
        information->packetToSend->address.host = record->clientIP3;	
        information->packetToSend->address.port = record->clientPort3;
        SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
    }
}

static void applyReceivedData(Player *playerX, Cargo *toSend) {
    playerX->position.x = toSend->positionX;
    playerX->position.y = toSend->positionY;
    playerX->frame = toSend->frame;
}

static void registerSourceInformation(Network *information, AddressBook *record, Player *player1, Player *player2, Player *player3) {
    if (record->clientIP1 == 0 && record->clientPort1 == 0) {
        printf("Client 1 joined\n");
        record->clientIP1 = information->packetToReceive->address.host;
        record->clientPort1 = information->packetToReceive->address.port;
    }else if (information->packetToReceive->address.port != record->clientPort1  && record->clientIP2 == 0) {
        printf("Client 2 joined\n");
        record->clientIP2 = information->packetToReceive->address.host;
        record->clientPort2 = information->packetToReceive->address.port;
    }else if (information->packetToReceive->address.port != record->clientPort2  && record->clientIP3 == 0) {
        printf("Client 3 joined\n");
        record->clientIP3 = information->packetToReceive->address.host;
        record->clientPort3 = information->packetToReceive->address.port;
    }else{
        if (information->packetToReceive->address.port == record->clientPort1) {
            printf("Client 1, ");
            if (record->clientIP2 != 0) {
                sendServerCopy(information, record->clientIP2, record->clientPort2, player1);
            }
            if (record->clientIP3 != 0) {
                sendServerCopy(information, record->clientIP3, record->clientPort3, player1);
            }
        }else if (information->packetToReceive->address.port == record->clientPort2) {
            printf("Client 2, ");
            if (record->clientIP1 != 0) {
                sendServerCopy(information, record->clientIP1, record->clientPort1, player2);
            }
            if (record->clientIP3 != 0) {
                sendServerCopy(information, record->clientIP3, record->clientPort3, player2);
            }
        }else if (information->packetToReceive->address.port == record->clientPort3) {
            printf("Client 3, ");
            if (record->clientIP1 != 0) {
                sendServerCopy(information, record->clientIP1, record->clientPort1, player3);
            }
            if (record->clientIP2 != 0) {
                sendServerCopy(information, record->clientIP2, record->clientPort2, player3);
            }
        }
    }
}