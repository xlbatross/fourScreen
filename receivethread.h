#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
//#include "clientw.h"
#include "clientl.h"


class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(
//            ClientW * client,
            ClientL * client,
            QObject *parent = nullptr
   );

    void start();
    void run() override;


private:
    bool isRunning;
//    ClientW * client;
    ClientL * client;

signals:
    void responseChat(ResChat);
};

#endif // RECEIVETHREAD_H
