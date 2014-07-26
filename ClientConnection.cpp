
#include "ClientConnection.h"
#include <cstdlib>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

ClientConnection::ClientConnection() : socketHandle(-1) {}

ClientConnection::~ClientConnection() {}
    
int ClientConnection::setAddress(struct sockaddr_un *address, const char* filename) {
    memset(address, 0, sizeof(*address));
    
    address->sun_family = PF_LOCAL;
    strcpy(address->sun_path, filename);
    
    return 1;
}

bool ClientConnection::connect(const char* socketFilePath) {
    struct sockaddr_un address;
    socketHandle = socket(PF_LOCAL, SOCK_STREAM, 0);
    
    setAddress(&address, socketFilePath);
    if(::connect(socketHandle, (struct sockaddr*)&address, sizeof(address)) < 0) {
        return false;
    }
    
    return true;
}

bool ClientConnection::close() {
    ::close(socketHandle);
    return false;
}
    
bool ClientConnection::sendMessage(const char* message) const {
    return sendMessage((const unsigned char*)message, strlen(message));
}

bool ClientConnection::sendMessage(const unsigned char* message, int length) const {
    if(socketHandle >= 0) {
        return send(socketHandle, message, length, 0) >= 0;
    }
    return false;
}

const char* ClientConnection::receiveMessage(char* buffer, int bufferSize) const {
    if(socketHandle >= 0) {
        recv(socketHandle, buffer, bufferSize, 0);
        return buffer;
    }
    return NULL;
}
