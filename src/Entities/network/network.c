#include "network.h"
#define MAX_CLIENTS 3

void setUpClient(Network *information, char IP_address[], int port) {
    // Open a socket on random port
    if (!(information->sourcePort = SDLNet_UDP_Open(0))) {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (SDLNet_ResolveHost(&information->serverAdress, IP_address, port) == -1) {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", IP_address, port, SDLNet_GetError());
        exit(1);
    }

    if (!((information->packetToSend = SDLNet_AllocPacket(sizeof(PlayerData))) && (information->packetToReceive = SDLNet_AllocPacket(sizeof(PlayerData))))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }
    
    
    printf(
        "Client connected to server at %s on port %d.\n", IP_address, port
    );
    
    information->gState = START;
    
    information->packetToSend->address.host = information->serverAdress.host;
    information->packetToSend->address.port = information->serverAdress.port;
}

void sendData(Network *information, PlayerData *toSend, Player *playerX) {
    if (checkDifference(toSend, playerX)) {
        prepareTransfer(toSend, playerX);
        commenceTransfer(information, toSend);
    }
}

static int checkDifference(PlayerData *toSend, Player *playerX) {
    if (toSend->positionX != playerX->position.x || toSend->positionY != playerX->position.y) {
        return 1;
    }
    else {
        return 0;
    }
}

static void prepareTransfer(PlayerData *toSend, Player *playerX) {
    toSend->player = playerX->player;
    toSend->positionX = playerX->position.x;
    toSend->positionY = playerX->position.y;
    toSend->frame = playerX->frame;
}

static void commenceTransfer(Network *information, PlayerData *toSend) {
    // Serialize the struct into a byte array
    char buf[sizeof(PlayerData)];
    memcpy(buf, toSend, sizeof(PlayerData));

    // Send the byte array as the payload of the UDP packet
    information->packetToSend->data = (Uint8*)buf;
    information->packetToSend->len = sizeof(PlayerData);
    information->packetToSend->address = information->serverAdress;
    information->packetToSend->channel = -1;

    if (SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend))
    {
        //printf("Sent: %d %d %d %d\n", toSend->player, toSend->positionX, toSend->positionY, toSend->frame);
    }
}

void receiveData(Network *information, Player *player1, Player *player2, Player *player3) {
    
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        PlayerData temporary;
        memcpy(&temporary, (char *) information->packetToReceive->data, sizeof(PlayerData));
        printf("receiveData-Received: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
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

	if (!((information->packetToReceive = SDLNet_AllocPacket(sizeof(PlayerData))) && (information->packetToSend = SDLNet_AllocPacket(sizeof(PlayerData))))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(1);
	}else{
        //printf("Packets allocated.\n");
    }

    information->gState = START;
    information->nrOfClients = 0;
    printf(
        "Server opened on port %d.\n",
        port
    );

}


void manageServerDuties(Network *information, AddressBook *record, Player *player1, Player *player2, Player *player3, PlayerData *toSend) {

    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        // Assuming `buf` is the received byte array
        PlayerData *receivedData = (PlayerData *) information->packetToReceive->data;
        memcpy(receivedData, information->packetToReceive->data, sizeof(PlayerData));

        registerSourceInformation(information, receivedData, record, player1, player2, player3);
        forwardreceivedPacket(information, receivedData, record, player1, player2, player3);
    }
    /*if (checkDifference(toSend, player2)){
        sendHostPlayerPacket(information, record, toSend, player2);
    }*/
}


void initiateAddressBook(AddressBook *record) {
    for(int i = 0; i < MAX_CLIENTS; i++){
        record->clients[i].id.ip = 0;
        record->clients[i].id.port = 0;
    }
}

static void sendServerCopy(Network *information, Uint32 clientIP, Uint32 clientPort, Player *player) {
    PlayerData temporary;

    // Serialize the struct into a byte array
    char buf[sizeof(PlayerData)];
    memcpy(&temporary, (char * ) information->packetToReceive->data, sizeof(PlayerData));
    applyReceivedData(player, &temporary); /// Implement received packet changes locally
    memcpy(buf, &temporary, sizeof(PlayerData));

    // Send the byte array as the payload of the UDP packet
    information->packetToSend->data = (Uint8*)buf;
    information->packetToSend->len = sizeof(PlayerData);
    information->packetToSend->address.host = clientIP;
    information->packetToSend->address.port = clientPort;
    information->packetToSend->channel = -1;

    if (SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend))
    {
        printf("Sent: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
    }
}

