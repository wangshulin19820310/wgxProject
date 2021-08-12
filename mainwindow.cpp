#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "include/ExternalApi.h"
#include "include/xosal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint);
 //   setAttribute(Qt::WA_AcceptTouchEvents);

    faceDb::Instance();
    comFunc comfunc;
    QStringList fileComtent = comfunc.readConfig("TerminaManage.cnf");
    QStringList conf = comfunc.analysisComten(fileComtent);
    ExternalApi::setVolume(s_devInfo.volume);

    /*创建识别线程*/
    /*detect*/
    m_detectThread = new detect();
    detectThread_id = new QThread;
    if(conf.size() == 4)
    {
        qDebug()<<"read conf file succ!";
        m_detectThread->init(conf.at(0),conf.at(1),conf.at(2),conf.at(3),71);
    }
    else
    {
        qDebug()<<"read conf file err!";
        m_detectThread->init("113.100.143.90","8393","v4Y54q2eF5IdETzZ","T4plqpGU8hcSZKGheakzNIELnnx19D",71);
    }
    connect(this,&MainWindow::setCheck,m_detectThread,&detect::isCheck);
    connect(detectThread_id,SIGNAL(started()),m_detectThread,SLOT(threadStart()));

    connect(m_detectThread, &detect::sendtoMain,this,&MainWindow::detectResult);
    connect(m_detectThread, &detect::hasFace,this,&MainWindow::startlightInterval);
    connect(m_detectThread, &detect::hasDetect,this,&MainWindow::startfaceInterval);
    connect(m_detectThread, &detect::finished, detectThread_id, &QThread::exit);
    connect(m_detectThread, &detect::finished, detectThread_id, &QThread::quit);

    m_detectThread->moveToThread(detectThread_id);
    detectThread_id->start();

    connect(T_detect,SIGNAL(timeout()),this,SLOT(endfaceInterval()));
    connect(T_light,SIGNAL(timeout()),this,SLOT(endlightInterval()));

    /*********************************************/

    /*communication*/
    m_communitThread = new communication();
    communitThread_id = new QThread;
    connect(communitThread_id,SIGNAL(started()),m_communitThread,SLOT(threadStart()));
    connect(m_communitThread, &communication::finished, communitThread_id, &QThread::exit);
    connect(m_communitThread, &communication::finished, communitThread_id, &QThread::quit);

    m_communitThread->moveToThread(communitThread_id);
 //   communitThread_id->start();
     /*********************************************/

    /*devicescan --wifi and 4G*/
    m_devicescanThread= new devicescan();
    devicescanThread_id = new QThread;
    connect(devicescanThread_id,SIGNAL(started()),m_devicescanThread,SLOT(threadStart()));
    connect(m_devicescanThread, &devicescan::finished, devicescanThread_id, &QThread::exit);
    connect(m_devicescanThread, &devicescan::finished, devicescanThread_id, &QThread::quit);
    connect(m_devicescanThread, &devicescan::isModleExisted, this, &MainWindow::modleExisted);
    m_devicescanThread->moveToThread(devicescanThread_id);
    devicescanThread_id->start();
     /*********************************************/


    /*check key status*/
    /*check door num */
     bool ret = ExternalApi::getDoorCount(offFireNum);
     if(ret == false)
     {
        /*display report err!*/
         qDebug()<<"get door num err!";
     }

     ret = ExternalApi::getRelayCount(delayNum);
     if(ret == false)
      {
         /*display report err!*/
         qDebug()<<"get relay num err!";
      }

     QTimer *scanT = new QTimer;
     scanT->start(500);
     connect(scanT,SIGNAL(timeout()),this,SLOT(timeOver()));
     status = statusbak = 0;
     qDebug()<<"door status scan run";
     /*********************************************/

    /*display screen*/
    QFont font;
    font.setFamily("Microsoft YaHei");
    ui->lb_timer->setFont(font);
    ui->lb_timer->setText("");
    ui->lb_timer->setStyleSheet("color:#ffffff;Font:10pt;Font:bold;");
    Mainwindow_timer = startTimer(1000); //开始显示时间定时

    /*display detect result*/
    disInfo = new QWidget(this);    
    disInfo->setGeometry((this->width()-670)/2,(this->height()-290)/2,670,290);
    disInfo->setObjectName("disInfo");
    disInfo->setStyleSheet("#disInfo{background-image:url(:/faceright.png);}");

    pic = new QLabel(disInfo);
    pic->setGeometry(40,30,141,180);
    pic->setStyleSheet("color:#ffffff;");
    name = new QLabel(disInfo);
    name->setGeometry(268,30,120,50);
    name->setStyleSheet("color:#ffffff;font:24px;font:bold;");
    disInfo->hide();
    connect(T_faceResult,SIGNAL(timeout()),this,SLOT(showTime()));
    QPushButton *bt_menu = new QPushButton(this);
    bt_menu->setFlat(true);
    bt_menu->setStyleSheet("background:transparent;background-image:url(:/back.png)");
    bt_menu->setGeometry(0,0,200,100);
  //  connect(bt_menu,SIGNAL(clicked(bool)),this,SLOT(on_bt_set_clicked()));
    connect(bt_menu,SIGNAL(pressed()),this,SLOT(bt_menu_pressed()));
    connect(bt_menu,SIGNAL(released()),this,SLOT(bt_menu_released()));
    connect(T_btMenu,SIGNAL(timeout()),this,SLOT(on_bt_set_clicked()));

    plate = new QLabel(this);
    plate->setStyleSheet("font:16pt;font:bold;color:#ffffff");
    //plate->setText(s_devInfo.chepai);
    plate->setText("粤B88888");
    plate->setGeometry(20, 650, 320, 60);

    perNum = new QLabel(this);
    perNum->setStyleSheet("font:44pt;color:#00ff00");
    //plate->setText(s_devInfo.chepai);
    perNum->setText("88");
    perNum->setGeometry(1050,510,200,200);


    /***dis net signal***/
    QTimer *T_signal = new QTimer;
    T_signal->setInterval(5000);
    T_signal->start(5000);
    connect(T_signal,SIGNAL(timeout()),this,SLOT(showSigal()));
    /********************/


    /*left time*/
    connect(T_left,SIGNAL(timeout()),this,SLOT(alarmEvent()));
    connect(T_playAlarm,SIGNAL(timeout()),this,SLOT(replayalarm()));
    /******/


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bt_set_clicked()
{
    qDebug()<<__func__<<__LINE__;
    setparameter *psetpar = new setparameter("设备设置",0);
    psetpar->setAttribute(Qt::WA_DeleteOnClose,true);
    psetpar->setWindowModality(Qt::WindowModal);
    psetpar->initPageOne(QString::number(s_devInfo.delayOff),QString::number(s_devInfo.leftDetect),QString::number(s_devInfo.stadbyTime),QString::number(s_devInfo.volume),"37.3");
    psetpar->initPageTwo(QString::number(s_netInfo.netMode),s_netInfo.serverIp,QString::number(s_netInfo.serverPort),s_netInfo.wifiSsid,wifiIp,s_netInfo.APN,s_netInfo.user,s_netInfo.pwd);
    psetpar->initPageThree(DEVVESION,s_devInfo.devId,s_devparInfo.productKey,s_devparInfo.devSecret);

  //  connect(pchangPort,&common_changePort::newPort,this,&MainWindow::getNewPort);
    emit setCheck(false);
    psetpar->exec();
    emit setCheck(true);
}


