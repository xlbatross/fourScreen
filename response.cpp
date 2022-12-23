#include "response.h"

Response::Response(const char * rawData)
{
    int pointer = 0;

    // 처음 4바이트는 헤더의 길이
    int headerSize;
    memcpy(&headerSize, rawData + pointer, sizeof(int));
    pointer += sizeof(int);

    // 다음 4바이트는 응답 타입
    memcpy(&responseType, rawData + pointer, sizeof(int));
    pointer += sizeof(int);

    // 다음 4바이트부터 헤더 끝까지는 의미있는 데이터 하나의 길이값들
    int dataLength;
    for (int i = 0; i < headerSize - sizeof(int); i += sizeof(int))
    {
        memcpy(&dataLength, rawData + pointer, sizeof(int));
        dataLengthList.push_back(dataLength);
        pointer += sizeof(int);
    }

    if (dataLengthList.size() > 0)
    {
        // 남은 로우데이터를 의미있는 데이터들로 변환
        dataBytesList = new char * [dataLengthList.size()];
        for (int i = 0; i < dataLengthList.size(); i++)
        {
            dataBytesList[i] = new char[dataLengthList[i]];
            memcpy(dataBytesList[i], rawData + pointer, dataLengthList[i]);
            pointer += dataLengthList[i];
        }
    }
}

Response::~Response()
{
    for (int i = 0; i < dataLengthList.size(); i++)
    {
        delete [] dataBytesList[i];
    }
    if (dataLengthList.size() > 0)
        delete [] dataBytesList;
}

const int Response::ResponseType()
{
    return responseType;
}

const char * const *Response::DataBytesList()
{
    return dataBytesList;
}

const vector<int> &Response::DataLengthList()
{
    return dataLengthList;
}


ResChat::ResChat()
{
    msg = "";
}

ResChat::ResChat(Response *res)
{
    msg.append(res->DataBytesList()[0], res->DataLengthList()[0]);
    delete res;
}

const string ResChat::Msg()
{
    return msg;
}
