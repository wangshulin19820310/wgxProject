#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include.h"
#include "detectThread/detect.h"
#include "datadb/facedb.h"
#include "communThread/communication.h"
#include "comFunc/comfunc.h"
#include "devicescan/devicescan.h"
#include "setPage/setparameter.h"
#include "include/netapi.h"
#include "devicescan/NetworkConf.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int Mainwindow_timer;
    QString wifiIp,wifiMac;
    int offFireNum;
    int delayNum;
    int status,statusbak;
    QLocale locale;

    void readConfig();
    void timerEvent(QTimerEvent *event);
    void offFireResult(bool);
    void modleExisted(bool,bool);
    void detectResult(QString name,QString picPath);

private slots:
    void on_bt_set_clicked();
    void timeOver();
    void showTime();
    void showSigal();
    void endfaceInterval();
    void endlightInterval();
    void startfaceInterval();
    void startlightInterval();

signals:
    void setCheck(bool);


private:
    Ui::MainWindow *ui;
    QThread *detectThread_id,*communitThread_id,*devicescanThread_id;
    detect *m_detectThread;
    communication *m_communitThread ;
    devicescan *m_devicescanThread;
    QWidget *disInfo;
    QLabel *pic,*name;
    QTimer *T_faceResult = new QTimer;
    QTimer *T_light = new QTimer();
    QTimer *T_detect = new QTimer();
};

#endif // MAINWINDOW_H
