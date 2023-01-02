#ifndef RESPONSE_H
#define RESPONSE_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Response
{
public:
    enum Type {Chat, FirstImage, SecondImage, ThirdImage, FourthImage};

    Response();

    const int ResponseType();
    const vector<vector<char>> & DataBytesList();

protected:
    int responseType;
    vector<vector<char>> dataBytesList;
};

// tcp start
class ResponseTCP : public Response
{
public:
    ResponseTCP(const char * rawData);
};


class ResChat
{
public:
    ResChat();
    ResChat(ResponseTCP * restcp);

    const string Msg();

private:
    string msg;
};
// tcp end

// udp start
class ResponseUDP : public Response
{
public:
    ResponseUDP();
    ResponseUDP(const char * rawData, const int totalDataSize);

    const int SeqNum();

protected:
    int seqNum = -1;
};
//udp end

#endif // RESPONSE_H
