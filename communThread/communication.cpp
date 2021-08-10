#include "communication.h"


communication *communication::mycommunit= nullptr;
communication::communication(QObject *parent) : QObject(parent)
{

    mycommunit = this;
}


communication::~communication()
{
    qDebug()<<"detect thread close";

}

void communication::threadStart()
{
   /*init conmuication parm*/
    

    qDebug()<<"conmuication thread run";
}

void communication::closeThread(void)
{
    emit finished(0);
}




