#ifndef RESPONSE_H
#define RESPONSE_H

#include <vector>

using std::vector;

class Response
{
public:
    Response(const char * rawData);

protected:
    int responseType;
    char ** dataBytesList;
    vector<int> dataLengthList;
};

#endif // RESPONSE_H
