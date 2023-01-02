#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , clientTCP(new ClientWTCP("127.0.0.1", 2500))
    , clientUDP(new ClientWUDP("127.0.0.1", 2501))
//    , clientTCP(new ClientLTCP("127.0.0.1", 2500))
//    , clientUDP(new ClientLUDP("127.0.0.1", 2501))
    , first(new char[320 * 240 * 3]())
    , second(new char[320 * 240 * 3]())
    , third(new char[320 * 240 * 3]())
    , fourth(new char[320 * 240 * 3]())
    , recvTCPThread(new ReceiveThread(clientTCP))
    , recvUDPThread(new ReceiveThread(clientUDP))
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    if (clientTCP->connectServer() && clientUDP->connectServer())
    {
        ReqUDPConnect requdp(clientUDP->sockIp(), clientUDP->sockPort());
        clientTCP->sendData(requdp);

        qRegisterMetaType<ResChat>("ResChat");
        connect(recvTCPThread, SIGNAL(responseChat(ResChat)), this, SLOT(responseChat(ResChat)));
        qRegisterMetaType<ResponseUDP>("ResponseUDP");
        connect(recvUDPThread, SIGNAL(responseImage(ResponseUDP)), this, SLOT(responseImage(ResponseUDP)));
        recvTCPThread->start();
        recvUDPThread->start();

        connect(timer, SIGNAL(timeout()), this, SLOT(sendImage()));
        timer->start(1000/30);
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
    delete recvTCPThread;
    delete recvUDPThread;
}

void MainWidget::sendImage()
{
    if (!cap.isOpened())
    {
        cap.open(0);
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
    }
    if (cap.isOpened())
    {
        cv::Mat img;
        cap.read(img);
        ReqImage reqImage(img);
        clientUDP->sendData(reqImage);
    }
}

void MainWidget::responseChat(ResChat resChat)
{
    QMessageBox msgbox;
    msgbox.setText(QString::fromStdString(resChat.Msg()));
    msgbox.exec();
}

void MainWidget::responseImage(ResponseUDP resUdp)
{
    char * select = NULL;
    vector<int> * numList = NULL;
    QLabel * lb = NULL;
    switch(resUdp.ResponseType())
    {
    case Response::FirstImage:
        select = first;
        numList = &firstNumList;
        lb = ui->label_1;
        break;
    case Response::SecondImage:
        select = second;
        numList = &secondNumList;
        lb = ui->label_2;
        break;
    case Response::ThirdImage:
        select = third;
        numList = &thirdNumList;
        lb = ui->label_3;
        break;
    case Response::FourthImage:
        select = fourth;
        numList = &fourthNumList;
        lb = ui->label_4;
        break;
    }

    if (std::find(numList->begin(), numList->end(), resUdp.SeqNum()) != numList->end())
    {
        QImage qtImage((const unsigned char *) select, 320, 240, QImage::Format_BGR888);
        lb->setPixmap(QPixmap::fromImage(qtImage));
        memset(select, 0, 320 * 240 * 3);
        numList->resize(0);
    }
    memcpy(select + 1024 * (resUdp.SeqNum()), resUdp.DataBytesList()[0].data(), 1024);
    numList->push_back(resUdp.SeqNum());
}
