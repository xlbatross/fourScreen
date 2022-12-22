#include "clientl.h"

ClientL::ClientL(const char * servIp, int port)
{
    tcpSock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&servTcpAdr, 0, sizeof(servTcpAdr));
    servTcpAdr.sin_family = AF_INET;
    servTcpAdr.sin_addr.s_addr = inet_addr(servIp);
    servTcpAdr.sin_port = htons(port);


    udpSock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&servTcpAdr, 0, sizeof(servTcpAdr));
    servUdpAdr.sin_family = AF_INET;
    servUdpAdr.sin_addr.s_addr = inet_addr(servIp);
    servUdpAdr.sin_port = htons(port + 1);
}

bool ClientL::connectServer()
{
    return (connect(tcpSock, (struct sockaddr*)&servTcpAdr, sizeof(servTcpAdr)) != -1
            && connect(udpSock, (struct sockaddr*)&servUdpAdr, sizeof(servUdpAdr)) != -1);
}

// 4바이트를 먼저 읽어, 총 데이터의 길이를 파악한다
// 총 데이터 길이는 헤더의 길이(4바이트) + 헤더 + 데이터의 길이(4바이트) + 실제 데이터
// 헤더는 요청 응답 타입(4바이트) + 실제 데이터 하나의 길이값(4바이트) * 헤더의 길이 - 1
int ClientL::receiveData(int sock, char * & rawData)
{
    int totalDataSize = -1;
    char dataSizeBuffer[4];

    int readBytes = read(sock, dataSizeBuffer, 4);
    if (readBytes != -1)
    {
        int dataSize = *((int *)dataSizeBuffer);
        rawData = new char[dataSize];
        for (int i = 0; i < dataSize; i += 1024)
        {
            readBytes = read(sock, rawData + i, (i + 1024 < dataSize) ? 1024 : dataSize - i);
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

Response *ClientL::receive(int sock)
{
    char * rawData = NULL;
    int totalDataSize = receiveData(sock, rawData);

    if (rawData == NULL)
        return NULL;
    else
    {

    }
}




