/* 
 * File:   ClientConnection.h
 * Author: toddgreener
 *
 * Created on June 5, 2014, 4:57 PM
 */

#ifndef CLIENTCONNECTION_H
#define	CLIENTCONNECTION_H

#include <stdbool.h>

class ClientConnection {
private:
    int socketHandle;
    
    int setAddress(struct sockaddr_un *address, const char* file);
    
public:
    ClientConnection();
    ~ClientConnection();
    
    bool connect(const char* socketFilePath);
    bool close();
    
    bool sendMessage(const char* message) const;
    bool sendMessage(const unsigned char* message, int length) const;
    const char* receiveMessage(char* buffer, int bufferLength) const;
};

#endif	/* CLIENTCONNECTION_H */

