#include "setparameter.h"
#include "ui_setparameter.h"

setparameter::setparameter(QString titel, int mode, QWidget *parent) :
    QDialog(parent),
    devMode(mode),    
    ui(new Ui::setparameter)
{
    ui->setupUi(this);
 //   setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::FramelessWindowHint |Qt::WindowTitleHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QFont font;
    font.setFamily("Microsoft YaHei");

    QList<QLabel *>charlist;
    charlist << this->findChildren<QLabel *>();
    foreach(QLabel *lbtext,charlist)
    {


        lbtext->setFont(font);        
        lbtext->setStyleSheet("color:#666666;Font:20px");

    }

    ui->lb_pagename->setText(titel);

    initItem();
    QTimer *checkIp = new QTimer;
    checkIp->start(1000);
    connect(checkIp,SIGNAL(timeout()),this,SLOT(disIp()));
    bakip = " ";
}

void setparameter::initItem()
{
    QStringList comtent;
    QFont font;
    font.setFamily("微软雅黑");


    ui->bt_confirm->setText(QObject::tr("确 定"));
    ui->bt_confirm->setFont(font);
    ui->bt_confirm->setStyleSheet("border:1px groove white;"
                                       "border-radius:5px;"
                                       "padding:2px 4px;"
                                       "background-color:#2966ff;"
                                       "color:white;"
                                       "Font:26px");

    ui->bt_cancel->setText(QObject::tr("取 消"));
    ui->bt_cancel->setFont(font);
    ui->bt_cancel->setStyleSheet("border:1px groove white;"
                                       "border-radius:5px;"
                                       "padding:2px 4px;"
                                       "background-color:#f0f0f0;"
                                       "color:#333333;"
                                       "Font:26px");

    ui->wt_1->setStyleSheet("#wt_1{background-color:#ffffff;}");

    QList<QLineEdit *>editlist;
    editlist << this->findChildren<QLineEdit *>();
    foreach(QLineEdit *letext,editlist)
    {
        letext->setFont(font);        
        letext->setStyleSheet("color:#333333;Font:20px;border:1px solid gray;border-radius:3px;");
        letext->setMaxLength(40);
    }
    ui->le_temp->setMaxLength(5);
    ui->le_ip->setMaxLength(15);
    ui->le_port->setMaxLength(5);




    ui->lb_pagename->setStyleSheet("color:#333333;Font:24px;");
    ui->lb_line1->setStyleSheet("background-color:#ebebeb;");

    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
    font.setPixelSize(24);
    ui->tabWidget->setTabText(0,QObject::tr("设备参数"));
    ui->tabWidget->setTabText(1,QObject::tr("网络设置"));
    ui->tabWidget->setTabText(2,QObject::tr("关于设备"));
    ui->tabWidget->setFont(font);
    /***page 1***/
    ui->lb_offfire->setText(QObject::tr("熄火检测时间："));
    ui->lb_temp->setText(QObject::tr("温度报警阈值："));
    ui->lb_standby->setText(QObject::tr("待机时间："));
    ui->lb_retention->setText(QObject::tr("滞留检测时间："));
    ui->lb_voice->setText(QObject::tr("语音音量："));

    ui->lb_dispress->setText(QObject::tr("*读取U盘中的相关信息*"));
    ui->lb_dispress->setStyleSheet("Font:13px;color:#999999");


    ui->bt_reboot->setText(QObject::tr("设备重启"));
    ui->bt_reboot->setFont(font);
    ui->bt_reboot->setStyleSheet("border:1px solid #2966ff;"
                                       "border-radius:5px;"
                                       "padding:2px 4px;"
                                       "background-color:#ffffff;"
                                       "color:#2966ff;"
                                       "Font:20px");

    ui->bt_readconf->setText(QObject::tr("读取配置"));
    ui->bt_readconf->setFont(font);
    ui->bt_readconf->setStyleSheet("border:1px solid #2966ff;"
                                       "border-radius:5px;"
                                       "padding:2px 4px;"
                                       "background-color:#ffffff;"
                                       "color:#2966ff;"
                                       "Font:20px");

    ui->bt_recover->setText(QObject::tr("清除数据"));
    ui->bt_recover->setFont(font);
    ui->bt_recover->setStyleSheet("border:1px solid #2966ff;"
                                       "border-radius:5px;"
                                       "padding:2px 4px;"
                                       "background-color:#ffffff;"
                                       "color:#2966ff;"
                                       "Font:20px");



    /***page 2***/
    ui->lb_wifi->setText(QObject::tr("WIFI开关："));
    ui->lb_4G->setText(QObject::tr("4G开关:"));
    ui->lb_now->setText(QObject::tr("当前热点"));
    ui->lb_nowip->setText(QObject::tr("当前IP"));
    ui->lb_APN->setText((QObject::tr("APN")));
    ui->lb_user->setText((QObject::tr("用户名:")));
    ui->lb_pwd->setText((QObject::tr("密码:")));

    ui->bt_wifi->setStyleSheet("border-image:url(:/off.png);");
    ui->bt_4G->setStyleSheet("border-image:url(:/off.png);");


    ui->lb_serverIp->setText(QObject::tr("服务器IP："));
    ui->lb_serverPort->setText(QObject::tr("服务器端口："));



    ui->bt_refresh->setIcon(QIcon(":/refresh2.png"));
    ui->bt_refresh->setText(QObject::tr("刷新"));
    ui->bt_refresh->setIconSize(QSize(80,40));
    ui->bt_refresh->setStyleSheet("border:2px groove #1671F1;"
                                 "border-radius:40px;"
                                 "color:#1671F1;"
                                  );




    /*wifi list start*/
       connect(ui->lt_wifi, SIGNAL(itemClicked(QListWidgetItem  *)), this, SLOT(WifiListClick(QListWidgetItem  *)));
       ui->lt_wifi->setStyleSheet(
        "color:#333333"
       // "border:3px solid gray;"
       //  "padding:3px 0px 0px 0px;"
           );

       ui->lt_wifi->setFlow(QListView::TopToBottom);
    //   ui->lt_wifi->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //   ui->lt_wifi->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

       /* 设置为像素滚动 */
       ui->lt_wifi->setVerticalScrollMode(QListWidget::ScrollPerPixel);
           /* 设置鼠标左键拖动 */
       QScroller::grabGesture(ui->lt_wifi,QScroller::LeftMouseButtonGesture);
       ui->lt_wifi->setIconSize(QSize(45,40));

    /***page 3***/
    ui->lb_version->setText(QObject::tr("版本号："));
    ui->lb_sn->setText(QObject::tr("序列号："));
    ui->lb_mode->setText(QObject::tr("设备型号："));
    ui->lb_ip->setText(QObject::tr("KEY:"));
    ui->lb_mac->setText(QObject::tr("SEC:"));




}


