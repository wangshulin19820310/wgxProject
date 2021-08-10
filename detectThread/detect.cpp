#include "detect.h"


detect *detect::mydetect= nullptr;

detect *detect::Instance()
{
    if (!mydetect) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!mydetect) {
            mydetect = new detect;
        }
    }
    return mydetect;
}


detect::detect(QObject *parent) : QObject(parent)
{
    iThreshold = 70;
    mydetect = this;
}


detect::~detect()
{
    qDebug()<<"detect thread close";

}

void detect::threadStart()
{
   /*init detect parm*/
    ExternalApi::setRotationAngle(270);
    ExternalApi::setDisplayRect(0,0,720,1280);
    ExternalApi::setRecogThreshold(iThreshold);
    ExternalApi::setFace_Algorithm(FACE_YTLF);

#if 1
    comFunc comfunc;
    //ExternalApi::setYtlfLicenseConfig("113.100.143.90", "8393", "v4Y54q2eF5IdETzZ", "T4plqpGU8hcSZKGheakzNIELnnx19D");
    qDebug()<<"ip:"<<comfunc.stddToqstring(algoInfo.serverIp)<<"port:"<<comfunc.stddToqstring(algoInfo.serverPort);
    ExternalApi::setYtlfLicenseConfig(algoInfo.serverIp, algoInfo.serverPort, algoInfo.authKey, algoInfo.authSec);
#endif

    //从数据库载入所有人脸特征   
    getAllFeatrue();
    std::string error;
    ExternalApi::setIrLight(1, error);;

    //设置体温补偿
#if 0    
    ExternalApi::setTempCB(TempRecv, this);

    if (Singleton<DBOptEx>::GetInstance()->GetTempConfFormDB(m_fTempGain, m_fTempAlarm))
    {
        ExternalApi::setTempOffset(m_fTempGain);
    }
#endif
    //设置人脸回调
    ExternalApi::setRegisterFaceCB(FaceBoxRecv, UserInfoRecv, this);



    qDebug()<<"detect thread run";
}

void detect::closeThread(void)
{
    emit finished(0);
}



//人脸框信息接收，left，top,right，bottom是人脸框位置，都为0，则没人脸。
void detect::FaceBoxRecv(int left, int top, int right, int bottom, void* pObj)
{
    std::string error;  
    detect * pFaceLogic = (detect *)pObj;
    //判断是否有人脸
    bool bHasFace = false;
    if (0 == left && 0 == top && 0 == right && 0 == bottom)
    {

   //     std::lock_guard<std::mutex> lk(pFaceLogic->m_mtxFace);
        bHasFace = false;
        pFaceLogic->m_bFaceFaileSounded = false;
        pFaceLogic->m_bFaceSucessSounded = false;
        pFaceLogic->m_bMaskSounded = false;
        pFaceLogic->m_bTempAlarmSounded = false;
        pFaceLogic->m_bTempNormalSounded = false;
        pFaceLogic->m_sPassId = "";
        pFaceLogic->m_tBegin = time(nullptr);
    }
    else {
        qDebug()<<__func__<<__LINE__<<left<<top<<right<<bottom;
        bHasFace = true;
    }
    //向灯光控制设置是否有人脸
//	Singleton<LightLogic>::GetInstance()->setHasFace(bHasFace);

    //计算是否有人脸重复次数
    static int bPreHasFace = false;
    static int iCount = 0;//有无人脸重复次数iCount
    if (bPreHasFace == bHasFace)
    {
        iCount++;

    }
    else {
        bPreHasFace = bHasFace;
        iCount = 0;
        if((s_devInfo.lightType == 0) || (s_devInfo.lightType == 1))
        {
            emit mydetect->hasFace();

        }
    }

    //向GUI发送是否有人脸,最多重复发送5次；
    if (iCount < 10 && 0 == iCount % 2)
    {       
     //   qDebug()<<__func__<<__LINE__;
        bPreHasFace = false;
    }
}
    //人脸识别信息接收。

