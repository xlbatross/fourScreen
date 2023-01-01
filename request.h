#ifndef REQUEST_H
#define REQUEST_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using std::vector;
using std::string;

class Request
{
public:
    enum Type {UDPConnect, Chat, Image};
    // UDPConnect = 0 tcp
    // Chat = 1 tcp
    // Image = 2 udp
    Request();
    ~Request();

    virtual void packaging(const int type) = 0;

    const int HeaderSize();
    const char * HeaderBytes();

    const int DataSize();
    const char * DataBytes();

protected:
    vector<vector<char>> dataBytesList;
    int headerSize = 0;
    char * headerBytes = NULL;
    int dataSize = 0;
    char * dataBytes = NULL;
};

// TCP Start
class RequestTCP : public Request
{
public:
    RequestTCP();
    void packaging(const int type) override;
};

class ReqUDPConnect: public RequestTCP
{
public:
    ReqUDPConnect(const string & servIp, const int port);
};

class ReqChat: public RequestTCP
{
public:
    ReqChat(const string & msg);
};
// TCP end

// UDP start
class RequestUDP : public Request
{
public:
    RequestUDP();

    void packaging(const int type) override;
};

class ReqImage : public RequestUDP
{
public:
    ReqImage(const cv::Mat & img);
};

// UDP end

#endif // REQUEST_H