setparameter::~setparameter()
{    
    delete ui;
}

void setparameter::paintEvent(QPaintEvent *)
{
  //
    QPainterPath path;
    path.addRoundedRect(10, 10, this->width() - 20, this->height() - 20, 10, 10);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 80);
    for (int i = 0; i < 10; i++)
    {
    QPainterPath path;
    path.addRoundedRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2,10,10);
    color.setAlpha(150 - qSqrt(i) * 50);
    painter.setPen(color);
    painter.drawPath(path);
    }

}



void setparameter::on_bt_cancel_clicked()
{
    frmInput2019::Instance()->hide();
    close();
}





void setparameter::on_bt_confirm_clicked()
{
    QByteArray usersn,name,id,cardNum;
    QMessageBox megboxWrong;
    QString strId,strCard;
    QStringList key;
    QStringList value;
    QStringList list;
    QString ret;
    bool result = false;
    comFunc mfunc;
    /*检测IP地址正确性*/
    if(ui->le_ip->text().isEmpty() != true)
    {
     //   if(ui->le_ip->text().compare(pwd,Qt::CaseSensitive) !=0 )
        if(mfunc.is_ipaddr(ui->le_ip->text()))
        {
            megboxWrong.setText(QObject::tr("请输入正确的IP地址！"));
            megboxWrong.setWindowTitle(QObject::tr("错误"));
            megboxWrong.setIcon(QMessageBox::Information);
            QPushButton(megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole));
            megboxWrong.exec();
            return;
        }

    }

    float temp = ui->le_temp->text().toFloat();
    if((temp>40)||(temp<36))
    {
        megboxWrong.setText(QObject::tr("温度阈值不在36-40℃范围内，请重新输入！"));
        megboxWrong.setWindowTitle(QObject::tr("错误"));
        megboxWrong.setIcon(QMessageBox::Information);
        QPushButton(megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole));
        megboxWrong.exec();
        ui->le_temp->setFocus();
        return;
    }

    /*save to sql*/
    /*save dev par*/
    s_devInfo.leftDetect = ui->le_retention->text().toInt();
    s_devInfo.delayOff = ui->le_offfire->text().toInt();
    s_devInfo.stadbyTime = ui->le_standby->text().toInt();
    s_devInfo.volume = ui->le_voice->text().toInt();
    int iVolume;
     ExternalApi::getVolume(iVolume);
     qDebug()<<"sys.volume:"<<iVolume;
    qDebug()<<"s_devInfo.volume:"<<s_devInfo.volume;

    bool setvolume = ExternalApi::setVolume(s_devInfo.volume);
    if(!setvolume)
    {
        qDebug()<<"set volume error!";
        goto Error;
    }
    result=faceDb::Instance()->setDevConf();
    if(!result)
    {
        qDebug()<<"set devconf error!";
        goto Error;
    }

    s_netInfo.netMode = netMode;
    if(1 == s_netInfo.netMode)
    {
        s_netInfo.APN = ui->le_APN->text();
        s_netInfo.user = ui->le_user->text();
        s_netInfo.pwd = ui->le_pwd->text();
        s_netInfo.serverIp = ui->le_ip->text();
        s_netInfo.serverPort = ui->le_port->text().toInt();
        result = faceDb::Instance()->set4GNetConf();
        if(!result)
        {
            qDebug()<<"set 4Gnet error!";
            goto Error;
        }
    }
    frmInput2019::Instance()->hide();
    close();
    return;
