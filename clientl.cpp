#include "clientl.h"

ClientL::ClientL(const char * servIp, int port)
{
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(servIp);
    servAdr.sin_port = htons(port);
}

ClientL::~ClientL()
{
    close(sock);
}

string ClientL::sockIp()
{
    char myIP[16] = {};
    inet_ntop(AF_INET, &myAdr.sin_addr, myIP, sizeof(myIP));
    return string(myIP, sizeof(myIP));
}

int ClientL::sockPort()
{
    return ntohs(myAdr.sin_port);
}

bool ClientL::setSockInfo()
{
    socklen_t addr_len = sizeof(myAdr);
    memset(&myAdr, 0, sizeof(myAdr));
    return (getsockname(sock, (struct sockaddr*)&myAdr, &addr_len) != -1);
}

bool ClientL::connectServer()
{
    return (connect(sock, (struct sockaddr*)&servAdr, sizeof(servAdr)) != -1 && setSockInfo());
}

bool ClientL::sendData(Request &reqtcp)
{
    int totalDataSize = sendBytes(reqtcp.HeaderBytes(), reqtcp.HeaderSize(), reqtcp.DataBytes(), reqtcp.DataSize());

    return (totalDataSize == -1);
}

// TCP

ClientLTCP::ClientLTCP(const char * servIp, int port)
    : ClientL(servIp, port)
{
    sock = socket(PF_INET, SOCK_STREAM, 0);
}

int ClientLTCP::receiveBytes(char * & rawData)
{
    int totalDataSize = -1;
    char dataSizeBuffer[4];
    // 4바이트를 먼저 읽어, 총 데이터의 길이를 파악한다
    // 총 데이터 길이는 헤더의 길이(4바이트) + 헤더 + 실제 데이터
    // 헤더는 요청 응답 타입(4바이트) + 실제 데이터 하나의 길이값(4바이트) * 헤더의 길이 - 1
    int readBytes = read(sock, dataSizeBuffer, 4);
    int dataSize = *((int *)dataSizeBuffer);
    if (readBytes != -1)
    {
        rawData = new char[dataSize];
        for (int i = 0; i < dataSize; i += 1024)
        {
            int packetSize = (i + 1024 < dataSize) ? 1024 : dataSize - i;
            readBytes = read(sock, rawData + i, packetSize);
            if (readBytes == -1)
                return -1;
            totalDataSize += readBytes;
        }
    }
    return totalDataSize;
}

int ClientLTCP::sendBytes(const char *headerBytes, const int headerSize, const char *dataBytes, const int dataSize)
{
    int totalSendSize = -1;
    int totalSize = headerSize + dataSize;
    int sendSize = -1;

    if ((sendSize = write(sock, (char *)&totalSize, sizeof(int))) == -1)
        return -1;
    totalSendSize += sendSize;

    if ((sendSize = write(sock, headerBytes, headerSize)) == -1)
        return -1;
    totalSendSize += sendSize;

    if ((sendSize = write(sock, dataBytes, dataSize)) == -1)
        return -1;
    totalSendSize += sendSize;

    return totalSendSize;
}

bool ClientLTCP::receiveData(Response * & res)
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

ClientLUDP::ClientLUDP(const char * servIp, int port)
    : ClientL(servIp, port)
{
    sock = socket(PF_INET, SOCK_DGRAM, 0);
}

int ClientLUDP::receiveBytes(char * & rawData)
{
    int readBytes = -1;
    int packetSize = sizeof(int) + sizeof(int) + 1024;

    rawData = new char[packetSize];

    readBytes = read(sock, rawData, packetSize);

    return readBytes;
}

bool ClientLUDP::receiveData(Response * & res)
{
    char * rawData = NULL;
    bool isSuccess = false;
    int totalDataSize = receiveBytes(rawData);

    if (totalDataSize != -1)
    {
        isSuccess = true;
        res = (Response *)(new ResponseUDP(rawData));
    }
    if (rawData != NULL)
        delete [] rawData;
    return isSuccess;
}

int ClientLUDP::sendBytes(const char *headerBytes, const int headerSize, const char *dataBytes, const int dataSize)
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

        if (write(sock, packet, packetSize) == -1)
            dataCount = -1;

        delete [] packet;

        if (dataCount == -1)
            break;
    }
    return dataCount;
}
