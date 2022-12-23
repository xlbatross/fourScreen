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
    
    int receiveBytes(SOCKET sock, char * & rawData);
    Response *receiveData(SOCKET sock);
    Response *receiveTCP();
    Response *receiveUDP();

    int sendBytes(SOCKET sock, const char * totalBytes, const int totalSize);
    bool sendData(SOCKET sock, Request & req);
    bool sendTCP(Request & req);
    bool sendUDP(Request & req);

private:
    int tcpSock;
    int udpSock;

    struct sockaddr_in servTcpAdr;
    struct sockaddr_in servUdpAdr;
    struct sockaddr_in fromUdpAdr;

};

#endif // CLIENTL_H