Error:
    megboxWrong.setText(QObject::tr("保存配置错误。"));
    megboxWrong.setWindowTitle(QObject::tr("错误"));
    megboxWrong.setIcon(QMessageBox::Information);
    QPushButton(megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole));
    megboxWrong.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    megboxWrong.exec();
    return;
}


void setparameter::initPageThree(QString ver, QString sn, QString ip,QString mac)
{
    ui->lb_disversion->setText(ver);
    ui->lb_dissn->setText(sn);  
    ui->lb_disip->setText(ip);
    ui->lb_dismac->setText(mac.toUpper());
}

void setparameter::initPageOne(QString offT,QString retT,QString sdT,QString vol,QString temp)
{
    ui->le_offfire->setText(offT);
    ui->le_retention->setText(retT);
    ui->le_standby->setText(sdT);
    ui->le_voice->setText(vol);
    if(devMode == 0)
    {
        ui->le_temp->setEnabled(false);
        ui->le_temp->setText("37.3");
        ui->le_temp->setStyleSheet("color:#999999;Font:20px");
    }
    else
    {
        ui->le_temp->setText(temp);
    }
}


void setparameter::initPageTwo(QString netMode,
                               QString sIp, QString sPort,
                               QString ssid,QString nowIp,
                               QString apn,QString user,QString pwd
                               )
{
    if(netMode == "2")
    {
        ui->bt_wifi->setStyleSheet("border-image:url(:/on.png)");
        ui->wt_pwd->hide();
        wifiScan();
    }
    else if(netMode == "1")
    {
        ui->bt_4G->setStyleSheet("border-image:url(:/on.png)");
        ui->wt_wifi->hide();
    }

    ui->le_APN->setText(apn);
    ui->le_user->setText(user);
    ui->le_pwd->setText(pwd);
    ui->le_ip->setText(sIp);
    ui->le_port->setText(sPort);

    ui->lb_ssid->setText(ssid);
    ui->lb_wifiIp->setText(nowIp);
    wifiSsid = ssid;
}



