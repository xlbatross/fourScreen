#include "response.h"

Response::Response()
{

}

const int Response::ResponseType()
{
    return responseType;
}

const vector<vector<char> > &Response::DataBytesList()
{
    return dataBytesList;
}

// tcp start
ResponseTCP::ResponseTCP(const char * rawData)
{
    int pointer = 0;
    int headerSize = 0;
    int dataLength = 0;
    vector<int> dataLengthList;

    // 처음 4바이트는 헤더의 길이
    memcpy(&headerSize, rawData + pointer, sizeof(int));
    pointer += sizeof(int);

    // 다음 4바이트는 응답 타입
    memcpy(&responseType, rawData + pointer, sizeof(int));
    pointer += sizeof(int);

    // 다음 4바이트부터 헤더 끝까지는 의미있는 데이터 하나의 길이값들
    for (int i = 0; i < headerSize - sizeof(int); i += sizeof(int))
    {
        memcpy(&dataLength, rawData + pointer, sizeof(int));
        dataLengthList.push_back(dataLength);
        pointer += sizeof(int);
    }

    if (dataLengthList.size() > 0)
    {
        // 남은 로우데이터를 의미있는 데이터들로 변환
        for (int i = 0; i < dataLengthList.size(); i++)
        {
            vector<char> dataBytes(dataLengthList[i]);
            std::copy(rawData + pointer, rawData + pointer + dataLengthList[i], dataBytes.begin());
            dataBytesList.push_back(dataBytes);
            pointer += dataLengthList[i];
        }
    }
}

ResChat::ResChat()
{
    msg = "";
}

ResChat::ResChat(ResponseTCP *restcp)
{
    msg.append(restcp->DataBytesList()[0].data(), restcp->DataBytesList()[0].size());
}

const string ResChat::Msg()
{
    return msg;
}
// tcp end

// udp start
ResponseUDP::ResponseUDP()
{

}

ResponseUDP::ResponseUDP(const char *rawData, const int totalDataSize)
{
    int pointer = 0;

    memcpy(&responseType, rawData + pointer, sizeof(int));
    pointer += sizeof(int);

    memcpy(&seqNum, rawData + pointer, sizeof(int));
    pointer += sizeof(int);

    vector<char> dataBytes(totalDataSize - pointer);
    std::copy(rawData + pointer, rawData + totalDataSize, dataBytes.begin());
    dataBytesList.push_back(dataBytes);
}

const int ResponseUDP::SeqNum()
{
    return seqNum;
}
// udp end



