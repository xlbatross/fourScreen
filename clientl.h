#ifndef CLIENTL_H
#define CLIENTL_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "request.h"
#include "response.h"

class ClientL
{
public:
    explicit ClientL(const char * servIp, int port);
    ~ClientL();

    string sockIp();
    int sockPort();

    virtual bool connectServer();
    virtual int receiveBytes(char * & rawData) = 0;
    virtual bool receiveData(Response * & res) = 0;
    virtual int sendBytes(const char * headerBytes, const int headerSize, const char * dataBytes, const int dataSize) = 0;
    virtual bool sendData(Request & req);

protected:
    int sock;
    struct sockaddr_in myAdr;
    struct sockaddr_in servAdr;

    bool setSockInfo();
};

class ClientLTCP : public ClientL
{
public:
    explicit ClientLTCP(const char * servIp, int port);

    int receiveBytes(char * & rawData) override;
    bool receiveData(Response * & res) override;
    int sendBytes(const char * headerBytes, const int headerSize, const char * dataBytes, const int dataSize) override;
};

class ClientLUDP : public ClientL
{
public:
    explicit ClientLUDP(const char * servIp, int port);

    int receiveBytes(char * & rawData) override;
    bool receiveData(Response * & res) override;
    int sendBytes(const char * headerBytes, const int headerSize, const char * dataBytes, const int dataSize) override;
};


#endif // CLIENTL_H
