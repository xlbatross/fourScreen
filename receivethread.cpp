#include "receivethread.h"

ReceiveThread::ReceiveThread(
        ClientW * client,
        QObject *parent
)
    : client(client)
    , QThread{parent}
{

}

void ReceiveThread::start()
{
    isRunning = true;
    QThread::start();
}

void ReceiveThread::run()
{
    while (isRunning)
    {
        Response * res = client->receiveUDP();

        if (res == NULL)
        {
            isRunning = false;
            break;
        }

        switch(res->ResponseType())
        {
        case Response::Chat:
        {
            ResChat resChat(res);
            emit responseChat(resChat);
        } break;
        }
    }
}
