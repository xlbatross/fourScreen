#ifndef CLIENTL_H
#define CLIENTL_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "response.h"

class ClientL
{
public:
    explicit ClientL(const char * servIp = "127.0.0.1", int port = 2500);
    ~ClientL();

    bool connectServer();
    int receiveData(int sock, char * & rawData);
    Response *receiveTCP();
    Response *receiveUDP();

private:
    int tcpSock;
    int udpSock;

    struct sockaddr_in servTcpAdr;
    struct sockaddr_in servUdpAdr;
    struct sockaddr_in fromUdpAdr;

};

#endif // CLIENTL_H