void detect::UserInfoRecv(S_UserInfo* pUserInfo, void* pObj)
{
    QStringList userInfo;
    comFunc mfunc;
    if(1)
    {
        //是否进行识别检测
        if(true != mydetect->m_iIsCheck)
        {
            return;
        }
   //     qDebug()<<"mydetect->m_bFaceSucessSounded11111111:"<<mydetect->m_bFaceSucessSounded;
  //      qDebug()<<"mydetect->m_bFaceFaileSounded:"<<mydetect->m_bFaceFaileSounded;
        //识别成功后不再识别处理
        if((mydetect->m_bFaceSucessSounded)||(mydetect->m_bFaceFaileSounded))
        {
            return;
        }

        int iIdentity = 0, iWeigen = 0, iMask = 0, iThreshold = 0;
//		Singleton<DBOptEx>::GetInstance()->GetPassConf(iIdentity, iWeigen, iMask, iThreshold);
        //没佩戴口罩播放声音提醒
        if (1==iMask&& 0 == pUserInfo->iMask&& !mydetect->m_bMaskSounded)
        {
            mydetect->m_bMaskSounded = true;
            ExternalApi::playAudio("/oem/Intellect/app/wav/mask_tip.wav");
        }
        std::string sId = std::to_string(pUserInfo->nId);
        QString sId1 = QString::fromStdString(sId);
        QString sName = "陌生人";
        QString sPhotoFile = "";
        bool bFecognition = true;
        QString sRecCheck = "人脸未注册";
    //    printf("[%s] %d  pUserInfo->nId[%llu] start shibie!!\n", __FUNCTION__, __LINE__, pUserInfo->nId);
        if (USER_STATE_REAL_REGISTERED == pUserInfo->state)//识别成功
        {
            //向mainWidget发送识别成功emit
             userInfo = mydetect->getUserInfo(pUserInfo->nId);
            sRecCheck = "人脸识别成功";
            //向GUI发送识别成功
            char cId[16] = { 0 };
            sprintf(cId, "%06llu", pUserInfo->nId);
            //打开闸机
            std::string sError;
    //        printf("[%s] %d  pUserInfo->nId[%llu] shibie ok3!!\n", __FUNCTION__, __LINE__, pUserInfo->nId);
            //播放声音
              if (!mydetect->m_bFaceSucessSounded)
            {
                ExternalApi::playAudio("/oem/Intellect/app/wav/distinguish.wav");
                mydetect->m_bFaceSucessSounded = true;
                emit mydetect->hasDetect();
            }
           printf("[%s] %d  pUserInfo->nId[%llu] shibie ok4!!\n", __FUNCTION__, __LINE__, pUserInfo->nId);

        }
        else
        {   //人脸未进行注册
            sRecCheck = "人脸未注册";
            bFecognition = false;
            if (!bFecognition)
            {
                //播放识别失败声音
                if (!mydetect->m_bFaceFaileSounded)
                {
                    ExternalApi::playAudio("/oem/Intellect/app/wav/distinguish_fail.wav");
                    mydetect->m_bFaceFaileSounded = true;
                    emit mydetect->hasDetect();
                    qDebug()<<"mydetect->m_bFaceFaileSounded:"<<mydetect->m_bFaceFaileSounded;
                }
            }

        }
     //   printf("[%s] %d  pUserInfo->nId[%llu] start save to db!!\n", __FUNCTION__, __LINE__, pUserInfo->nId);
        if(USER_STATE_REAL_REGISTERED == pUserInfo->state|| false==bFecognition)
        {
            //现场照片jpg文件
            std::string sJpgFile = "";

            QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
            std::string sPassId = timestamp.toStdString();

            if(pUserInfo->nId == 0)
            {
                 sJpgFile = JPGPHOTOPATH + sPassId + ".jpg";
            }
            else
            {
                 QString id = QString::number(pUserInfo->nId);

                 sJpgFile = JPGPHOTOPATH + mfunc.qstringTostd(id) + sPassId + ".jpg";
            }

            //生成现场照片jpg文件
            if (!ExternalApi::convertToPicture(pUserInfo, sJpgFile,ImgJpg))
            {
                sJpgFile = "";
            }


            printf("ready to dis pic11111111111111111111 path:%s\n",sJpgFile.c_str());
            /*send to UI display*/
         //   if(true == bFecognition)
            {
                comFunc mfunc;
                emit mydetect->sendtoMain("1123",mfunc.stddToqstring(sJpgFile));
            }

            /*save to db*/
            if (USER_STATE_REAL_REGISTERED == pUserInfo->state)
            {
                S_RECORDINFO s_recordInfo;

                s_recordInfo.recordType = 0;
                s_recordInfo.fid = pUserInfo->nId;
                s_recordInfo.name = userInfo.at(0);
                s_recordInfo.workId = "";
                s_recordInfo.part = "";
                s_recordInfo.icCard = "";
                s_recordInfo.temperature = "0";
                s_recordInfo.recordTime = timestamp;
                s_recordInfo.pic = mfunc.stddToqstring(sJpgFile);
                s_recordInfo.gps = "";
                s_recordInfo.pGroup = 0;
                s_recordInfo.status = 0;
                mydetect->saveRecord(&s_recordInfo);

            }
        }
     //   printf("[%s] %d  pUserInfo->nId[%d] end save to db!!\n", __FUNCTION__, __LINE__, pUserInfo->nId);

    }
}