void MainWindow::timerEvent(QTimerEvent *event)
{
    QString currentTime;
    if(event->timerId() == Mainwindow_timer) //定时显示时间
    {
        /*display time*/
        ui->lb_timer->setText("");
        QDateTime dt;     
        dt = QDateTime::currentDateTime();
        currentTime = dt.toString("HH:mm:ss");
        ui->lb_timer->setText(currentTime);
        QString currentDate = locale.toString(QDateTime::currentDateTime(),QString("yyyy/MM/dd"));

        ui->lb_timer->setText(currentTime+" "+currentDate);



    }
}



void MainWindow::timeOver()
{
    std::string error;
    for(int i=0;i<offFireNum;i++)
    {
        ExternalApi::getDoorState(i,status,error);
        if((status != statusbak)&&(status == 1))
        {
            qDebug()<<"offFire slove offFire status:"<<QString::number(status);
            statusbak = status;
            /*to display offFire sigal off*/
            T_left->stop();
            T_playAlarm->stop();

        }

        if(status != statusbak)
        {
            qDebug()<<"offFire open status:"<<QString::number(status);
            statusbak = status;
            /*to display offFire sigal*/
            T_left->start(s_devInfo.leftDetect*1000*60);

        }
    }
}



void MainWindow::modleExisted(bool wifi,bool wirless)
{
    if((false == wifi)&&(s_netInfo.netMode == 2))
    {
        ui->lb_net->setStyleSheet("border-image:url(:/state_wifi_offline.png)");
    }
    else if((false == wirless)&&(s_netInfo.netMode == 1))
    {
        ui->lb_net->setStyleSheet("border-image:url(:/state_traffic_offline.png)");
    }
}


