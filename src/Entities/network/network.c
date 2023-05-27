#include "network.h"

void setUpClient(UDPLocalInformation *information, char IP_address[], int port) {
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

    information->packetToSend->address.host = information->destination.host;
    information->packetToSend->address.port = information->destination.port;
}

static void manageUDPClientConnection(UDPLocalInformation *information, PlayerData *toSend, Player players[], int playerNumber) {
    sendData(information, toSend, &players[playerNumber]);
    receiveData(information, players);
}

static void sendData(UDPLocalInformation *information, PlayerData *toSend, Player *playerX) {
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

static void commenceTransfer(UDPLocalInformation *information, PlayerData *toSend) {
    // Serialize the struct into a byte array
    char buf[sizeof(PlayerData)];
    memcpy(buf, toSend, sizeof(PlayerData));

    information->packetToSend->data = (Uint8 *)buf;
    information->packetToSend->len = sizeof(PlayerData);
    information->packetToSend->address.host = information->destination.host;
    information->packetToSend->address.port = information->destination.port;
    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend);
}

static void receiveData(UDPLocalInformation *information, Player players[]) {
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        PlayerData temporary;
        memcpy(&temporary, (char *)information->packetToReceive->data, sizeof(PlayerData));
        // printf("receiveData-Received: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
        for (int i = 0; players[i].player != 0; i++) {
            if (temporary.player == players[i].player) {
                applyReceivedData(&players[i], &temporary);
                break;
            }
        }
    }
}

void setUpServer(UDPLocalInformation *information, UDPClientInformation UDPRecord[], int port) {
    if (!(information->sourcePort = SDLNet_UDP_Open(port))) {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (!((information->packetToReceive = SDLNet_AllocPacket(sizeof(PlayerData))) && (information->packetToSend = SDLNet_AllocPacket(sizeof(PlayerData))))) {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    initiateUDPAddressBook(UDPRecord);
    printf("Server opened on port %d.\n", port);
}

static void manageServerDuties(UDPLocalInformation *information, UDPClientInformation UDPRecord[], Player players[], PlayerData *toSend) {
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive)) {
        // Assuming `buf` is the received byte array
        PlayerData *receivedData = (PlayerData *)information->packetToReceive->data;
        memcpy(receivedData, information->packetToReceive->data, sizeof(PlayerData));

        registerSourceInformation(information, UDPRecord);
        forwardreceivedPacket(information, receivedData, UDPRecord, players);
    }
    if (checkDifference(toSend, &players[2])) {
        sendHostPlayerPacket(information, UDPRecord, toSend, &players[2]);
    }
}

static void initiateUDPAddressBook(UDPClientInformation UDPRecord[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        UDPRecord[i].ip = 0;
        UDPRecord[i].port = 0;
        UDPRecord[i].active = false;
    }
}

static void sendServerCopy(UDPLocalInformation *information, Uint32 clientIP, Uint16 clientPort, Player *player) {
    PlayerData temporary;

    // Serialize the struct into a byte array
    char buf[sizeof(PlayerData)];
    memcpy(&temporary, (char *)information->packetToReceive->data, sizeof(PlayerData));
    memcpy(buf, &temporary, sizeof(PlayerData));

    // Send the byte array as the payload of the UDP packet
    information->packetToSend->data = (Uint8 *)buf;
    information->packetToSend->len = sizeof(PlayerData);
    information->packetToSend->address.host = clientIP;
    information->packetToSend->address.port = clientPort;
    information->packetToSend->channel = -1;

    if (SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend)) {
        printf("Sent: %d %d %d %d\n", temporary.player, temporary.positionX, temporary.positionY, temporary.frame);
    }
}

static void sendHostPlayerPacket(UDPLocalInformation *information, UDPClientInformation UDPRecord[], PlayerData *toSend, Player *host) {
    prepareTransfer(toSend, host);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (UDPRecord[i].active) {
            changeDestination(information, UDPRecord[i].ip, UDPRecord[i].port);
            commenceTransfer(information, toSend);
        }
    }
}

static void applyReceivedData(Player *playerX, PlayerData *toSend) {
    playerX->position.x = toSend->positionX;
    playerX->position.y = toSend->positionY;
    playerX->frame = toSend->frame;
}

static void registerSourceInformation(UDPLocalInformation *information, UDPClientInformation UDPRecord[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (UDPRecord[i].ip == information->packetToReceive->address.host && UDPRecord[i].port == information->packetToReceive->address.port) {
            return;
        }
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!UDPRecord[i].active) {
            UDPRecord[i].ip = information->packetToReceive->address.host;
            UDPRecord[i].port = information->packetToReceive->address.port;
            UDPRecord[i].active = true;

            printf("Client %d joined\n---------------\n", i);
            for (int j = 0; j < MAX_CLIENTS; j++) {
                printf("Client %d: %d %d\n", j, UDPRecord[j].ip, UDPRecord[j].port);
            }
            return;
        }
    }
}