void detect::TempRecv(float fValue, std::string sErr, void* pObj)
{
    char cValue[32] = { 0 };
    sprintf(cValue, "%.02f", fValue);
    detect* pFaceLogic = (detect*)pObj;

    static int iCount = 0;
    if (fValue <= 0)
    {
        //10次以上为0，认为没有体温；
        if (++iCount >= 10)
        {
            //向GUI发送没有体温
    //		Singleton<GuiTcpSrv>::GetInstance()->sendTemperature(cValue, std::to_string(pFaceLogic->m_fTempAlarm));
            iCount = 0;
        }
        return;
    }
    else {
        iCount = 0;
    }
    //是否进行识别检测
    if (true != pFaceLogic->m_iIsCheck)
    {
        return;
    }

    if (pFaceLogic)
    {
        std::string sPassId =pFaceLogic->m_sPassId;

        if (sPassId.length() > 0)
        {
    //        std::lock_guard<std::mutex> lk(pFaceLogic->m_mtxFace);
            QString sRecCheck = "温度正常";
            if (fValue >= pFaceLogic->m_fTempAlarm)//超过告警线状态
            {
                //向GUI发送异常体温
                //播放声音
                if (!pFaceLogic->m_bTempAlarmSounded)
                {
                    ExternalApi::playAudio("/oem/Intellect/app/wav/tiwen_abnormal.wav");
                    pFaceLogic->m_bTempAlarmSounded = true;
                    pFaceLogic->m_bTempNormalSounded = false;//温度告警可以一直检测
                }
                sRecCheck = "温度异常";
            }
            else { //温度正常
                //温度正常后不再处理
                if (pFaceLogic->m_bTempNormalSounded)
                {
                    return;
                }
                //向GUI发送正常体温

                //播放声音
                if (!pFaceLogic->m_bTempNormalSounded)
                {
                    ExternalApi::playAudio("/oem/Intellect/app/wav/tiwen_normal.wav");
                    pFaceLogic->m_bTempNormalSounded = true;
                }
            }
    //		Singleton<GuiTcpSrv>::GetInstance()->sendTemperature(cValue, std::to_string(pFaceLogic->m_fTempAlarm));
    //		Singleton<DBOptEx>::GetInstance()->TempRecordUpdate(sPassId, cValue, sRecCheck);
        }
    }
}


