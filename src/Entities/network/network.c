#include "network.h"

Network setUpClient(char IP_address[], int port) {
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

Network setUpServer(int port) {
    Network information;

	if (SDLNet_Init() < 0) {
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(1);
	}

	if (!(information.sourcePort = SDLNet_UDP_Open(port))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(1);
	}

	if (!(information.packetToReceive1 = SDLNet_AllocPacket(sizeof(Cargo)))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(1);
	}

    if (!(information.packetToSend1 = SDLNet_AllocPacket(sizeof(Cargo)))) { 
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(1);
    }

    return information;
}

struct data {
   int x;
   int y;
   int status;
};



//addressBook record->

int quit, a, b;
struct data udpData = {0, 0, 0};
Cargo test;

void manageServerDuties(Network *information, addressBook *record, Cargo *toSend1) {
    if (SDLNet_UDP_Recv(information->sourcePort, information->packetToReceive1)) {
        if(record->clientIP1 == 0 && record->clientPort1 == 0){
            printf("Client 1\n");
            record->clientIP1 = information->packetToReceive1->address.host;
            record->clientPort1 = information->packetToReceive1->address.port;
        }else if(information->packetToReceive1->address.port != record->clientPort1  && record->clientIP2 == 0){
            printf("Client 2\n");
            record->clientIP2 = information->packetToReceive1->address.host;
            record->clientPort2 = information->packetToReceive1->address.port;
        }else{
            if (information->packetToReceive1->address.port == record->clientPort1){
                    printf("Recived data\n");
                    //
                    memcpy(&test, (char * ) information->packetToReceive1->data, sizeof(Cargo));
                    printf("UDP Packet data %d %d\n", test.positionX, test.positionY);
                    //
                if(record->clientIP2 != 0){
                    printf("Send to Client 2\n");
                    information->packetToSend1->address.host = record->clientIP2;	/* Set the destination host */
                    information->packetToSend1->address.port = record->clientPort2;
                    memcpy(&udpData, (char * ) information->packetToReceive1->data, sizeof(struct data));
                    printf("UDP Packet data %d %d\n", udpData.x, udpData.y);
                    memcpy((char *)information->packetToSend1->data, &udpData , sizeof(struct data)+1);
                    information->packetToSend1->len = sizeof(struct data)+1;
                    SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend1);
                }
            } else if (information->packetToReceive1->address.port == record->clientPort2){
                printf("Send to Client 1\n");    
                information->packetToSend1->address.host = record->clientIP1;	
                information->packetToSend1->address.port = record->clientPort1;
                memcpy(&udpData, (char * ) information->packetToReceive1->data, sizeof(struct data));
                printf("UDP Packet data %d %d\n", udpData.x, udpData.y);
                memcpy((char *)information->packetToSend1->data, &udpData , sizeof(struct data)+1);
                information->packetToSend1->len = sizeof(struct data)+1;
                SDLNet_UDP_Send(information->sourcePort, -1, information->packetToSend1);
            }
        

        /* Quit if packet contains "quit" */
        if (strcmp((char *)information->packetToSend1->data, "quit") == 0)
            quit = 1;
    }		
}
}