static void forwardreceivedPacket(UDPLocalInformation *information, PlayerData *receivedData, UDPClientInformation UDPRecord[], Player players[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (UDPRecord[i].port == information->packetToReceive->address.port) {
            for (int j = 0; j < MAX_CLIENTS; j++) {
                if (i == j) {
                    applyReceivedData(&players[i], receivedData);
                }
                else if (UDPRecord[j].active) {
                    sendServerCopy(information, UDPRecord[j].ip, UDPRecord[j].port, &players[i]);
                }
            }
            break;
        }
    }
}

static void changeDestination(UDPLocalInformation *information, Uint32 clientIP, Uint16 clientPort) {
    information->destination.host = clientIP;
    information->destination.port = clientPort;
}

void resetClientNetwork(NetworkBundle *networkData) {
    SDLNet_TCP_Close(networkData->TCPInformation.socket);
    SDLNet_UDP_Close(networkData->UDPInformation.sourcePort);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        networkData->TCPRecord[i].active = false;
    }
    networkData->toSend.positionX = 0;
    networkData->toSend.positionY = 0;
    networkData->TCPInformation.socket = NULL;
}

///// TCP /////
static void addClient(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!TCPRecord[i].active) {
            TCPRecord[i].socket = SDLNet_TCP_Accept(TCPInformation->socket);
            if (TCPRecord[i].socket == NULL) {
                return;
            }
            TCPRecord[i].active = true;
            printf("Established a connection with client: %d\n", i);
            SDLNet_TCP_AddSocket(TCPInformation->set, TCPRecord[i].socket);
            sendClientNumber(TCPRecord[i].socket, i);
            break;
        }
    }
}

static void sendClientNumber(TCPsocket clientSocket, int numberToAssign) {
    TCPPacket toSend = {false, 0, {NULL, false}};
    toSend.playerNumber = numberToAssign;
    SDLNet_TCP_Send(clientSocket, &toSend, sizeof(TCPPacket));
}

static void receiveTCPData(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[], UDPClientInformation UDPRecord[], int clientNumber) {
    TCPPacket toSend = {false, 0, {NULL, false}};
    if (SDLNet_TCP_Recv(TCPRecord[clientNumber].socket, &toSend, sizeof(TCPPacket)) <= 0) {
        removeTCPEntry(TCPInformation, &TCPRecord[clientNumber], clientNumber);
        removeUDPEntry(UDPRecord, clientNumber);
    }
    else {
        printf("Received the following data in variable inMenu: \"%d\"\n", toSend.inLobby);
        sendTCPData(TCPRecord, toSend);
    }
}

static void sendTCPData(TCPClientInformation TCPRecord[], TCPPacket toSend) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (TCPRecord[i].active) {
            if (i == toSend.playerNumber) {
                continue;
            }
            else {
                SDLNet_TCP_Send(TCPRecord[i].socket, &toSend, sizeof(TCPPacket));
            }
        }
    }
}

static void removeTCPEntry(TCPLocalInformation *TCPInformation, TCPClientInformation *TCPRecord, int clientNumber) {
    printf("Lost connection with client: %d\n", clientNumber);
    SDLNet_TCP_DelSocket(TCPInformation->set, TCPRecord->socket);
    SDLNet_TCP_Close(TCPRecord->socket);
    TCPRecord->socket = NULL;
    TCPRecord->active = false;
}

static void receiveClientNumber(TCPLocalInformation *TCPInformation) {
    TCPPacket toReceive = {false, 0, {NULL, false}};
    SDLNet_TCP_Recv(TCPInformation->socket, &toReceive, sizeof(TCPPacket));
    printf("Received the following number from server: %d\n", toReceive.playerNumber);
    TCPInformation->playerNumber = toReceive.playerNumber;
}

static void removeUDPEntry(UDPClientInformation UDPRecord[], int clientNumber) {
    UDPRecord[clientNumber].ip = 0;
    UDPRecord[clientNumber].port = 0;
    UDPRecord[clientNumber].active = false;
    printf("UDP entry for client %d was removed\n", clientNumber);
}

