
#include "PointOfInterestParser.h"
#include <cstdlib>
#include <cerrno>
#include <cstring>

PointOfInterestParser::PointOfInterestParser(const char* n, const char* sl, const char* st, const char* wld) : id(0){
    soil = valFromString(sl);
    stone = valFromString(st);
    wilderness = valFromString(wld);
    
    setName(n);
    
    createReady = true;
}

PointOfInterestParser::PointOfInterestParser(const char* id) : createReady(false){
    this->id = strtol(id, NULL, 0);
    if(this->id <= 0) {
        throw "Exception: Point of interest attribute value out of range.";
    }
}

PointOfInterestParser::~PointOfInterestParser() {
}

unsigned char PointOfInterestParser::valFromString(const char* str) {
    int i = strtol(str, NULL, 0);
    
    if( i > 10 || i < 1) {
        throw "Exception: Point of interest attribute value out of range.";
    }
    
    unsigned char uc = i;
    
    return uc;
}

void PointOfInterestParser::setName(const char* str) {
    name = str;
}

bool PointOfInterestParser::isNew() {
    return createReady;
}

bool PointOfInterestParser::create(const ClientConnection& connection) {
    if(createReady) {
        int poiDataSize = 6;
        int bufferSize = name.length() + 1 + poiDataSize;
        unsigned char buffer[bufferSize];

        buffer[bufferSize - 1] = 0xff;
        memcpy(buffer + 5, name.c_str(), name.length() + 1);
        buffer[4] = wilderness;
        buffer[3] = stone;
        buffer[2] = soil;
        buffer[1] = 0x02;
        buffer[0] = 0x03;
        
        connection.sendMessage(buffer, bufferSize);
        return true;
    }
    return false;
}

bool PointOfInterestParser::deletePOI(const ClientConnection& connection) {
    if(!createReady && id > 0) {
        int messageSize = 4;
        unsigned char buffer[messageSize];
        
        buffer[messageSize - 1] = 0xff;
        buffer[2] = id;
        buffer[1] = 0x03;
        buffer[0] = 0x03;
        
        connection.sendMessage(buffer, messageSize);
        return true;
    }
    
    return false;
}

bool PointOfInterestParser::getPOIData(const ClientConnection& connection) {
    if(!createReady && id > 0) {
        int messageSize = 4;
        unsigned char buffer[messageSize];
        
        buffer[messageSize - 1] = 0xff;
        buffer[2] = id;
        buffer[1] = 0x01;
        buffer[0] = 0x03;
        
        connection.sendMessage(buffer, messageSize);
        return true;
    }
    return false;
}