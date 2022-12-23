#ifndef RESPONSE_H
#define RESPONSE_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Response
{
public:
    enum Type {Chat = 1};
    Response(const char * rawData);
    ~Response();

    const int ResponseType();
    const char * const * DataBytesList();
    const vector<int> & DataLengthList();

private:
    int responseType;
    char ** dataBytesList = NULL;
    vector<int> dataLengthList;
};

class ResChat
{
public:
    ResChat();
    ResChat(Response * res);

    const string Msg();

private:
    string msg;
};

#endif // RESPONSE_H