void MainWindow::detectResult(QString username,QString path)
{
    QImage image;
    QPixmap *pixmap = new QPixmap;
    name->clear();
    pic->clear();
    name->setText(username);
    if(path.length()==0)
    {
        pic->setStyleSheet("border-image:url(:/defaultpic.png)");
    }
    else
    {
        image.load(path);
        if(image.isNull() == true)
        {
            *pixmap = QPixmap::fromImage(image.scaled(QSize(92, 94), Qt::KeepAspectRatio));
        }
        else
        {
            *pixmap = QPixmap::fromImage(image.scaled(QSize(100, 100), Qt::KeepAspectRatio));
        }

        pic->setScaledContents(true);
        pic->setPixmap(*pixmap);
    }
    disInfo->show();
    T_faceResult->start(3000);
}

void MainWindow::showTime()
{
    disInfo->hide();
    T_faceResult->stop();
}


void MainWindow::showSigal()
{
    /*display net*/
    string sWifiName, sWifiMac;
    int iRssi = 0;
    WIFI_STATUS status;

    if (NetworkConf::Instance()->wifiCheck(status, sWifiName, sWifiMac, iRssi))
    {
        if(WIFI_STATUS_CONNECTED == status)
        {
         //    qDebug()<<"iRssi:"<<iRssi;
            comFunc comfunc;
            wifiMac = comfunc.stddToqstring(sWifiMac);
            wifiIp = comfunc.stddToqstring(NetworkConf::Instance()->netIp());
            if(iRssi>=-120&&iRssi<=-100)
            {
                ui->lb_net->setStyleSheet("border-image:url(:/state_wifi_one.png)");
            }else if(iRssi>-100&&iRssi<=-75)
            {
                 ui->lb_net->setStyleSheet("border-image:url(:/state_wifi_two.png)");
            }else if(iRssi>-75&&iRssi<=-55)
            {
                 ui->lb_net->setStyleSheet("border-image:url(:/state_wifi_three.png)");
            }
            else
            {
                ui->lb_net->setStyleSheet("border-image:url(:/state_wifi_four.png)");
            }
        }
        else
        {
            qDebug()<<"wifi statu:"<<status;
        }

     }

}



void MainWindow::endfaceInterval()
{
    T_detect->stop();
    detect::Instance()->m_bFaceSucessSounded = false;
    detect::Instance()->m_bFaceFaileSounded = false;


}

void MainWindow::endlightInterval()
{
    T_light->stop();
    std::string error;
    ExternalApi::setLightBrightness(0,error);
}


void MainWindow::startfaceInterval()
{
    T_detect->start(s_devInfo.faceInterval*1000);
}

void MainWindow::startlightInterval()
{
    T_light->start(10000);
    std::string error;
    ExternalApi::setLightBrightness(90,error);
}

void MainWindow::bt_menu_released()
{
    qDebug()<<"button release";
    T_btMenu->stop();

}

void MainWindow::bt_menu_pressed()
{
    T_btMenu->start(3000);
}

void MainWindow::alarmEvent()
{
    /*send to sever left message*/
    ExternalApi::playAudio("/oem/Face/wav/alarm.wav");
    T_playAlarm->start(2*60*1000);

}

void MainWindow::replayAlarm()
{
    /*send to sever left message*/
    ExternalApi::playAudio("/oem/Face/wav/alarm.wav");

}