void initiateServerTCPCapability(TCPLocalInformation *TCPInformation) {
    TCPInformation->playerNumber = -1;
    TCPInformation->inLobby = true;
    TCPInformation->set = SDLNet_AllocSocketSet(MAX_CLIENTS + 1);

    if (SDLNet_ResolveHost(&TCPInformation->ip, NULL, 9999) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    TCPInformation->socket = SDLNet_TCP_Open(&TCPInformation->ip);
    if (!TCPInformation->socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(1);
    }

    SDLNet_TCP_AddSocket(TCPInformation->set, TCPInformation->socket);
}

void InitiateClientTCPCapability(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[], char IPAddress[]) {
    TCPInformation->inLobby = true;
    TCPInformation->set = SDLNet_AllocSocketSet(2);

    if (SDLNet_ResolveHost(&TCPInformation->ip, IPAddress, 9999) == -1) { // "127.0.0.1"
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        SDLNet_FreeSocketSet(TCPInformation->set);
        return;
    }

    TCPInformation->socket = SDLNet_TCP_Open(&TCPInformation->ip);
    if (TCPInformation->socket == NULL) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        SDLNet_TCP_Close(TCPInformation->socket);
        TCPInformation->socket = NULL;
        SDLNet_FreeSocketSet(TCPInformation->set);
        return;
    }

    SDLNet_TCP_AddSocket(TCPInformation->set, TCPInformation->socket);

    receiveClientNumber(TCPInformation);
    SDL_Delay(20); // Delay to give the server time to get to the right function for the beneeth function to work as intendeed if the game is already running
    checkLobbyStatus(TCPInformation, TCPRecord);
}

static void manageServerTCPActivity(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[], UDPClientInformation UDPRecord[]) {
    manageLobby(TCPInformation, TCPRecord);
    SDLNet_CheckSockets(TCPInformation->set, 0);

    if (SDLNet_SocketReady(TCPInformation->socket)) {
        addClient(TCPInformation, TCPRecord);
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (TCPRecord[i].active) {
            if (SDLNet_SocketReady(TCPRecord[i].socket) > 0) {
                receiveTCPData(TCPInformation, TCPRecord, UDPRecord, i);
            }
        }
    }
}

void manageNetwork(NetworkBundle *networkData, Player players[]) {
    if (networkData->TCPInformation.playerNumber == -1) {
        manageServerDuties(&networkData->UDPInformation, networkData->UDPRecord, players, &networkData->toSend); //players, client
        manageServerTCPActivity(&networkData->TCPInformation, networkData->TCPRecord, networkData->UDPRecord);
    }
    else {
        manageUDPClientConnection(&networkData->UDPInformation, &networkData->toSend, players, networkData->TCPInformation.playerNumber);
        checkLobbyStatus(&networkData->TCPInformation, networkData->TCPRecord);
    }
}

static void manageLobby(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[]) {
    if (TCPInformation->inLobby) {
        sendNumberOfPlayersConnected(TCPRecord);
    }
    else {
        sendNewInLobbyValue(TCPRecord);
    }
}

static void sendNumberOfPlayersConnected(TCPClientInformation TCPRecord[]) {
    TCPPacket toSend = {true, 0, {NULL, false}};
    static bool reference[MAX_CLIENTS] = {0};
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (TCPRecord[i].active != reference[i]) {
            for (int j = 0; j < MAX_CLIENTS; j++) {
                if (TCPRecord[j].active) {
                    for (int k = 0; k < MAX_CLIENTS; k++) {
                        toSend.TCPRecord[k].active = TCPRecord[k].active;
                    }
                    SDLNet_TCP_Send(TCPRecord[j].socket, &toSend, sizeof(TCPPacket));
                }
            }
            reference[i] = TCPRecord[i].active;
        }
    }
}

static void checkLobbyStatus(TCPLocalInformation *TCPInformation, TCPClientInformation TCPRecord[]) {
    TCPPacket toReceive = {false, 0, {NULL, false}};
    SDLNet_CheckSockets(TCPInformation->set, 0);
    if (SDLNet_SocketReady(TCPInformation->socket)) {
        SDLNet_TCP_Recv(TCPInformation->socket, &toReceive, sizeof(TCPPacket));
        if (toReceive.inLobby) {
            for (int i = 0; i < MAX_CLIENTS; i++) {
                TCPRecord[i].active = toReceive.TCPRecord[i].active;
                printf("Received the following value from the server regarding connected players: %d\n", toReceive.TCPRecord[i].active);
            }
            printf("\n");
        }
        else {
            TCPInformation->inLobby = false;
        }
    }
}

static void sendNewInLobbyValue(TCPClientInformation TCPRecord[]) {
    TCPPacket toSend = {false, 0, {NULL, false}};
    static bool reference[MAX_CLIENTS] = {0};
    for (int i = 0; i < MAX_CLIENTS; i++) {
    if (TCPRecord[i].active != reference[i]) {
        if (TCPRecord[i].active) {
            SDLNet_TCP_Send(TCPRecord[i].socket, &toSend, sizeof(TCPPacket));
        }
        reference[i] = TCPRecord[i].active;
        }
    } 
}