/* 
 * File:   RouteClient.h
 * Author: toddgreener
 *
 * Created on July 26, 2014, 11:27 AM
 */

#ifndef ROUTECLIENT_H
#define	ROUTECLIENT_H

#include "ClientConnection.h"
#include "RouteParser.h"

class RouteClient {
private:
    const ClientConnection& cc;
    void printNewRouteMessage();
    void printRouteDeletedMessage();
    void printRouteData();
    
public:
    RouteClient(const ClientConnection& con);
    
    void addRoute(RouteParser& rp);
    void deleteRoute(RouteParser& rp);
    void printRoute(RouteParser& rp);
    void printAllRoutes();
};

#endif	/* ROUTECLIENT_H */

