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

    if (!((information->packetToSend = SDLNet_AllocPacket(sizeof(PlayerData))) && (information->packetToReceive = SDLNet_AllocPacket(sizeof(PlayerData))))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    printf("Client connected to server at %s on port %d.\n", IP_address, port);
    
    information->gState = START;
    
    information->packetToSend->address.host = information->destination.host;
    information->packetToSend->address.port = information->destination.port;
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

    information->packetToSend->data = (Uint8*)buf;
    information->packetToSend->len = sizeof(PlayerData);
    information->packetToSend->address.host = information->destination.host;
    information->packetToSend->address.port = information->destination.port;
    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
}

void receiveData(Network *information, Player players[]) {
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        PlayerData temporary;
        memcpy(&temporary, (char *) information->packetToReceive->data, sizeof(PlayerData));
        //printf("receiveData-Received: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
        for (int i = 0; players[i].player != 0; i++) {
            if (temporary.player == players[i].player) {
                applyReceivedData(&players[i], &temporary);
                break;
            }
        }
    }
}

void setUpServer(Network *information, ClientID record[], int port) {
	if (!(information->sourcePort = SDLNet_UDP_Open(port))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(1);
	}

	if (!((information->packetToReceive = SDLNet_AllocPacket(sizeof(PlayerData))) && (information->packetToSend = SDLNet_AllocPacket(sizeof(PlayerData))))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(1);
	}

    initiateAddressBook(record);
    information->gState = START;
    information->nrOfClients = 0;
    printf("Server opened on port %d.\n", port);
}


void manageServerDuties(Network *information, ClientID record[], Player players[], PlayerData *toSend) {
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        // Assuming `buf` is the received byte array
        PlayerData *receivedData = (PlayerData *) information->packetToReceive->data;
        memcpy(receivedData, information->packetToReceive->data, sizeof(PlayerData));

        registerSourceInformation(information, receivedData, record);
        forwardreceivedPacket(information, receivedData, record, players);
    }
    if (checkDifference(toSend, &players[2])) {
        sendHostPlayerPacket(information, record, toSend, &players[2]);
    }
}


static void initiateAddressBook(ClientID record[]) {
    for(int i = 0; i < MAX_CLIENTS; i++){
        record[i].ip = 0;
        record[i].port = 0;
        record[i].connectedStatus = false;
    }
}

static void sendServerCopy(Network *information, Uint32 clientIP, Uint16 clientPort, Player *player) {
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

static void sendHostPlayerPacket(Network *information, ClientID record[], PlayerData *toSend, Player *host) {
    prepareTransfer(toSend, host);
    for (int i = 0; record[i].port != 0; i++) {
        changeDestination(information, record[i].ip, record[i].port);
        commenceTransfer(information, toSend);
    }
}

static void applyReceivedData(Player *playerX, PlayerData *toSend) {
    playerX->position.x = toSend->positionX;
    playerX->position.y = toSend->positionY;
    playerX->frame = toSend->frame;
}

static void registerSourceInformation(Network *information, PlayerData *receivedData, ClientID record[]) {
    if (information->nrOfClients < MAX_CLIENTS) {
        for (int i = 0; record[i].ip != 0; i++) {
            if (record[i].ip == information->packetToReceive->address.host && record[i].port == information->packetToReceive->address.port) {
                return;
            }
        }

        printf("Client %d joined\n--------------------\n", information->nrOfClients + 1);
        record[information->nrOfClients].ip = information->packetToReceive->address.host;
        record[information->nrOfClients].port = information->packetToReceive->address.port;
        record[information->nrOfClients].connectedStatus = true;
        (information->nrOfClients)++;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            printf("Client %d: %d %d\n", i+1, record[i].ip, record[i].port);
        }
    }
}

static void forwardreceivedPacket(Network *information, PlayerData *receivedData, ClientID record[], Player players[]) {
    for (int i = 0; record[i].port != 0; i++) {
        if (record[i].port == information->packetToReceive->address.port) {
            for (int j = 0; record[j].ip != 0; j++) {
                if (i == j) {
                    continue;
                }
                else {
                    sendServerCopy(information, record[j].ip, record[j].port, &players[i]);
                }
            }
            break;
        }
    }
}

