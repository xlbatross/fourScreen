#include "receivethread.h"

ReceiveThread::ReceiveThread(
//        ClientW * client,
        ClientL * client,
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
        Response * res = NULL;
        client->receiveData(res);

        if (res == NULL)
        {
            isRunning = false;
            break;
        }

        switch(res->ResponseType())
        {
        case Response::Chat:
        {
            ResChat resChat((ResponseTCP *)res);
            emit responseChat(resChat);
        } break;
        }

        delete res;
    }
}
