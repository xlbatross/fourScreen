#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
//    , client(new ClientW)
    , clientTCP(new ClientLTCP("127.0.0.1", 2500))
    , clientUDP(new ClientLUDP("127.0.0.1", 2501))
    , recvThread(new ReceiveThread(clientTCP))
{
    ui->setupUi(this);

    if (clientTCP->connectServer() && clientUDP->connectServer())
    {
        ReqUDPConnect requdp(clientUDP->sockIp(), clientUDP->sockPort());
        clientTCP->sendData(requdp);
        qRegisterMetaType<ResChat>("ResChat");
        connect(recvThread, SIGNAL(responseChat(ResChat)), this, SLOT(responseChat(ResChat)));
        recvThread->start();
    }
    else
    {
        QMessageBox msgbox;
        msgbox.setText("서버와 연결할 수 없습니다.");
        msgbox.exec();
        this->deleteLater();
    }
}

MainWidget::~MainWidget()
{
    delete ui;
    delete clientTCP;
    delete clientUDP;
    delete recvThread;
}

void MainWidget::responseChat(ResChat resChat)
{
    QMessageBox msgbox;
    msgbox.setText(QString::fromStdString(resChat.Msg()));
    msgbox.exec();
}

