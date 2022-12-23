#ifndef CLIENTW_H
#define CLIENTW_H

#include <string.h>
#include <winsock2.h>
#include "response.h"
#include "request.h"

class ClientW
{
public:
    explicit ClientW(const char * servIp = "127.0.0.1", int port = 2500);
    ~ClientW();

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
    WSADATA wsaData;

    SOCKET tcpSock;
    SOCKET udpSock;

    SOCKADDR_IN servTcpAdr;
    SOCKADDR_IN servUdpAdr;
    SOCKADDR_IN fromUdpAdr;

};

#endif // CLIENTW_H