void setparameter::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        initItem();
        break;
     default:
        break;
    }

}





void setparameter::on_bt_reboot_clicked()
{
    QMessageBox megboxWrong;
    megboxWrong.setText(QObject::tr("确定需要重启设备吗？"));
    megboxWrong.setWindowTitle(QObject::tr("提示"));
    megboxWrong.setIcon(QMessageBox::Information);
    QPushButton *okbut = megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole);
    QPushButton(megboxWrong.addButton(QObject::tr("取 消"),QMessageBox::NoRole));
    megboxWrong.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    megboxWrong.exec();
    if(megboxWrong.clickedButton() == okbut)
    {
        system("sync;reboot");
    }
}

void setparameter::on_bt_recover_clicked()
{
    bool ret;
    comFunc mfunc;
    QMessageBox megboxWrong,errbox;
    megboxWrong.setText(QObject::tr("确定需要将设备清除数据吗？"));
    megboxWrong.setWindowTitle(QObject::tr("提示"));
    megboxWrong.setIcon(QMessageBox::Information);
    QPushButton *okbut = megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole);
    QPushButton(megboxWrong.addButton(QObject::tr("取 消"),QMessageBox::NoRole));
    megboxWrong.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    megboxWrong.exec();
    if(megboxWrong.clickedButton() == okbut)
    {
        /*rebuild data*/
        ret = faceDb::Instance()->deleteTable(TABLE_PERSON);
        QFileInfo wavfile(qApp->applicationDirPath()+"/userwav");
        mfunc.rmAllFile(wavfile);
        if(!ret)
        {
            goto Error;
        }

        ret = faceDb::Instance()->deleteTable(TABLE_RECORD);
        QFileInfo recordPic(qApp->applicationDirPath()+"/JpgPhoto");
        mfunc.rmAllFile(recordPic);
        QFileInfo regPic(qApp->applicationDirPath()+"/Photo");
        mfunc.rmAllFile(regPic);
        if(!ret)
        {
            goto Error;
        }
        return;

    }
    else
    {
        return;
    }
Error:
        {
            errbox.setText(QObject::tr("清除数据出错。"));
            errbox.setWindowTitle(QObject::tr("提示"));
            errbox.setIcon(QMessageBox::Information);
            QPushButton *okbut = errbox.addButton(QObject::tr("确 定"),QMessageBox::YesRole);
            QPushButton(errbox.addButton(QObject::tr("取 消"),QMessageBox::NoRole));
            errbox.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
            errbox.exec();
            return;
        }
}




void setparameter::on_bt_wifi_clicked()
{
    ui->bt_wifi->setStyleSheet("border-image:url(:/on.png)");
    ui->bt_4G->setStyleSheet("border-image:url(:/off.png)");
    netMode = 2;
    ui->wt_pwd->hide();
    ui->wt_wifi->show();
}

void setparameter::on_bt_4G_clicked()
{
    ui->bt_wifi->setStyleSheet("border-image:url(:/off.png)");
    ui->bt_4G->setStyleSheet("border-image:url(:/on.png)");
    netMode = 1;
    ui->wt_pwd->show();
    ui->wt_wifi->hide();
}

