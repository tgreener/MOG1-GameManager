/* 
 * File:   PointOfInterestParser.h
 * Author: toddgreener
 *
 * Created on June 8, 2014, 2:02 PM
 */

#ifndef POINTOFINTERESTPARSER_H
#define	POINTOFINTERESTPARSER_H

#include "ClientConnection.h"
#include <string>

class PointOfInterestParser {
private:
    unsigned char soil;
    unsigned char stone;
    unsigned char wilderness;
    
    unsigned char id;
    
    std::string name;
    
    bool createReady;
    
    unsigned char valFromString(const char* str);
    void setName(const char* str);
    
public:
    PointOfInterestParser(const char* n, const char* sl, const char* st, const char* wld);
    PointOfInterestParser(const char* id);
    ~PointOfInterestParser();
    
    bool isNew();
    bool create(const ClientConnection& connection);
    bool deletePOI(const ClientConnection& connection);
    bool getPOIData(const ClientConnection& connection);
};

#endif	/* POINTOFINTERESTPARSER_H */

