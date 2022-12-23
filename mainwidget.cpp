#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , client(new ClientW)
    , recvThread(new ReceiveThread(client))
{
    ui->setupUi(this);

    if (client->connectServer())
    {
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
    delete client;
    delete recvThread;
}

void MainWidget::responseChat(ResChat resChat)
{
    QMessageBox msgbox;
    msgbox.setText(QString::fromStdString(resChat.Msg()));
    msgbox.exec();
}

