#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
//#include "clientw.h"
#include "clientl.h"
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

//    ClientW * client;
    ClientLTCP * clientTCP;
    ClientLUDP * clientUDP;

    ReceiveThread * recvThread;

private slots:
    void responseChat(ResChat);
};
#endif // MAINWIDGET_H
