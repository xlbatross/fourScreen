#ifndef REQUEST_H
#define REQUEST_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Request
{
public:
    enum Type {UDPConnect, Chat};
    Request();
    ~Request();

    void packaging(const int type, const vector<vector<char>> & dataBytes);

    const int TotalSize();
    const char * TotalBytes();

protected:
    int totalSize = 0;
    char * totalBytes = NULL;
};

class ReqUDPConnect: public Request
{
public:
    ReqUDPConnect();
};

class ReqChat: public Request
{
public:
    ReqChat(const string & msg);
};

#endif // REQUEST_H
