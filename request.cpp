#include "request.h"

Request::Request()
{

}

Request::~Request()
{
    if (headerBytes != NULL)
        delete [] headerBytes;
    if (dataBytes != NULL)
        delete [] dataBytes;
}

const int Request::HeaderSize()
{
    return headerSize;
}

const char *Request::HeaderBytes()
{
    return headerBytes;
}

const int Request::DataSize()
{
    return dataSize;
}

const char *Request::DataBytes()
{
    return dataBytes;
}

// TCP start
RequestTCP::RequestTCP()
{

}

void RequestTCP::packaging(const int type)
{
    vector<int> headerVector;
    int realHeaderSize = 0;
    int dataPointer = 0;

    // 헤더
    // 헤더의 길이(4바이트 정수형, 이 길이값은 이 뒤에 오는 데이터의 길이를 의미한다.)
    //  + 요청 타입(4바이트 정수형) + 데이터 하나의 바이트 길이(4바이트 정수형) * ((헤더의 길이 / 4바이트) - 1)
    headerVector.push_back(type);
    for (int i = 0; i < dataBytesList.size(); i++)
    {
        // 데이터 하나의 바이트 길이 (4바이트 정수형)
        headerVector.push_back(dataBytesList[i].size());
        dataSize += dataBytesList[i].size();
    }

    realHeaderSize = headerVector.size() * sizeof(int);
    headerSize = sizeof(int) + realHeaderSize;

    headerBytes = new char[headerSize]();
    memcpy(headerBytes, &realHeaderSize, sizeof(int));
    memcpy(headerBytes + sizeof(int), (char *)headerVector.data(), realHeaderSize);

    // 데이터
    dataBytes = new char[dataSize];
    for (int i = 0; i < dataBytesList.size(); i++)
    {
        memcpy(dataBytes + dataPointer, dataBytesList[i].data(), dataBytesList[i].size());
        dataPointer += dataBytesList[i].size();
    }
}

ReqUDPConnect::ReqUDPConnect(const std::string &servIp, const int port)
{
    vector<char> servIpvector(servIp.length());
    std::copy(servIp.begin(), servIp.end(), servIpvector.begin());
    dataBytesList.push_back(servIpvector);

    vector<char> portvector(4);
    std::copy((char *)&port, (char *)&port + sizeof(int), portvector.begin());
    dataBytesList.push_back(portvector);

    packaging(RequestTCP::UDPConnect);
}

ReqChat::ReqChat(const std::string &msg)
{
    vector<char> msgvector(msg.length());
    std::copy(msg.begin(), msg.end(), msgvector.begin());
    dataBytesList.push_back(msgvector);

    packaging(RequestTCP::Chat);
}
// tcp End

// UDP start
RequestUDP::RequestUDP()
{

}

void RequestUDP::packaging(const int type)
{
    int dataPointer = 0;

    // 헤더
    // 요청 타입(4바이트 정수형)
    // 데이터를 보낼 때 순서 번호(4바이트 정수형)을 붙여서 보낼 예정
    headerSize = sizeof(int);
    headerBytes = new char[headerSize];
    memcpy(headerBytes, (char *)&type, headerSize);

    for (int i = 0; i < dataBytesList.size(); i++)
        dataSize += dataBytesList[i].size();

    // 데이터
    dataBytes = new char[dataSize];
    for (int i = 0; i < dataBytesList.size(); i++)
    {
        memcpy(dataBytes + dataPointer, dataBytesList[i].data(), dataBytesList[i].size());
        dataPointer += dataBytesList[i].size();
    }
}

ReqImage::ReqImage(const cv::Mat &img)
{
    vector<char> imgvector(img.total() * img.channels());
    std::copy((char *)img.data, (char *)img.data + (img.total() * img.channels()), imgvector.begin());
    dataBytesList.push_back(imgvector);

    packaging(Request::Image);
}
// UDP end
