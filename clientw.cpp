#include "clientw.h"

ClientW::ClientW(const char * servIp, int port)
{
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    tcpSock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&servTcpAdr, 0, sizeof(servTcpAdr));
    servTcpAdr.sin_family = AF_INET;
    servTcpAdr.sin_addr.s_addr = inet_addr(servIp);
    servTcpAdr.sin_port = htons(port);

    udpSock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&servUdpAdr, 0, sizeof(servUdpAdr));
    servUdpAdr.sin_family = AF_INET;
    servUdpAdr.sin_addr.s_addr = inet_addr(servIp);
    servUdpAdr.sin_port = htons(port + 1);
}

ClientW::~ClientW()
{
    closesocket(tcpSock);
    closesocket(udpSock);
    WSACleanup();
}

bool ClientW::connectServer()
{
    ReqUDPConnect reqUdpConnect;
    return (connect(tcpSock, (SOCKADDR *)&servTcpAdr, sizeof(servTcpAdr)) != SOCKET_ERROR
         && connect(udpSock, (SOCKADDR *)&servUdpAdr, sizeof(servUdpAdr)) != SOCKET_ERROR
         && sendUDP(reqUdpConnect));
}

// 4바이트를 먼저 읽어, 총 데이터의 길이를 파악한다
// 총 데이터 길이는 헤더의 길이(4바이트) + 헤더 + 실제 데이터
// 헤더는 요청 응답 타입(4바이트) + 실제 데이터 하나의 길이값(4바이트) * ((헤더의 길이 / 4바이트) - 1)
int ClientW::receiveBytes(SOCKET sock, char * & rawData)
{
    int totalDataSize = -1;
    char dataSizeBuffer[4];

    int readBytes = recv(sock, dataSizeBuffer, 4, 0);
    if (readBytes != -1)
    {
        int dataSize = *((int *)dataSizeBuffer);
        rawData = new char[dataSize];
        for (int i = 0; i < dataSize; i += 1024)
        {
            readBytes = recv(sock, rawData + i, (i + 1024 < dataSize) ? 1024 : dataSize - i, 0);
            if (readBytes == -1)
            {
                delete [] rawData;
                return -1;
            }
            totalDataSize += readBytes;
        }
    }
    return totalDataSize;
}

Response *ClientW::receiveData(SOCKET sock)
{
    char * rawData = NULL;
    int totalDataSize = receiveBytes(sock, rawData);

    if (totalDataSize == -1 && rawData == NULL)
        return NULL;
    else
    {
        Response * res = new Response(rawData);
        delete [] rawData;
        return res;
    }
}

Response *ClientW::receiveTCP()
{
    return receiveData(tcpSock);
}

Response *ClientW::receiveUDP()
{
    return receiveData(udpSock);
}

int ClientW::sendBytes(SOCKET sock, const char *totalBytes, const int totalSize)
{
    int totalDataSize = -1;
    int writeBytes = send(sock, (char *)&totalSize, sizeof(int), 0);
    if (writeBytes != -1)
    {
        for (int i = 0; i < totalSize; i += 1024)
        {
            writeBytes = send(sock, totalBytes + i, (i + 1024 < totalSize) ? 1024 : totalSize - i, 0);
            if (writeBytes == -1)
            {
                return -1;
            }
            totalDataSize += writeBytes;
        }
    }
    return totalDataSize;
}

bool ClientW::sendData(SOCKET sock, Request &req)
{
    int totalDataSize = sendBytes(sock, req.TotalBytes(), req.TotalSize());

    if (totalDataSize == -1)
        return false;
    else
        return true;
}

bool ClientW::sendTCP(Request &req)
{
    return sendData(tcpSock, req);
}

bool ClientW::sendUDP(Request &req)
{
    return sendData(udpSock, req);
}




