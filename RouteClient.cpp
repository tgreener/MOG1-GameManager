
#include "RouteClient.h"
#include <stdio.h>

#define DEFAULT_BUFFER_SIZE 256

void printBool(bool b) {
    if(b) printf("true");
    else printf("false");
}

RouteClient::RouteClient(const ClientConnection& con) : cc(con) {
}
    
void RouteClient::addRoute(RouteParser& rp) {
    if(!rp.createRoute(cc)) {
        throw "Could not create route from parameters";
    }
    else {
        printNewRouteMessage();
    }
}

void RouteClient::printNewRouteMessage() {
    unsigned char buffer[DEFAULT_BUFFER_SIZE];
    cc.receiveMessage((char*)buffer, DEFAULT_BUFFER_SIZE);

    if(buffer[0] == 1) {
        int id = *((int*)(buffer + 1));
        printf("New Route ID: %d\n", id);
    }
    else if(!buffer[0] && buffer[1] == 0xff) {
        printf("Server unable to creat route.\n");
    }
    else {
        printf("%x; %x\n", buffer[0], *((unsigned int*)(buffer + 1)));
    }
}

void RouteClient::deleteRoute(RouteParser& rp) {
    if(!rp.deleteRoute(cc)) {
        throw "Could not delete route from parameters";
    }
    else {
        printRouteDeletedMessage();
    }
}

void RouteClient::printRouteDeletedMessage() {
    unsigned char buffer[DEFAULT_BUFFER_SIZE];
    cc.receiveMessage((char*)buffer, DEFAULT_BUFFER_SIZE);

    if(buffer[0] == 1) {
        printf("Route deleted.\n");
    }
    else if(!buffer[0] && buffer[1] == 0xff) {
        printf("Server unable to delete route.\n");
    }
    else {
        printf("%s\n", buffer);
    }
}

void RouteClient::printRoute(RouteParser& rp) {
    if(!rp.getRoute(cc)) {
        throw "Could not fetch route from parameters";
    }
    else {
        printRouteData();
    }
}

void RouteClient::printRouteData() {
    unsigned char buffer[DEFAULT_BUFFER_SIZE];
    cc.receiveMessage((char*)buffer, DEFAULT_BUFFER_SIZE);
    
    if(buffer[0] == 1) {
        unsigned char id = buffer[1];
        unsigned char poiAID = buffer[2];
        unsigned char poiBID = buffer[3];
        unsigned char difficulty = buffer[4];
        bool bidirectional = buffer[5] != 0;
        bool reverse = buffer[6] != 0;
        
        printf("Route Data retrieved:\n");
        printf("Route {\n\t");
        printf("poiA: %d\n\t", poiAID);
        printf("poiB: %d\n\t", poiBID);
        printf("difficulty: %d\n\t", difficulty);
        printf("bidirectional: ");
        printBool(bidirectional);
        printf("\n\treverse: ");
        printBool(reverse);
        printf("\n}\n");
        fflush(stdout);
    }
    else {
        printf("Error retrieving POI data.\n");
    }
}

void RouteClient::printAllRoutes() {
    int messageLength = 3;
    unsigned char message[messageLength];
    
    message[0] = 0x03;
    message[1] = 0x08;
    message[2] = 0xff;
    
    if(cc.sendMessage(message, messageLength)) {
        int headerBufferSize = 2 + sizeof(unsigned int);
        unsigned char headerBuffer[headerBufferSize];
        unsigned int replyLength = 0;
        
        cc.receiveMessage((char*)headerBuffer, headerBufferSize);
        if(headerBuffer[0] == 1) {
            unsigned int numBytesInLengthMessage = headerBuffer[1];
            replyLength = *((unsigned int*)(headerBuffer + 2));
            printf("Bytes: %d, Length: %d\n", numBytesInLengthMessage, replyLength);
        }
        else {
            throw "Server error retrieving all route data.";
        }
        
        unsigned int replyIndex = 0;
        
        unsigned char* readBuffer = new unsigned char[replyLength];
        cc.receiveMessage((char*)readBuffer, replyLength);
        
        while(replyLength > 0 && readBuffer[replyIndex] != 255) {
            unsigned char id = readBuffer[replyIndex];
            unsigned char poiAID = readBuffer[replyIndex + 1];
            unsigned char poiBID = readBuffer[replyIndex + 2];
            unsigned char difficulty = readBuffer[replyIndex + 3];
            bool bidirectional = readBuffer[replyIndex + 4] != 0;
            bool reverse = readBuffer[replyIndex + 5] != 0;
            
            replyIndex += 6;
            
            printf("Route %d: %d, %d, %d, %d, %d\n", id, poiAID, poiBID, difficulty, bidirectional, reverse);
        }
        
        delete[] readBuffer;
    }
}
