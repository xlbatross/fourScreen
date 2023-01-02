#ifndef CLIENTW_H
#define CLIENTW_H

#include <string.h>
#include <winsock2.h>
#include "request.h"
#include "response.h"

class ClientW
{
public:
    explicit ClientW(const char * servIp, int port);
    ~ClientW();

    string sockIp();
    int sockPort();

    virtual bool connectServer();
    virtual int receiveBytes(char * & rawData) = 0;
    virtual bool receiveData(Response * & res) = 0;
    virtual int sendBytes(const char * headerBytes, const int headerSize, const char * dataBytes, const int dataSize) = 0;
    virtual bool sendData(Request & req);

protected:
    WSADATA wsaData;

    SOCKET sock;
    SOCKADDR_IN myAdr;
    SOCKADDR_IN servAdr;

    bool setSockInfo();
};

class ClientWTCP : public ClientW
{
public:
    explicit ClientWTCP(const char * servIp, int port);

    int receiveBytes(char * & rawData) override;
    bool receiveData(Response * & res) override;
    int sendBytes(const char * headerBytes, const int headerSize, const char * dataBytes, const int dataSize) override;
};

class ClientWUDP : public ClientW
{
public:
    explicit ClientWUDP(const char * servIp, int port);

    int receiveBytes(char * & rawData) override;
    bool receiveData(Response * & res) override;
    int sendBytes(const char * headerBytes, const int headerSize, const char * dataBytes, const int dataSize) override;
};

#endif // CLIENTW_H
