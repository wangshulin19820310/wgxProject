#ifndef DETECT_H
#define DETECT_H
#include <QObject>
#include "include.h"
#include "../include/ExternalApi.h"
#include "./include/defineExt.h"
#include "comFunc/comfunc.h"

class detect : public QObject
{
    Q_OBJECT
public:
    explicit detect(QObject *parent = nullptr);
    ~detect();

    int iThreshold;
    bool m_bMaskSounded = false;//口罩识别是否已播放声音;禁止重复播放
    bool m_bFaceSucessSounded = false;//人脸识别成功是否已播放声音;禁止重复播放
    bool m_bFaceFaileSounded = false;//人脸识别失败是否已播放声音;禁止重复播放
    bool m_bTempNormalSounded = false;//体温正常是否已播放声音;禁止重复播放
    bool m_bTempAlarmSounded = false;//体温过高是否已播放声音;禁止重复播放
    std::string m_sPassId = "";//通行id
    time_t m_tBegin = time(nullptr);//识别超过1秒中则识别失败
    float m_fTempGain; //温度补偿
    float m_fTempAlarm; //温度告警上限
    bool m_iIsCheck = true; //是否进行识别检测
  //  std::mutex m_mtxFace;//人脸识别互斥体

    typedef struct {
            std::string serverIp;
            std::string serverPort;
            std::string authKey;
            std::string authSec;
    }algorithmInfo;
    algorithmInfo algoInfo;
    void getAllFeatrue();
    QStringList getUserInfo(qint64 pid);
    void init(QString ip,QString port,QString key,QString secret,int threshold);
    static detect *Instance();
    bool saveRecord(S_RECORDINFO *s_recordInfo);


signals:
    void finished(int);
    void sendtoMain(QString,QString);
    void hasFace();
    void hasDetect();

public slots:
    void closeThread(void);
    void threadStart();
    void isCheck(bool);
    void createFeature(const char *picPath);



private:
    //人脸框信息接收，left，top,right，bottom是人脸框位置，都为0，则没人脸。
    static void FaceBoxRecv(int left, int top, int right, int bottom, void* pObj);
    //人脸识别信息接收。
    static void UserInfoRecv (S_UserInfo* pUserInfo, void* pObj);
    //体温信息接收
    static void TempRecv(float fValue, std::string sErr, void* pObj);
    static detect *mydetect;


};

#endif // DETECT_H
