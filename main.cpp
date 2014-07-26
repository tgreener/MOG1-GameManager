/* 
 * File:   main.cpp
 * Author: toddgreener
 *
 * Created on June 5, 2014, 1:24 PM
 */

#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include <string>

#include "ClientConnection.h"
#include "PointOfInterestParser.h"
#include "POIClient.h"
#include "RouteParser.h"
#include "RouteClient.h"

using namespace std;

void poiArgumentInterpreter(int argc, char** argv, const ClientConnection& cc) {
    POIClient pc(cc);
    if(strcmp(argv[2], "-add") == 0 && argc >= 7) {
        try {
            PointOfInterestParser poip(argv[3], argv[4], argv[5], argv[6]);
            pc.addPointOfInterest(poip);
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else if(strcmp(argv[2], "-del") == 0 && argc >= 4) {
        try {
            PointOfInterestParser poip(argv[3]);
            pc.deletePointOfInterest(poip);
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else if(strcmp(argv[2], "-get") == 0 && argc >= 4) {
        try {
            PointOfInterestParser poip(argv[3]);
            pc.printPointOfInterest(poip);
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else if(strcmp(argv[2], "-all") == 0 && argc >= 3) {
        try {
            pc.printAllPointOfInterest();
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else {
        printf("USAGE: -poi\n");
    }
}

void routeArgumentInterpreter(int argc, char** argv, const ClientConnection& cc) {
    RouteClient rc(cc);
    if(strcmp(argv[2], "-add") == 0 && argc >= 8) {
        try {
            RouteParser routep(argv[3], argv[4], argv[5], argv[6], argv[7]);
            rc.addRoute(routep);
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else if(strcmp(argv[2], "-del") == 0 && argc >= 4) {
        try {
            RouteParser routep(argv[3]);
            rc.deleteRoute(routep);
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else if(strcmp(argv[2], "-get") == 0 && argc >= 4) {
        try {
            RouteParser routep(argv[3]);
            rc.printRoute(routep);
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else if(strcmp(argv[2], "-all") == 0 && argc >= 3) {
        try {
            rc.printAllRoutes();
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    }
    else {
        printf("USAGE: -route\n");
    }
}

int main(int argc, char** argv) {
    ClientConnection cc;
    cc.connect("../GameDataServer/connection");
    
    if(argc < 3) {
        printf("USAGE: Need more useful arguments.\n");
        return 0;
    } 
    
    if(strcmp(argv[1], "-poi") == 0) {
        poiArgumentInterpreter(argc, argv, cc);
    }
    else if(strcmp(argv[1], "-route") == 0) {
        routeArgumentInterpreter(argc, argv, cc);
    }
    
    cc.close();
    
    return 0;
}

