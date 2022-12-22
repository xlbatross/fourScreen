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

    // 다음 4바이트부터 헤더 끝까지는 데이터 하나의 길이
    int dataLength;
    for (int i = pointer; i < headerSize; i += sizeof(int))
    {
        memcpy(&dataLength, rawData + pointer, sizeof(int));
        dataLengthList.push_back(dataLength);
        pointer += i;
    }

    // 헤더가 끝난 다음 4바이트는 실제 데이터의 총 길이
    int dataSize;
    memcpy(&dataSize, rawData + pointer, sizeof(int));
    pointer += sizeof(int);

    dataBytesList = new char * [dataLengthList.size()];
    for (int i = 0; i < dataLengthList.size(); i++)
    {

    }
}