void setparameter::wifiScan()
{

    m_vecWifi_Info.clear();
    memset(ap, 0, sizeof(ap));
    int iCount = sizeof(ap) / sizeof(ap_info);
    if(NetworkConf::Instance()->wifiScan(ap, iCount))
    {
        for(int i=0;i<iCount;i++)
       {
           S_Wifi_Info wifiInfo;
           wifiInfo.sWifiName = QString(QLatin1String(ap[i].essid));
           wifiInfo.sWifiMac = QString(QLatin1String(ap[i].bssid));
           wifiInfo.sSecmode = QString::number(ap[i].secmode);
           wifiInfo.iWifiSignal = ap[i].rssi;
           wifiInfo.adhoc = ap[i].adhoc;
           wifiInfo.authMode = ap[i].authmode;
           m_vecWifi_Info.push_back(wifiInfo);

           qDebug()<<wifiInfo.sWifiName<<wifiInfo.sWifiMac<<wifiInfo.sSecmode<<wifiInfo.iWifiSignal;


        }
    }
     showWifiList();

}

void setparameter::showWifiList()
{
    //向列表框添加wifi
    ui->lt_wifi->clear();
    if(NetworkConf::Instance()->bWifiModular&&m_vecWifi_Info.size()>0)
    {
    //    emit sigCheckWifi();
        for(auto wifi : m_vecWifi_Info)
        {

            QWidget* pBoxWidget = new QWidget;
            pBoxWidget->setStyleSheet("background-color:#999999;");
            //wifi图标
            QWidget* pIconWidget = new QWidget(pBoxWidget);
            pIconWidget->setGeometry(0, 10, 30, 24);

            if(wifi.iWifiSignal>=-120 && wifi.iWifiSignal<=-100)
            {
                pIconWidget->setStyleSheet("border-image:url(:/state_wifi_one.png)");
            }
            else if(wifi.iWifiSignal>-100 && wifi.iWifiSignal<=-75)
            {
                 pIconWidget->setStyleSheet("border-image:url(:/state_wifi_two.png)");
            }
            else if(wifi.iWifiSignal>-75 && wifi.iWifiSignal<=-55)
            {
                 pIconWidget->setStyleSheet("border-image:url(:/state_wifi_three.png)");
            }
            else
            {
                 pIconWidget->setStyleSheet("border-image:url(:/state_wifi_four.png)");
            }

            //wifi名字
            QLabel* pLblName = new QLabel(pBoxWidget);
            pLblName->setGeometry(40, 0, 200, 63);
            pLblName->setText(wifi.sWifiName);
            pLblName->setStyleSheet("QLabel{font-size:20px;font-family:Microsoft YaHei;font-weight:400;color:#fff;line-height:30px;}");
            //wifiMac
         //   QLabel* pLblMac = new QLabel(pBoxWidget);
         //   pLblMac->setGeometry(500, 0, 200, 63);
         //   pLblMac->setText(wifi.sWifiMac);
        //    pLblMac->setHidden(true);
            //右侧图标
          //  QPushButton* pBtnLockIcon = new QPushButton(pBoxWidget);
          // pBtnLockIcon->setGeometry(m_rectDesk.width()-180, 24, 21, 21);
          //  pBtnLockIcon->setStyleSheet("border-image:url(:/WisdomCurrent/image/img/01-img/machine_lock_default.png)");

            //ListWidgetItem
            QListWidgetItem *pListWidgetItem = new QListWidgetItem();
            pListWidgetItem->setSizeHint(QSize(50, 80));
            ui->lt_wifi->addItem(pListWidgetItem);
            ui->lt_wifi->setItemWidget(pListWidgetItem, pBoxWidget);

        }
    }
}

void setparameter::on_bt_refresh_clicked()
{
    wifiScan();
    showWifiList();
}


