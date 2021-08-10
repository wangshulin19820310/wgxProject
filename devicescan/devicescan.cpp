#include "devicescan.h"


devicescan *devicescan::mydevicescan= nullptr;
devicescan::devicescan(QObject *parent) : QObject(parent)
{

    mydevicescan = this;
}


devicescan::~devicescan()
{
    qDebug()<<"detect thread close";

}

void devicescan::threadStart()
{
    bool wififlag,wirlessflag;
    NetworkConf::Instance()->isExistWifiAndWnet(wififlag,wirlessflag);
    qDebug()<<__func__<<__LINE__<<wififlag<<wirlessflag;
    if((false == wififlag)&&(false == wirlessflag))
    {
        emit isModleExisted(wififlag,wirlessflag);
        return;
    }
    else
    {
        qDebug()<<__func__<<__LINE__;
        QTimer *autoTimer = new QTimer;
        autoTimer->start(30000);
        connect(autoTimer,SIGNAL(timeout()),this,SLOT(timeCheckNet()));
    }
}

void devicescan::closeThread(void)
{
    emit finished(0);
}


void devicescan::timeCheckNet()
{
    qDebug()<<__func__<<__LINE__;
    NetworkConf::Instance()->timeCheckNet();
}