/*在这里直接调用API*/
void detect::getAllFeatrue()
{
    QString str;
    QSqlDatabase db;
    if(QSqlDatabase::contains("no1"))
    {
        db = QSqlDatabase::database("no1");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE","no1");
        db.setDatabaseName(qApp->applicationDirPath()+"/datadb/face.db");
    }

    if (!db.open())
    {
        qDebug()<<"datadb is no exist";
    }

    str = QString("SELECT fid,feature FROM person");
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
    {
        qDebug()<<query2.lastError()<<__func__<<__LINE__;
        return ;
    }
    while(query2.next() == true)
    {
        /*0:pid,1:feature*/

        qDebug()<<query2.value(0).toInt()<<QString::number(query2.value(1).toByteArray().length());
        std::string Error;
        ExternalApi::loadFeatureInfo(query2.value(1).toByteArray().data(), query2.value(1).toByteArray().length(), query2.value(0).toInt(), Error);
    }
    query2.clear();
    db.close();
}


QStringList detect::getUserInfo(qint64 pid)
{
    QStringList out;
    QString str;
    QSqlDatabase db;
    if(QSqlDatabase::contains("no1"))
    {
        db = QSqlDatabase::database("no1");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE","no1");
        db.setDatabaseName(qApp->applicationDirPath()+"/datadb/face.db");
    }

    if (!db.open())
    {
        qDebug()<<"datadb is no exist";
    }

    str = QString("SELECT name,pic FROM person where fid='%1'").arg(pid);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
    {
        qDebug()<<query2.lastError()<<__func__<<__LINE__;
        return out;
    }

    while(query2.next() == true)
    {
        /*0:fid,1:name,2:pic*/
        qDebug()<<query2.value(0).toString()<<query2.value(1).toString();
        out.append(query2.value(1).toString());
        out.append(query2.value(2).toString());
    }
    query2.clear();
    db.close();
    return out;
}

void detect::init(QString ip,QString port,QString key,QString secret,int threshold)
{
    iThreshold = threshold;
    comFunc comfunc;
    algoInfo.serverIp = comfunc.qstringTostd(ip);
    algoInfo.serverPort = comfunc.qstringTostd(port);
    algoInfo.authKey = comfunc.qstringTostd(key);
    algoInfo.authSec = comfunc.qstringTostd(secret);

}

void detect::isCheck(bool flag)
{
    m_iIsCheck = flag;
}



bool detect::saveRecord(S_RECORDINFO *s_recordInfo)
{
    QString str;

    QSqlDatabase db;
    if(QSqlDatabase::contains("no1"))
    {
        db = QSqlDatabase::database("no1");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE","no1");
        db.setDatabaseName(qApp->applicationDirPath()+"/datadb/face.db");
    }

    if (!db.open())
    {
        qDebug()<<"datadb is no exist";
    }

    str = QString("SELECT * FROM %1 where %2 = %3").arg("devRecord").arg("fid").arg(s_recordInfo->fid);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
    {
        qDebug()<<query2.lastError();
        return false;
    }

    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query2);
    while(queryModel->canFetchMore())
    {
        queryModel->fetchMore();
    }
    s_recordInfo->cnt = queryModel->rowCount();
    delete queryModel;
    query2.clear();

    s_recordInfo->cnt++;
    query2.prepare("insert into devRecord (devId,recordType,fid,name,temperature,recordTime,pic,gps,pGroup,cnt,status) values (?,?,?,?,?,?,?,?,?,?,?)");
    query2.addBindValue(s_recordInfo->devId);
    query2.addBindValue(s_recordInfo->recordType);
    query2.addBindValue(s_recordInfo->fid);
    query2.addBindValue(s_recordInfo->name);
    query2.addBindValue(s_recordInfo->temperature);
    query2.addBindValue(s_recordInfo->recordTime);
    query2.addBindValue(s_recordInfo->pic);
    query2.addBindValue(s_recordInfo->gps);
    query2.addBindValue(s_recordInfo->pGroup);
    query2.addBindValue(s_recordInfo->cnt);
    query2.addBindValue(s_recordInfo->status);
    if(!query2.exec())
    {
       qDebug()<<query2.lastError();
       return false;
    }
    query2.clear();
    return true;
}



