/* 
 * File:   POIClient.h
 * Author: toddgreener
 *
 * Created on June 8, 2014, 11:11 PM
 */

#ifndef POICLIENT_H
#define	POICLIENT_H

#include "PointOfInterestParser.h"
#include "ClientConnection.h"

class POIClient {
private:
    const ClientConnection& cc;
    void printNewPOIMessage();
    void printPOIDeletedMessage();
    void printPOIData();
public:
    POIClient(const ClientConnection& con);
    
    void addPointOfInterest(PointOfInterestParser& poip);
    void deletePointOfInterest(PointOfInterestParser& poip);
    void printPointOfInterest(PointOfInterestParser& poip);
    void printAllPointOfInterest();
};

#endif	/* POICLIENT_H */