static void sendHostPlayerPacket(Network *information, AddressBook *record, PlayerData *toSend, Player *host) {
    printf("1");
    prepareTransfer(toSend, host);
    printf(
        "About to send: %d %d %d %d\n",
        toSend->player,
        toSend->positionX,
        toSend->positionY,
        toSend->frame
    );
    printf("2");
    if (record->clients[0].id.ip != 0) {
        PlayerData temporary;
        // Serialize the struct into a byte array
        char buf[sizeof(PlayerData)];
        memcpy(&temporary, toSend, sizeof(PlayerData));
        memcpy(buf, &temporary, sizeof(PlayerData));

        // Send the byte array as the payload of the UDP packet
        information->packetToSend->data = (Uint8*)buf;
        information->packetToSend->len = sizeof(PlayerData);
        information->packetToSend->address.host = record->clients[0].id.ip;
        information->packetToSend->address.port = record->clients[0].id.port;
        information->packetToSend->channel = -1;

        if (SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend))
        {
            printf("Sent: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
        }
    }
    if (record->clients[1].id.ip != 0) {
        information->packetToSend->address.host = record->clients[1].id.ip;	
        information->packetToSend->address.port = record->clients[1].id.port;
        if(SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend)){
            printf("Sent 2: %d %d %d %d\n", toSend->player, toSend->positionX, toSend->positionY, toSend->frame);
        };
    }    
    if (record->clients[2].id.ip != 0) {
        information->packetToSend->address.host = record->clients[2].id.ip;	
        information->packetToSend->address.port = record->clients[2].id.port;
        if(SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend)){
            printf("Sent 3: %d %d %d %d\n", toSend->player, toSend->positionX, toSend->positionY, toSend->frame);
        };
    }
}

static void applyReceivedData(Player *playerX, PlayerData *toSend) {
    playerX->position.x = toSend->positionX;
    playerX->position.y = toSend->positionY;
    playerX->frame = toSend->frame;
}

static void registerSourceInformation(Network *information, PlayerData *receivedData, AddressBook *record, Player *player1, Player *player2, Player *player3) {
    if (record->clients[0].id.ip == 0 && record->clients[0].id.port == 0) {
        printf("Client 1 joined\n--------------------\n");
        record->clients[0].id.ip = information->packetToReceive->address.host;
        record->clients[0].id.port = information->packetToReceive->address.port;
        record->clients[0].player.isConnected = true;
        // Increment the number of clients that have joined
        (information->nrOfClients)++;
    }else if (information->packetToReceive->address.port != record->clients[0].id.port  && record->clients[1].id.ip == 0) {
        printf("Client 2 joined\n--------------------\n");
        record->clients[1].id.ip = information->packetToReceive->address.host;
        record->clients[1].id.port = information->packetToReceive->address.port;
        record->clients[1].player.isConnected = true;
        // Increment the number of clients that have joined
        (information->nrOfClients)++;
    }else if (
        information->packetToReceive->address.port != record->clients[0].id.port && 
        information->packetToReceive->address.port != record->clients[1].id.port && 
        record->clients[2].id.ip == 0
        ) {
        printf("Client 3 joined\n--------------------\n");
        record->clients[2].id.ip = information->packetToReceive->address.host;
        record->clients[2].id.port = information->packetToReceive->address.port;
        record->clients[2].player.isConnected = true;
        // Increment the number of clients that have joined
        (information->nrOfClients)++;
    }
    for (int i = 0; i < MAX_CLIENTS; i++){
            printf("Client %d: %d %d\n", i+1, record->clients[i].id.ip, record->clients[i].id.port);
        }
}

void forwardreceivedPacket(Network *information, PlayerData *receivedData, AddressBook *record, Player *player1, Player *player2, Player *player3) {
    if (information->packetToReceive->address.port == record->clients[0].id.port) {
        printf("Client 1, ");
        if (record->clients[1].id.ip != 0) {
            sendServerCopy(information, record->clients[1].id.ip, record->clients[1].id.port, player1);
        }
        if (record->clients[2].id.ip != 0) {
            sendServerCopy(information, record->clients[2].id.ip, record->clients[2].id.port, player1);
        }
    }else if (information->packetToReceive->address.port == record->clients[1].id.port) {
        printf("Client 2, ");
        if (record->clients[0].id.ip != 0) {
            sendServerCopy(information, record->clients[0].id.ip, record->clients[0].id.port, player2);
        }
        if (record->clients[2].id.ip != 0) {
            sendServerCopy(information, record->clients[2].id.ip, record->clients[2].id.port, player2);
        }
    }else if (information->packetToReceive->address.port == record->clients[2].id.port) {
        printf("Client 3, ");
        if (record->clients[0].id.ip != 0) {
            sendServerCopy(information, record->clients[0].id.ip, record->clients[0].id.port, player3);
        }
        if (record->clients[1].id.ip != 0) {
            sendServerCopy(information, record->clients[1].id.ip, record->clients[1].id.port, player3);
        }
    }

    for (int i = 0; i < MAX_CLIENTS; i++){
        printf("Client %d: %d %d\n", i+1, record->clients[i].id.ip, record->clients[i].id.port);
    }
}