#ifndef INCLUDE_H
#define INCLUDE_H

#include <QKeyEvent>
#include <QWidget>
#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>
#include <QTimerEvent>
#include <QTime>
#include <QObject>
#include <QPixmap>
#include <QGridLayout>
#include <QTextCodec>
#include <QMessageBox>
#include <QBitmap>
#include <QPainter>
#include <QPen>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QScroller>
#include <QTextCodec>
#include <QPropertyAnimation>
#include <QMovie>
#include <QSlider>
#include <QIcon>
#include <QLocale>
#include <QLatin1String>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QDesktopWidget>
#include <QTranslator>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QScreen>
#include <QTimer>
#include <QStandardItemModel>
#include <QTableView>
#include <QTableWidget>
#include <QCheckBox>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QFileDialog>
#include <QTextEdit>
#include <QDesktopServices>
#include <QProgressDialog>
#include <QToolTip>
#include <QFileInfo>
#include <QThread>
#include <QtMath>
#include <QButtonGroup>
/*数据结构定义*/
typedef struct S_DEVPARINFO
{
    QString version;
    QString devName;
    QString productKey;
    QString devSecret;
    QString  OSSDomain;
    QString  OSSKeyId;
    QString  OSSSec;
}S_DEVPARINFO;

typedef struct S_DEVINFO
{
    char devId[32];
    char chepai[8];
    int  workMode;
    int  delayOff;
    int  leftDetect;
    int  stadbyTime;
    int  faceInterval;
    int  clearTime;
    int  shxMode;
    int  volume;
    int  faceSize;
    int  uploadType;
    int  natStatus;
    int  timezone;
    int  temperature;
    int  noReg;
    int  lightType;
    int  personLimit;
    int  setTts;
    int  syncTimer;
}S_DEVINFO;

typedef struct S_PERINFO
{
    int fid;
    QString  name;
    QString  part;
    QString  workId;
    QString  icCard;
    QString  level;
    QString  passWord;
    QString  pic;
    QString  cerType;
    QString  regTime;
    QString  effectTime;
    QByteArray feature;
    int      pGroup;
    QString  go_station;
    QString  reture_station;
    QString  school;

}S_PERINFO;

typedef struct S_RECORDINFO
{
    int id;
    QString  devId;
    int  recordType;
    int      fid;
    QString  name;
    QString  workId;
    QString  part;
    QString  icCard;
    QString  temperature;
    QString  recordTime;
    QString  pic;
    QString  gps;
    int      pGroup;
    int      cnt;
    int      status;
}S_RECORDINFO;

typedef struct S_NETINFO
{
    int  netMode;
    int  autoIp;
    QString  sIp;
    QString  sMask;
    QString  sGateWay;
    QString  sDns;
    QString  wifiSsid;
    QString  wifiPwd;
    QString  wifiMac;
    int      adhoc;
    int      wifiAuthMode;
    int      secMode;
    QString  APN;
    QString  user;
    QString  pwd;
    int      authMode;
    QString  serverIp;
    int      serverPort;
}S_NETINFO;

typedef struct S_FILEFLAG
{
    bool flag_up;
    bool flag_conf;
    bool flag_stand;
    bool flag_voice;
}S_FILEFLAG;

extern S_DEVPARINFO s_devparInfo;
extern S_DEVINFO    s_devInfo;
extern S_PERINFO    s_perInfo;
extern S_RECORDINFO s_recordInfo;
extern S_NETINFO    s_netInfo;
extern S_FILEFLAG   s_fileFlag;

/*更新翻译标志*/

extern  bool transFlag;

/*应答码*/
#define     OK              200
#define     ERROR           -255
#define     SUCCESS         "200"               /*成功*/
#define     DUPLICATE       -200                /*采集人脸返回数据重复*/
#define     NODEVICE        "-201"              /*设备不存在*/
#define     USERPICWRONG    "-202"              /*用户图片数据错误*/
#define     DATAINCOMP      "-203"              /*上送数据不完全*/
#define     SQLPROCWR0NG    "-204"              /*SQL数据库操作失败*/
#define     USERREPEAT      "-205"              /*用户重复*/
#define     DEVREPEAT       "-206"              /*设备重复*/
#define     NOITEM          "-207"              /*无记录*/
#define     DEVOFFLINE      "-208"              /*设备离线*/
#define     NETWRONG        "-209"              /*网络故障*/
#define     NOPERSON        "-210"              /*终端无人员*/
#define     INCOMPLETEDATA  "-211"              /*接收数据不完整*/

/*设备必须文件路径*/
#define     PIC             "/pic"
#define     DATA            "/data"
#define     UPGRADE         "/upgrade"
#define     OPENFILE        "/file"
#define     RECORDPIC       "/recordpic"
#define     LOG             "/log"

/*导出文件关键字*/
#define     USERNAME        "UserName:"
#define     WORKID          "WorkID:"
#define     FILENAME        "FileName:"
#define     CARD            "Card:"
#define     PASSTIME        "PassTime:"
#define     TEMPRATURE      "TEMP:"
#define     RECORDTIME      "RecordTime:"
#define     MODE            "Mode:"

/*最大数*/
#define MAXSIZE        (40*1024)// (51200)
#define MAXOVERTIME    (140)
#define MAXPERSON      (2000)


#define ONLYFACE        "2"
#define ONLYCARD        "1"
#define FACEANDCARD     "3"
#define FACEORCARD      "5"

#define T_TEMPURATURE           "4"
#define T_FACEANDTEMP           "2"
#define T_CARDANDTEMP           "1"
#define T_FACEANDCARDANDTEMP    "3"
#define T_ONLYFACE              "5"
#define T_ONLYCARD              "6"
#define T_FACEANDCARD           "7"


/*分页按钮类型*/
#define BUTTONSTART   0
#define BUTTONEND     1
#define BUTTONOFF     2
#define BUTTONOPEN    3


#define DEVVESION   "WGX_XB_FACE_001"

#define JPGPHOTOPATH "JpgPhoto/"




#endif // INCLUDE_H
