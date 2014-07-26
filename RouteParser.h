/* 
 * File:   RouteParser.h
 * Author: toddgreener
 *
 * Created on July 26, 2014, 10:49 AM
 */

#ifndef ROUTEPARSER_H
#define	ROUTEPARSER_H

#include "ClientConnection.h"

class RouteParser {
private:
    
    unsigned char id;
    
    unsigned char poiAID;
    unsigned char poiBID;
    unsigned char difficulty;
    bool bidirectional;
    bool reverse;
    
    bool createReady;
    
    void checkValueRanges(int a, int b, int d);
    
public:
    RouteParser(const char* id);
    RouteParser(const char* poi_a, const char* poi_b, const char* difficulty, const char* bidirectional, const char* reverse);
    
    bool isNew();
    bool createRoute(const ClientConnection& connection);
    bool deleteRoute(const ClientConnection& connection);
    bool getRoute(const ClientConnection& connection);
};

#endif	/* ROUTEPARSER_H */

