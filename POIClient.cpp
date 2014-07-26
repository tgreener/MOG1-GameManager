
#include "POIClient.h"

#define DEFAULT_BUFFER_SIZE 256

POIClient::POIClient(const ClientConnection& con) : cc(con){}

void POIClient::addPointOfInterest(PointOfInterestParser& poip) {
    if(!poip.create(cc)) {
        throw "Could not creat point of interest from parameteters.";
    }
    else {
        printNewPOIMessage();
    }
}

void POIClient::printNewPOIMessage() {
    unsigned char buffer[DEFAULT_BUFFER_SIZE];
    cc.receiveMessage((char*)buffer, DEFAULT_BUFFER_SIZE);

    if(buffer[0] == 1) {
        int id = *((int*)(buffer + 1));
        printf("New POI ID: %d\n", id);
    }
    else if(!buffer[0] && buffer[1] == 0xff) {
        printf("Server unable to creat point of interest.\n");
    }
    else {
        printf("%x; %x\n", buffer[0], *((unsigned int*)(buffer + 1)));
    }
}

void POIClient::deletePointOfInterest(PointOfInterestParser& poip) {
    if(!poip.deletePOI(cc)) {
        throw "Could not create delete request.";
    }
    else {
        printPOIDeletedMessage();
    }
}

void POIClient::printPOIDeletedMessage() {
    unsigned char buffer[DEFAULT_BUFFER_SIZE];
    cc.receiveMessage((char*)buffer, DEFAULT_BUFFER_SIZE);

    if(buffer[0] == 1) {
        printf("Point of interest deleted.\n");
    }
    else if(!buffer[0] && buffer[1] == 0xff) {
        printf("Server unable to delete point of interest.\n");
    }
    else {
        printf("%s\n", buffer);
    }
}

void POIClient::printPointOfInterest(PointOfInterestParser& poip) {
    if(!poip.getPOIData(cc)) {
        throw "Could not carry out POI retrieval request.";
    }
    else {
        printPOIData();
    }
}

void POIClient::printAllPointOfInterest() {
    int messageLength = 3;
    unsigned char message[messageLength];
    
    message[0] = 0x03;
    message[1] = 0x04;
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
            throw "Server error retrieving all POI data.";
        }
        
        unsigned int replyIndex = 0;
        
        unsigned char* readBuffer = new unsigned char[replyLength];
        cc.receiveMessage((char*)readBuffer, replyLength);
        
        while(replyLength > 0 && readBuffer[replyIndex] != 255) {
            unsigned char id = readBuffer[replyIndex];
            unsigned char soil = readBuffer[replyIndex + 1];
            unsigned char stone = readBuffer[replyIndex + 2];
            unsigned char wild = readBuffer[replyIndex + 3];
            unsigned char* name = readBuffer + replyIndex + 4;
            
            replyIndex += 4 + (strlen((char*)name) + 1);
            
            printf("%s: %d, %d, %d, %d\n", name, id, soil, stone, wild);
        }
        
        delete[] readBuffer;
    }
    else {
        throw "Could not retrieval all POI data.";
    }
}

void POIClient::printPOIData() {
    unsigned char buffer[DEFAULT_BUFFER_SIZE];
    cc.receiveMessage((char*)buffer, DEFAULT_BUFFER_SIZE);
    
    if(buffer[0] == 1) {
        unsigned char id = buffer[1];
        unsigned char soil = buffer[2];
        unsigned char stone = buffer[3];
        unsigned char wild = buffer[4];
        
        unsigned char* name = buffer + 5;
        
        printf("POI Data retrieved:\n");
        printf("%s {\n\t", name);
        printf("soil: %d\n\t", soil);
        printf("stone: %d\n\t", stone);
        printf("wild: %d\n", wild);
        printf("}\n");
        fflush(stdout);
    }
    else {
        printf("Error retrieving POI data.\n");
    }
}

