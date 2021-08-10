#ifndef SETPARAMETER_H
#define SETPARAMETER_H

#include <QWidget>
#include "include.h"
#include "datadb/facedb.h"

#include "comFunc/comfunc.h"
#include "comFunc/common_input.h"
#include "customtabstyle.h"
#include "include/netapi.h"
#include "devicescan/NetworkConf.h"
#include "include/ExternalApi.h"

struct  S_Wifi_Info
{
    QString sWifiName;
    QString sWifiMac;
    QString sSecmode;
    int iWifiSignal;
    int adhoc;
    int authMode;
};


namespace Ui {
class setparameter;
}

class setparameter : public QDialog
{
    Q_OBJECT

public:
    explicit setparameter(QString titel, int mode, QWidget *parent = nullptr);
    ~setparameter();

public:
    void paintEvent(QPaintEvent *event);    

    bool flag_pwd = false;
    int devMode;

    QString devIp,devSn;
    QString serverIp,serverPort;
    QString tempOnorOff;
    QString wifiSsid,nowip,bakip;
    bool flag_update = false;
    bool flag_parmeter = false;
    int result;
    bool input_flag;
    QString inputdata;
    int netMode;

    void initPageOne(QString offT,QString retT,QString sdT,QString vol,QString temp);
    void initPageTwo(QString netMode,
                     QString sIp, QString sPort,
                     QString ssid,QString nowIp,
                     QString apn,QString user,QString pwd
                     );
    void initPageThree(QString ver, QString sn, QString ip,QString mac);
    void initWedgit();
    void stepPage(int step);    
    void changeEvent(QEvent *e);
    void initItem();
    void getCommonResult(QString str);
    void getDownloadPar(bool);
    void wifiScan();
    void showWifiList();


signals:
    void sendSetPar(QString);
    void sendStartHttp(QString);


private slots:


    void on_bt_cancel_clicked();    

    void on_bt_confirm_clicked();

    void on_bt_reboot_clicked();

    void on_bt_recover_clicked();

    void on_bt_wifi_clicked();

    void on_bt_4G_clicked();

    void on_bt_refresh_clicked();

    void WifiListClick(QListWidgetItem  *item);

    void receivedata(QString,bool);

    void disIp();
	
	void on_bt_readconf_clicked();

private:
    Ui::setparameter *ui;
    comFunc *pcom_func = new comFunc;
    ap_info ap[20];
    vector<S_Wifi_Info> m_vecWifi_Info;
      
};

#endif // COMMON_DEVDETAIL_H
