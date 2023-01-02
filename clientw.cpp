#include "clientw.h"

ClientW::ClientW(const char * servIp, int port)
{
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(servIp);
    servAdr.sin_port = htons(port);
}

ClientW::~ClientW()
{
    closesocket(sock);
    WSACleanup();
}

string ClientW::sockIp()
{
    string stringIP;
    char charIP[50] = {};
    DWORD ip_len = sizeof(charIP);
    WSAAddressToStringA((SOCKADDR *)&myAdr, sizeof(myAdr), NULL, charIP, &ip_len);
    for (int i = 0; i < sizeof(charIP); i++)
    {
        if (charIP[i] == ':')
            break;
        stringIP += charIP[i];
    }
    return stringIP;
}

int ClientW::sockPort()
{
    return ntohs(myAdr.sin_port);
}

bool ClientW::setSockInfo()
{
    int addr_len = sizeof(myAdr);
    memset(&myAdr, 0, sizeof(myAdr));
    return (getsockname(sock, (SOCKADDR *)&myAdr, &addr_len) != SOCKET_ERROR);
}

bool ClientW::connectServer()
{
    return (connect(sock, (SOCKADDR *)&servAdr, sizeof(servAdr)) != SOCKET_ERROR && setSockInfo());
}

bool ClientW::sendData(Request &reqtcp)
{
    int totalDataSize = sendBytes(reqtcp.HeaderBytes(), reqtcp.HeaderSize(), reqtcp.DataBytes(), reqtcp.DataSize());

    return (totalDataSize != -1);
}

// TCP

ClientWTCP::ClientWTCP(const char * servIp, int port)
    : ClientW(servIp, port)
{
    sock = socket(PF_INET, SOCK_STREAM, 0);
}

int ClientWTCP::receiveBytes(char * & rawData)
{
    int totalDataSize = -1;
    char dataSizeBuffer[4];
    // 4바이트를 먼저 읽어, 총 데이터의 길이를 파악한다
    // 총 데이터 길이는 헤더의 길이(4바이트) + 헤더 + 실제 데이터
    // 헤더는 요청 응답 타입(4바이트) + 실제 데이터 하나의 길이값(4바이트) * 헤더의 길이 - 1
    int readBytes = recv(sock, dataSizeBuffer, 4, 0);
    int dataSize = *((int *)dataSizeBuffer);
    int packetSize = 0;
    if (readBytes != -1)
    {
        rawData = new char[dataSize];
        for (int i = 0; i < dataSize; i += 1024)
        {
            packetSize = (i + 1024 < dataSize) ? 1024 : dataSize - i;
            readBytes = recv(sock, rawData + i, packetSize, 0);
            if (readBytes == SOCKET_ERROR)
                return -1;
            totalDataSize += readBytes;
        }
    }
    return totalDataSize + 1;
}

int ClientWTCP::sendBytes(const char *headerBytes, const int headerSize, const char *dataBytes, const int dataSize)
{
    int totalSendSize = -1;
    int totalSize = headerSize + dataSize;
    int sendSize = -1;

    if ((sendSize = send(sock, (char *)&totalSize, sizeof(int), 0)) == SOCKET_ERROR)
        return -1;
    totalSendSize = sendSize;

    if ((sendSize = send(sock, headerBytes, headerSize, 0)) == SOCKET_ERROR)
        return -1;
    totalSendSize += sendSize;

    if ((sendSize = send(sock, dataBytes, dataSize, 0)) == SOCKET_ERROR)
        return -1;
    totalSendSize += sendSize;

    return totalSendSize;
}

bool ClientWTCP::receiveData(Response * & res)
{
    char * rawData = NULL;
    bool isSuccess = false;
    int totalDataSize = receiveBytes(rawData);

    if (totalDataSize != -1)
    {
        isSuccess = true;
        res = (Response *)(new ResponseTCP(rawData));
    }
    if (rawData != NULL)
        delete [] rawData;
    return isSuccess;
}

// UDP

ClientWUDP::ClientWUDP(const char * servIp, int port)
    : ClientW(servIp, port)
{
    sock = socket(PF_INET, SOCK_DGRAM, 0);
}

int ClientWUDP::receiveBytes(char * & rawData)
{
    int readBytes = -1;
    int packetSize = sizeof(int) + sizeof(int) + 1024;

    rawData = new char[packetSize];

    readBytes = recv(sock, rawData, packetSize, 0);

    return readBytes;
}

bool ClientWUDP::receiveData(Response * & res)
{
    char * rawData = NULL;
    bool isSuccess = false;
    int totalDataSize = receiveBytes(rawData);

    if (totalDataSize != -1)
    {
        isSuccess = true;
        res = (Response *)(new ResponseUDP(rawData, totalDataSize));
    }
    if (rawData != NULL)
        delete [] rawData;
    return isSuccess;
}

int ClientWUDP::sendBytes(const char *headerBytes, const int headerSize, const char *dataBytes, const int dataSize)
{
    int dataCount = dataSize / 1024 + ((dataSize % 1024 == 0) ? 0 : 1);
    int packetDataSize = -1;
    int packetSize = -1;

    for (int i = 0; i < dataCount; i++)
    {
        packetDataSize = (i < dataSize / 1024) ? 1024 : dataSize % 1024;
        packetSize = headerSize + sizeof(int) + packetDataSize;

        char * packet = new char[packetSize];

        memcpy(packet, headerBytes, headerSize);
        memcpy(packet + headerSize, (char *)&i, sizeof(int));
        memcpy(packet + headerSize + sizeof(int) , dataBytes + (1024 * i), packetDataSize);

        if (send(sock, packet, packetSize, 0) == SOCKET_ERROR)
            dataCount = -1;

        delete [] packet;

        if (dataCount == -1)
            break;
    }
    return dataCount;
}
