#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "clientw.h"
//#include "clientl.h"
#include "receivethread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

    ClientW * clientTCP;
    ClientW * clientUDP;
//    ClientL * clientTCP;
//    ClientL * clientUDP;

    char * first;
    vector<int> firstNumList;
    char * second;
    vector<int> secondNumList;
    char * third;
    vector<int> thirdNumList;
    char * fourth;
    vector<int> fourthNumList;

    ReceiveThread * recvTCPThread;
    ReceiveThread * recvUDPThread;

    QTimer * timer;

    cv::VideoCapture cap;

private slots:
    void sendImage();
    void responseChat(ResChat);
    void responseImage(ResponseUDP);
};
#endif // MAINWIDGET_H