void setparameter::WifiListClick(QListWidgetItem  *item) //wifi列表点击消息处理函数
{

    int postion = ui->lt_wifi->row(item);
    QString m_sWifiConnName =  m_vecWifi_Info[postion].sWifiName;//wifi名称
    QString m_sWifiConnMac =  m_vecWifi_Info[postion].sWifiMac;//wifi mac 地址
    //set_scan_list(false);
    //flag_refresh = true;
    qDebug()<<"mac:"<<m_sWifiConnMac;
    common_input *pcommon_input = new common_input;
    connect(pcommon_input,SIGNAL(senddata(QString,bool)),this,SLOT(receivedata(QString,bool)));
    inputdata = "";
    pcommon_input->input_text(m_sWifiConnName,m_sWifiConnMac);
    pcommon_input->disfont(10);
    pcommon_input->setWindowModality(Qt::WindowModal);
    pcommon_input->setAttribute(Qt::WA_DeleteOnClose,true);
    pcommon_input->exec();

    qDebug()<<"get data "<<inputdata;
    qDebug()<<"get flag "<<input_flag;
    wifiSsid = m_sWifiConnName;
     qDebug()<<__func__<<__LINE__<<"wifiSsid"<<wifiSsid;
    if(input_flag)
    {
        input_flag = false;
        comFunc mfunc;
        bool ret=NetworkConf::Instance()->wifiConnect(mfunc.qstringTostd(m_sWifiConnName),mfunc.qstringTostd(m_sWifiConnMac),mfunc.qstringTostd(inputdata));
        bakip.clear();
        nowip.clear();
        s_netInfo.wifiSsid = m_vecWifi_Info[postion].sWifiName;
        s_netInfo.wifiMac = m_vecWifi_Info[postion].sWifiMac;
        s_netInfo.wifiPwd = inputdata;
        s_netInfo.secMode = m_vecWifi_Info[postion].sSecmode.toInt();
        s_netInfo.adhoc = m_vecWifi_Info[postion].adhoc;
        s_netInfo.authMode = m_vecWifi_Info[postion].authMode;
        faceDb::Instance()->setWifiNetConf();
        ui->lb_wifiIp->clear();
    }

 //   set_scan_list(true);
 //   flag_refresh = false;
}


void setparameter::receivedata(QString str,bool flag)
{
    input_flag = flag;
    inputdata = str;
}

void setparameter::disIp()
{
    if(2==s_netInfo.netMode)
    {
        comFunc mfunc;
    //   qDebug()<<__func__<<__LINE__<<"wifip:"<<mfunc.stddToqstring(NetworkConf::Instance()->netIp())<<"wifiSsid"<<wifiSsid;
        ui->lb_ssid->setText(wifiSsid);
        nowip = mfunc.stddToqstring(NetworkConf::Instance()->netIp());
        if(nowip != bakip)
        {
            ui->lb_wifiIp->setText(nowip);
            bakip = nowip;
        }
        else if((nowip.isEmpty())&&(bakip.isEmpty()))
        {
            ui->lb_wifiIp->setText("wifi连接中...");
        }

    }	
  }
	
void setparameter::on_bt_readconf_clicked()
{
   // S_FILEFLAG *fileFlag;
   // comFunc mfunc;
   // mfunc.UsbExit("/media/usb0/");
     /*
      * fileFlag = mfunc.UsbExit("/media/usb0/");
    QString up,conf,stand,voice;
    if(fileFlag->flag_up)
    {
        up = "程序文件升级成功";
    }
    if(fileFlag->flag_conf)
    {
        conf = "配置文件升级成功";
    }
    if(fileFlag->flag_conf)
    {
        stand = "待机图片升级成功";
    }
    if(fileFlag->flag_conf)
    {
        voice = "语音文件升级成功";
    }

    QMessageBox megboxWrong;
    megboxWrong.setText(QObject::tr("%1\n%2\n%3\n%4").arg(up).arg(conf).arg(stand).arg(voice));
    megboxWrong.setWindowTitle(QObject::tr("提示"));
    megboxWrong.setIcon(QMessageBox::Information);
    QPushButton *okbut = megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole);
    megboxWrong.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    megboxWrong.exec();
    */
    QDir dir;
    dir.setPath("/media/usb0/");
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();
    QString src,des;

    if (list.count() <= 0)
    {
        qDebug() << "文件夹为空";

    }
}

