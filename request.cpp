#include "request.h"

Request::Request()
{

}


Request::~Request()
{
    if (totalBytes != NULL)
        delete [] totalBytes;
}

void Request::packaging(const int type, const std::vector<std::vector<char>> & dataBytesList)
{
    int pointer = 0;
    int dataSize = 0;
    int headerSize = 0;
    vector<int> header;

    header.push_back(type);

    for (int i = 0; i < dataBytesList.size(); i++)
    {
        header.push_back(dataBytesList[i].size());
        dataSize += dataBytesList[i].size();
    }

    headerSize = header.size() * sizeof(int);
    totalSize = sizeof(int) + headerSize + dataSize;
    totalBytes = new char[totalSize];

    memcpy(totalBytes + pointer, &headerSize, sizeof(int));
    pointer += sizeof(int);

    memcpy(totalBytes + pointer, (char *)header.data(), headerSize);
    pointer += headerSize;

    for (int i = 0; i < dataBytesList.size(); i++)
    {
        memcpy(totalBytes + pointer, dataBytesList[i].data(), dataBytesList[i].size());
        pointer += dataBytesList[i].size();
    }
}

const int Request::TotalSize()
{
    return totalSize;
}

const char *Request::TotalBytes()
{
    return totalBytes;
}

ReqUDPConnect::ReqUDPConnect()
{
    vector<vector<char>> dataBytesList;
    packaging(Request::UDPConnect, dataBytesList);
}

ReqChat::ReqChat(const std::string &msg)
{
    vector<vector<char>> dataBytesList;

    vector<char> msgvector;
    std::copy(msg.begin(), msg.end(), msgvector.begin());

    dataBytesList.push_back(msgvector);

    packaging(Request::Chat, dataBytesList);
}
