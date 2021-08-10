#ifndef DEVICESCAN_H
#define DEVICESCAN_H

#include <QObject>
#include "include.h"
#include "datadb/facedb.h"
#include "include/netapi.h"
#include "NetworkConf.h"
#include "include.h"

class devicescan : public QObject
{
    Q_OBJECT
public:
    explicit devicescan(QObject *parent = nullptr);
    ~devicescan();




signals:
    void finished(int);
    void isModleExisted(bool,bool);

public slots:
    void closeThread(void);
    void threadStart();
    void timeCheckNet();


private:    
    static devicescan *mydevicescan;


};

#endif // DETECT_H
