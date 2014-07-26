
#include "RouteParser.h"
#include <cstdlib>
#include <cerrno>
#include <cstring>


RouteParser::RouteParser(const char* id) : poiAID(0), poiBID(0), difficulty(0), bidirectional(false), reverse(false) {
    this->id = strtol(id, NULL, 0);
    if(this->id <= 0) {
        throw "Exception: Route attribute value out of range.";
    }
}

RouteParser::RouteParser(const char* poi_a, const char* poi_b, const char* difficulty, const char* bidirectional, const char* reverse) : createReady(true) {
    this->bidirectional = (unsigned char)(strtol(bidirectional, NULL, 0) != 0);
    this->reverse = (unsigned char)(strtol(reverse, NULL, 0) != 0);
    checkValueRanges(strtol(poi_a, NULL, 0), strtol(poi_b, NULL, 0), strtol(difficulty, NULL, 0));
}

void RouteParser::checkValueRanges(int a, int b, int d) {
    if(a < 1 || a > 255 || b > 255 || b < 1 || d < 1 || d > 100) {
        throw "Exception: Route attribute value out of range.";
    }
    
    this->poiAID = a;
    this->poiBID = b;
    this->difficulty = d;
}

bool RouteParser::isNew() {
    return createReady;
}

bool RouteParser::createRoute(const ClientConnection& connection) {
    if(createReady) {
        int bufferSize = 8;
        unsigned char buffer[bufferSize];

        buffer[bufferSize - 1] = 0xff;
        buffer[bufferSize - 2] = reverse;
        buffer[bufferSize - 3] = bidirectional;
        buffer[bufferSize - 4] = difficulty;
        buffer[bufferSize - 5] = poiBID;
        buffer[bufferSize - 6] = poiAID;
        buffer[bufferSize - 7] = 0x06;
        buffer[bufferSize - 8] = 0x03;
        
        connection.sendMessage(buffer, bufferSize);
        return true;
    }
    return false;
}

bool RouteParser::deleteRoute(const ClientConnection& connection) {
    if(!createReady && id > 0) {
        int messageSize = 4;
        unsigned char buffer[messageSize];
        
        buffer[messageSize - 1] = 0xff;
        buffer[2] = id;
        buffer[1] = 0x07;
        buffer[0] = 0x03;
        
        connection.sendMessage(buffer, messageSize);
        return true;
    }
    
    return false;
}

bool RouteParser::getRoute(const ClientConnection& connection) {
    if(!createReady && id > 0) {
        int messageSize = 4;
        unsigned char buffer[messageSize];
        
        buffer[messageSize - 1] = 0xff;
        buffer[2] = id;
        buffer[1] = 0x05;
        buffer[0] = 0x03;
        
        connection.sendMessage(buffer, messageSize);
        return true;
    }
    
    return false;
}
