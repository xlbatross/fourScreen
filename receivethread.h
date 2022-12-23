#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include "clientw.h"


class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(
            ClientW * client,
            QObject *parent = nullptr
   );

    void start();
    void run() override;


private:
    bool isRunning;
    ClientW * client;

signals:
    void responseChat(ResChat);
};

#endif // RECEIVETHREAD_H