static void changeDestination(Network *information, Uint32 clientIP, Uint16 clientPort) {
    information->destination.host = clientIP;
    information->destination.port = clientPort;
}

///// TCP /////

void addClient(TCPLocalInformation *TCPInformation, TCPClientInformation client[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!client[i].active) {
            client[i].socket = SDLNet_TCP_Accept(TCPInformation->socket);
            if (client[i].socket == NULL) {
                return;
            }
            client[i].active = true;
            printf("Established a connection with client: %d\n", i);
            SDLNet_TCP_AddSocket(TCPInformation->set, client[i].socket);
            sendClientNumber(client[i].socket, i);
            break;
        }
    }
}

void sendClientNumber(TCPsocket clientSocket, int numberToAssign) {
    TCPPacket toSend = {0, 0};
    toSend.playerNumber = numberToAssign;
    SDLNet_TCP_Send(clientSocket, &toSend, sizeof(TCPPacket));
}

void receiveTCPData(TCPLocalInformation *TCPInformation, TCPClientInformation client[], int clientNumber) {
    TCPPacket toSend = {0, 0};
    if (SDLNet_TCP_Recv(client[clientNumber].socket, &toSend, sizeof(TCPPacket)) <= 0) {
        removeClient(TCPInformation, &client[clientNumber], clientNumber);
    }
    else {
        printf("Received the following data: \"%u\"\n", toSend.command);
        sendTCPData(client, toSend);
    }
}

void sendTCPData(TCPClientInformation client[], TCPPacket toSend) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client[i].active) {
            if (i == toSend.playerNumber) {
                continue;
            }
            else {
                SDLNet_TCP_Send(client[i].socket, &toSend, sizeof(TCPPacket));
            }
        }
    }
}

void removeClient(TCPLocalInformation *TCPInformation, TCPClientInformation *client, int clientNumber) {
        printf("Lost connection with client: %d\n", clientNumber);
        SDLNet_TCP_DelSocket(TCPInformation->set, client->socket);
        SDLNet_TCP_Close(client->socket);
        client->socket = NULL;
        client->active = false;
}

void receiveClientNumber(TCPLocalInformation *TCPInformation) {
    TCPPacket toReceive = {0, 0};
    SDLNet_TCP_Recv(TCPInformation->socket, &toReceive, sizeof(TCPPacket));
    printf("Received the following number from server: %d\n", toReceive.playerNumber);
    TCPInformation->playerNumber = toReceive.playerNumber;
}

void initiateServerTCPCapability(TCPLocalInformation *TCPInformation) {
    TCPInformation->set = SDLNet_AllocSocketSet(MAX_CLIENTS+1);

    if(SDLNet_ResolveHost(&TCPInformation->ip, NULL, 9999) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    TCPInformation->socket = SDLNet_TCP_Open(&TCPInformation->ip);
    if(!TCPInformation->socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    SDLNet_TCP_AddSocket(TCPInformation->set, TCPInformation->socket);
}

void InitiateClientTCPCapability(TCPLocalInformation *TCPInformation) {
    TCPInformation->set = SDLNet_AllocSocketSet(2);

    if(SDLNet_ResolveHost(&TCPInformation->ip, "127.0.0.1", 9999) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    TCPInformation->socket = SDLNet_TCP_Open(&TCPInformation->ip);
    if(TCPInformation->socket == NULL) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    SDLNet_TCP_AddSocket(TCPInformation->set, TCPInformation->socket);

    receiveClientNumber(TCPInformation);
}

void manageServerTCPActivity(TCPLocalInformation *TCPInformation, TCPClientInformation client[]) {
    TCPPacket toSend = {0, 0};

    SDLNet_CheckSockets(TCPInformation->set, 0);

    if (SDLNet_SocketReady(TCPInformation->socket)) {
        addClient(TCPInformation, client);
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client[i].active) {
            if (SDLNet_SocketReady(client[i].socket) > 0) {
                receiveTCPData(TCPInformation, client, i);
            }
        }
    }
}