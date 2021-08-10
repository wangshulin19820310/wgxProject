#ifndef COMMUNITCATION_H
#define COMMUNITCATION_H

#include <QObject>
#include "include.h"
#include "datadb/facedb.h"

class communication : public QObject
{
    Q_OBJECT
public:
    explicit communication(QObject *parent = nullptr);
    ~communication();

    int iThreshold;
    



signals:
    void finished(int);

public slots:
    void closeThread(void);
    void threadStart();


private:    
    static communication *mycommunit;


};

#endif // DETECT_H
