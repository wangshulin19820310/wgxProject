#include "facedb.h"

S_DEVPARINFO s_devparInfo;
S_NETINFO    s_netInfo;
S_DEVINFO    s_devInfo;
S_PERINFO    s_perInfo;


faceDb *faceDb::self = nullptr;
faceDb *faceDb::Instance()
{
    if (!self) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new faceDb;
        }
    }
    return self;
}

faceDb::faceDb()
{
    bool ret = false;
    QString tablename;
    QMessageBox megboxWrong;
    ret = initDb();

    if(ret == false)
    {
        goto Error;
    }


    ret = checkTableIsExist(TABLE_DEVPAR);
    if(ret == false)
    {      
        tablename +="CREATE TABLE ";
        tablename +=TABLE_DEVPAR;
        tablename +=
        "("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "version VARCHAR(40), "
        "devName VARCHAR(40), "
        "productKey VARCHAR(40), "
        "devSecret VARCHAR(40),"
        "OSSDomain VARCHAR(64),"
        "OSSKeyId VARCHAR(64),"
        "OSSSec VARCHAR(64))";
        qDebug()<<"devPar table:"<<tablename;
        ret = query.exec(tablename);
        if(ret == false)
        {
            qDebug()<<"devPar err:"<<query.lastError().text();
            goto Error;

        }
        //QString insert_sql = "insert into devPar (version, devName,productKey,devSecret) values ('001', 'newdev', 'a1vperyb2Cg','jqSbrJZ11baH1aAH')";
        QString insert_sql = "insert into devPar (version, devName) values ('001', 'newdev')";
        ret = query.exec(insert_sql);
        if(ret == false)
        {
            qDebug()<<"insert devPar err:"<<query.lastError().text();
            goto Error;

        }
    }

    ret = checkTableIsExist(TABLE_DEVS);
    if(ret == false)
    {        
        tablename.clear();
        tablename +="CREATE TABLE ";
        tablename +=TABLE_DEVS;
        tablename +=
        "("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "devId VARCHAR(40) ,"
        "chepai VARCHAR(40) ,"
        "workMode INTEGER ,"
        "delayOff INTEGER ,"
        "leftDetect INTEGER ,"
        "standbyTime INTEGER ,"
        "faceInterval INTEGER ,"
        "clearTime   INTEGER ,"
        "shxMode   INTEGER ,"
        "volume   INTEGER ,"
        "faceSize   INTEGER ,"
        "uploadType   INTEGER ,"
        "natStatus INTEGER,"
        "timeZone INTEGER,"
        "temperature INTEGER,"
        "noReg INTEGER,"
        "lightType INTEGER,"
        "personLimit INTEGER,"
        "setTts INTEGER,"
        "syncTimer INTEGER)";
        qDebug()<<"devconf table:"<<tablename;
        ret = query.exec(tablename);
        if(ret == false)
        {
            qDebug()<<"devconf err:"<<query.lastError().text();
            goto Error;

        }
        QString insert_sql = "insert into devConf (devid, chepai,workMode,delayOff,leftDetect,faceInterval,clearTime,shxMode,volume,faceSize,uploadType,natStatus,timeZone,temperature,noReg,lightType,personLimit)"
                             " values "
                                                 "('000000', '0000000', 0,10,20,3,30,0,80,65535,1,0,8,0,1,0,2000)";
        ret = query.exec(insert_sql);
        if(ret == false)
        {
            qDebug()<<"insert devConf err:"<<query.lastError().text();
            goto Error;

        }
    }


    ret = checkTableIsExist(TABLE_PERSON);
    if(ret == false)
    {        
        tablename.clear();
        tablename +="CREATE TABLE ";
        tablename +=TABLE_PERSON;
        tablename +=
        "("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "fid INTEGER ,"
        "name VARCHAR(40) ,"        
        "part VARCHAR(40),"
        "workId VARCHAR(40) ,"
        "icCard VARCHAR(40) , "
        "level VARCHAR(40) , "
        "passWord VARCHAR(40) , "
        "pic VARCHAR(256),"
        "certType VARCHAR(10),"
        "regTime VARCHAR(40),"
        "effectTime VARCHAR(40),"
        "feature BLOB,"
        "pGroup INTEGER,"
        "go_station VARCHAR(40),"
        "return_station VARCHAR(40),"
        "school VARCHAR(40)"
        ")";
        qDebug()<<"person table:"<<tablename;
        ret = query.exec(tablename);
        if(ret == false)
        {
            qDebug()<<"person err:"<<query.lastError().text();
            goto Error;

        }

    }

    ret = checkTableIsExist(TABLE_RECORD);
    if(ret == false)
    {        
        tablename.clear();
        tablename +="CREATE TABLE ";
        tablename +=TABLE_RECORD;
        tablename +=
        "("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "devId VARCHAR(40) ,"
        "recordType INTEGER ,"
        "fid INTEGER ,"        
        "name VARCHAR(40) ,"
        "workId VARCHAR(40) ,"
        "part VARCHAR(40),"
        "icCard VARCHAR(40) , "
        "temperature VARCHAR(40) , "
        "recordTime VARCHAR(40), "
        "pic VARCHAR(256),"
        "gps VARCHAR(256),"
        "pGroup INTEGER,"
        "cnt INTEGER,"
        "status INTEGER"
        ")";
        qDebug()<<"record table:"<<tablename;
        ret = query.exec(tablename);
        if(ret == false)
        {
            qDebug()<<"record err:"<<query.lastError().text();
            goto Error;

        }
    }  


    ret = checkTableIsExist(TABLE_NET);
    if(ret == false)
    {

        tablename.clear();
        tablename +="CREATE TABLE ";
        tablename +=TABLE_NET;
        tablename +=
        "("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "netMode INTEGER, "
        "autoIP INTEGER, "
        "sIp VARCHAR(40),"
        "sMask VARCHAR(40),"
        "sGateWay VARCHAR(40),"
        "sDns VARCHAR(40),"
        "wifiSsid VARCHAR(40),"
        "wifiPwd VARCHAR(40),"
        "wifiMac VARCHAR(40),"
        "adhoc INTEGER,"
        "wifiAuthMode INTEGER,"
        "secMode INTEGER,"
        "APN VARCHAR(40),"
        "user VARCHAR(40),"
        "pwd VARCHAR(40),"
        "authMode INTEGER,"
        "serverIp VARCHAR(40),"
        "serverPort INTEGER"
        ") ";
         qDebug()<<"netConf table:"<<tablename;
        ret = query.exec(tablename);
        if(ret == false)
        {
            qDebug()<<"err:"<<query.lastError().text();
            goto Error;
        }
        QString insert_sql = "insert into netConf (netMode, wifiSsid,wifiPwd,wifiMac,adhoc,wifiAuthMode,secMode,APN,User,Pwd,authMode)"
                             " values "
                             "(2, 'TP-LINK_12DD','tan7758520','f4:83:cd:59:12:dd',0,7,16,'','','',0)";
        qDebug()<<"netConf:"<<insert_sql;
        ret = query.exec(insert_sql);
        if(ret == false)
        {
            qDebug()<<"insert netConf err:"<<query.lastError().text();
            goto Error;

        }


    }

#if 0
    ret = checkColumnExist(TABLE_DEVS,K_DEVINFO);
    if(ret == false)
    {
        ret = addColumn(TABLE_DEVS,K_DEVINFO,COLUMNTYPE);
        if(ret == false)
        {
            qDebug()<<"create column err!";
            goto Error;
        }
    }

    ret = checkColumnExist(TABLE_SYSTEMPAR,K_WORKIDSORT);
    if(ret == false)
    {
        ret = addColumn(TABLE_SYSTEMPAR,K_WORKIDSORT,COLUMNTYPE_10);
        if(ret == false)
        {
            qDebug()<<"create column err!";
            goto Error;
        }
    }

    ret = checkColumnExist(TABLE_SYSTEMPAR,K_RECORDCONDITION);
    if(ret == false)
    {
        ret = addColumn(TABLE_SYSTEMPAR,K_RECORDCONDITION,COLUMNTYPE_10);
        if(ret == false)
        {
            qDebug()<<"create column err!";
            goto Error;
        }
    }

    ret = checkColumnExist(TABLE_SYSTEMPAR,K_RECORDSORT);
    if(ret == false)
    {
        ret = addColumn(TABLE_SYSTEMPAR,K_RECORDSORT,COLUMNTYPE_10);
        if(ret == false)
        {
            qDebug()<<"create column err!";
            goto Error;
        }
    }
#endif
    tableList<<TABLE_DEVPAR<<TABLE_DEVS<<TABLE_PERSON<<TABLE_RECORD<<TABLE_NET;

    getDevPar();
    getDevConf();
    getNetConf();

    return;
Error:
    megboxWrong.setText(QObject::tr("数据库加载失败,请查看文件是否齐全。"));
    megboxWrong.setWindowTitle(QObject::tr("出错"));
    megboxWrong.setIcon(QMessageBox::Information);
   // QPushButton(megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole));
    megboxWrong.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    megboxWrong.exec();


}

bool faceDb::initDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE","no0");
    db.setDatabaseName(qApp->applicationDirPath()+"/datadb/face.db");
     if (!db.open())
     {         
         qDebug()<<"datadb is no exist";
     }
     query=QSqlQuery(db);
     return true;
}

bool faceDb::checkTableIsExist(const char *tableName)
{
    QString tr="SELECT * FROM ";
    tr +=tableName;    
    bool result = query.exec(tr);
    if(result == false)
    {
       return false;
    }
    else
    {
        return true;
    }
}

bool faceDb::checkColumnExist(const char *tableName,const char *column)
{
    bool result = query.exec(QString("select %1 from %2").arg(column).arg(tableName));
    if(result == false)
    {
       return false;
    }
    else
    {
        return true;
    }
}


bool faceDb::addColumn(const char *tableName,const char *column,const char *type)
{
    bool result = query.exec(QString("alter table %1 add %2 %3").arg(tableName).arg(column).arg(type));
    if(result == false)
    {
       return false;
    }
    else
    {
        return true;
    }
}

bool faceDb::selectTable(e_tableDefine operation)
{
    QString oper;
    switch (operation)
    {
    case E_TABLE_SYSTEMPAR:
    {
        oper = "select userName, passWord,ipAddr, port, language ,filePath,workIdSort,recordCondition,recordSort from systemParTable";
        QSqlQuery query(oper,db);        
        while (query.next())
        {
#if 0
            s_systemInfo.userName = query.value(0).toString();
            s_systemInfo.psword = query.value(1).toString();
            s_systemInfo.ip = query.value(2).toString();
            s_systemInfo.port = query.value(3).toInt();
            s_systemInfo.language = query.value(4).toString();
            s_systemInfo.filePath = query.value(5).toString();
            s_systemInfo.workIdSort = query.value(6).toString();
            s_systemInfo.recordCondition = query.value(7).toString();
            s_systemInfo.recordSort = query.value(8).toString();
#endif
        }
     }
        break;

     default:       
        break;

    }
    return true;
}

QString faceDb::selectAndGet(QString tableName, QStringList keyword, QStringList value, QStringList *datalist)
{

    int i,snums=0,index = 0;
    QString operation;
    for(i = 0; i< tableList.size();i++)
    {
        if(tableName.compare(tableList.at(i)) == 0)
        {
            index = i+1;
            break;
        }
    }
    switch(index)
    {
        case E_TABLE_DEVS:
           {
            operation +="select *";
            operation +=" from ";
            operation +=tableName;
            operation +=" where ";
            operation +=keyword.at(0);
            operation +="=";
            operation +="\'";
            operation +=value.at(0);
            operation +="\'";
            if(!query.exec(operation))
            {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
            }

            bool ret = true;
            while (ret)
            {
                ret = query.next();
                if(ret == true)
                {
                     /*qDebug()<<QString("查询结果：%1,%2,%3,%4,%5")
                              .arg(query.value(0).toString())
                              .arg(query.value(1).toString())
                              .arg(query.value(2).toString())
                              .arg(query.value(3).toString())
                              .arg(query.value(10).toString());
                     */
                    return DEVREPEAT;
                }
                else
                {
                    break;
                }
              }
            }
        break;

        case E_TABLE_PERSON:
        {
         operation +="select *";
         operation +=" from ";
         operation +=tableName;
         operation +=" where ";
         operation +=keyword.at(0);
         operation +="=";
         operation +="\'";
         operation +=value.at(0);
         operation +="\'";
        if(!query.exec(operation))
        {
            qDebug()<<query.lastError();
            return SQLPROCWR0NG;
        }

         bool ret = true;
         while (ret)
         {
             ret = query.next();
             if(ret == true)
             {
                 /*qDebug()<<QString("查询结果：%1,%2,%3,%4,%5")
                           .arg(query.value(0).toString())
                           .arg(query.value(1).toString())
                           .arg(query.value(2).toString())
                           .arg(query.value(3).toString())
                           .arg(query.value(6).toString());
                */
                 datalist->append(query.value(1).toString());                 
                 datalist->append( query.value(3).toString());
                 datalist->append( query.value(4).toString());
                 datalist->append( query.value(6).toString());
                 snums++;
             }
             else
             {
                 if(snums == 0)
                    {
                     return NOITEM;
                    }
                 else
                 {
                     return SUCCESS;
                 }
             }
           }
         }
        break;

        default:
        break;

    }
    return SUCCESS;

}



QString faceDb::selectAndUpdate(QString tableName,e_operDefine oper,QStringList keyword,QStringList value,QStringList datalist)
{
    int i,snums=0,index = 0;
    QString operation;
    for(i = 0; i< tableList.size();i++)
    {
        if(tableName.compare(tableList.at(i)) == 0)
        {
            index = i+1;
            break;
        }
    }
    switch(index)
    {
        case E_TABLE_SYSTEMPAR:
           {
                operation +="update ";
                operation +=tableName;
                operation +=" set ";
                operation +="passWord";
                operation +="=";
                operation +="\'";
                operation +=datalist.at(0)+"\', ";
                operation +="ipAddr";
                operation +="=";
                operation +="\'";
                operation +=datalist.at(1)+"\', ";
                operation +="port";
                operation +="=";
                operation +=datalist.at(2);
                operation +=", language";
                operation +="=";
                operation +="\'";
                operation +=datalist.at(3)+"\' ";
                operation +=", filePath";
                operation +="=";
                operation +="\'";
                operation +=datalist.at(4)+"\' ";
                operation +=" where ";
                operation +=keyword.at(0);
                operation +="=";
                operation +="\'";
                operation +=value.at(0);
                operation +="\'";
                if(!query.exec(operation))
                {
                    qDebug()<<query.lastError();
                    return SQLPROCWR0NG;
                }
         }
        break;

        case E_TABLE_DEVS:
          if(oper == E_MULTI)
           {
            operation +="select *";
            operation +=" from ";
            operation +=tableName;
            operation +=" where ";
            operation +=keyword.at(0);
            operation +="=";
            operation +="\'";
            operation +=value.at(0);
            operation +="\'";
            if(!query.exec(operation))
            {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
            }
            bool ret = true;
            while (ret)
            {
                ret = query.next();            
                if(ret == true)
                {
                    /*
                    qDebug()<<QString("查询结果：%1,%2,%3,%4,%5")
                              .arg(query.value(0).toString())
                              .arg(query.value(1).toString())
                              .arg(query.value(2).toString())
                              .arg(query.value(3).toString())
                              .arg(query.value(10).toString());
                              */
                    operation.clear();
                    operation +="update ";
                    operation +=tableName;
                    operation +=" set ";

                    operation +="devIp";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(0)+"\', ";

                    operation +="devPort";
                    operation +="=";                  
                    operation +=datalist.at(1)+", ";

                    operation +="gateWay";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(2)+"\', ";

                    operation +="macAddr";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(3)+"\', ";

                    operation +="devInOut";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(4)+"\', ";

                    operation +="devStatus";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(5)+"\', ";

                    operation +="devType";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(6)+"\', ";

                    operation +="devVer";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(7)+"\', ";

                    operation +="passType";
                    operation +="=";
                    operation +="\'";
                    operation +=datalist.at(8)+"\'";

                    operation +=" where ";
                    operation +=keyword.at(0);
                    operation +="=";
                    operation +="\'";
                    operation +=value.at(0);
                    operation +="\'";
                    if(!query.exec(operation))
                    {
                        qDebug()<<query.lastError();
                        return SQLPROCWR0NG;
                    }
                    snums++;
                }
                else
                {
                    if(snums == 0)
                    {
                        return NODEVICE;
                    }
                }
              }
            }
          else
          {
              operation +="select *";
              operation +=" from ";
              operation +=tableName;

              operation +=" where ";
              operation +=keyword.at(0);
              operation +="=";
              operation +="\'";
              operation +=value.at(0);
              operation +="\'";            
           // qDebug()<<operation;
            if(!query.exec(operation))
            {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
            }

              bool ret = true;
              while (ret)
              {
                  ret = query.next();

                  if(ret == true)
                  {
                      /*
                      qDebug()<<QString("查询结果：%1,%2,%3,%4,%5")
                                .arg(query.value(0).toString())
                                .arg(query.value(1).toString())
                                .arg(query.value(2).toString())
                                .arg(query.value(3).toString())
                                .arg(query.value(10).toString());
                      */
                      operation.clear();
                      operation +="update ";
                      operation +=tableName;
                      operation +=" set ";

                      operation +="devStatus";
                      operation +="=";
                      operation +="\'";
                      operation +=datalist.at(0)+"\' ";

                      operation +=" where ";
                      operation +=keyword.at(0);
                      operation +="=";
                      operation +="\'";
                      operation +=value.at(0);
                      operation +="\'";                 
                   // qDebug()<<operation;
                    if(!query.exec(operation))
                    {
                        qDebug()<<query.lastError();
                        return SQLPROCWR0NG;
                    }
                      snums++;
                  }
                  else
                  {
                      if(snums == 0)
                      {
                          return NODEVICE;
                      }
                  }
                }
          }
        break;

        case E_TABLE_PERSON:
            {
                operation +="select *";
                operation +=" from ";
                operation +=tableName;
                operation +=" where ";
                operation +=keyword.at(0);
                operation +="=";
                operation +="\'";
                operation +=value.at(0);
                operation +="\'";
                operation +=" and ";
                operation +=K_DEVSN;
                operation +="=";
                operation +="\'";
                operation +=value.at(1);
                operation +="\'";

                //qDebug()<<operation;
                if(!query.exec(operation))
                {
                    qDebug()<<query.lastError();
                    return SQLPROCWR0NG;
                }
                if(query.next() == true)
                {                    
                    if(oper == E_EDITE)
                    {
                        QFile::remove(query.value(8).toString());
                    }
                    operation = QString("update person set name=%1,part=%2,icCard=%3,pic=%4 where workId=%5 ").arg("'"+datalist.at(1)+"'").arg("'"+datalist.at(2)+"'").arg("'"+datalist.at(3)+"'").arg("'"+datalist.at(4)+"'").arg("'"+datalist.at(0)+"'");
                    //qDebug()<<operation;
                    if(!query.exec(operation))
                    {
                        qDebug()<<query.lastError();
                        return SQLPROCWR0NG;
                    }                    
                }
                else
               {
                    operation.clear();
                    operation +="insert into ";
                    operation +=tableName;
                    operation +=" (";                 
                    operation +=K_WORKID;
                    operation +=",";
                    operation +=K_NAME;
                    operation +=",";
                    operation +=K_PART;
                    operation +=",";
                    operation +=K_ICCARD;
                    operation +=",";
                    operation +=K_PIC;
                    operation +=",";
                    operation +=K_DEVSN;
                    operation +=")";

                    operation +=" values ";
                    operation +=" (";
                    operation +="\'"+datalist.at(0)+"\'";
                    operation +=",";
                    operation +="\'"+datalist.at(1)+"\'";
                    operation +=",";
                    operation +="\'"+datalist.at(2)+"\'";
                    operation +=",";
                    operation +="\'"+datalist.at(3)+"\'";
                    operation +=",";
                    operation +="\'"+datalist.at(4)+"\'";
                    operation +=",";
                    operation +="\'"+datalist.at(5)+"\'";                  
                    operation +=")";

                    //qDebug()<<operation;
                    if(!query.exec(operation))
                    {
                        qDebug()<<query.lastError();
                        return SQLPROCWR0NG;
                    }
               }
        }
        break;


        case E_TABLE_RECORD:
        if(oper == E_MULTI)
         {
#if 0
            query.prepare("select * from devRecord where workId=? and recordTime=?");
            query.addBindValue(QString("'"+value.at(0)+"'"));
            //qDebug()<<"sn:"<<value.at(0)<<"recordSn:"<<value.at(1).toInt();
            query.addBindValue(QString("'"+value.at(1)+"'"));
            if(!query.exec())
            {
                qDebug()<<"select:"<<query.lastError();
                return SQLPROCWR0NG;
            }
#else
            operation = QString("select * from devRecord where workId=%1 and recordTime=%2").arg("'"+value.at(0)+"'").arg("'"+value.at(1)+"'");
            //qDebug()<<operation;
            if(!query.exec(operation))
            {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
            }

#endif
            if(query.next() == true)
            {
#if 0
                query.prepare("update devRecord set recordType=?,workId=?,name=?,part=?,"
                              "icCard=?,temperature=?,recordTime=? where devSn=? and recordSn=?");

                query.addBindValue(datalist.at(1));
                query.addBindValue(datalist.at(3));
                query.addBindValue(datalist.at(4));
                query.addBindValue(datalist.at(5));
                query.addBindValue(datalist.at(6));
                query.addBindValue(datalist.at(7));
                query.addBindValue(datalist.at(8));                
                query.addBindValue(value.at(0));
                query.addBindValue(value.at(1).toInt());
                if(!query.exec())
                {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
                }
#else
                operation = QString("update devRecord set devSn=%1,recordType=%2,workId=%3,name=%4,part=%5,icCard=%6,temperature=%7,recordTime=%8,pic=%9 where workId=%10 and recordTime=%11").arg("'"+datalist.at(0)+"'").arg("'"+datalist.at(1)+"'").arg("'"+datalist.at(3)+"'").arg("'"+datalist.at(4)+"'").arg("'"+datalist.at(5)+"'").arg("'"+datalist.at(6)+"'").arg("'"+datalist.at(7)+"'").arg("'"+datalist.at(8)+"'").arg("'"+datalist.at(9)+"'").arg("'"+value.at(0)+"'").arg("'"+value.at(1)+"'");
              //  qDebug()<<operation;

                if(!query.exec(operation))
                {
                    qDebug()<<query.lastError();
                    return SQLPROCWR0NG;
                }
#endif
            }
            else
            {              
                 query.prepare("insert into devRecord (devSn,recordType,recordSn,workId,name,part,icCard,temperature,recordTime,pic) values (?,?,?,?,?,?,?,?,?,?)");
                 query.addBindValue(datalist.at(0));
                 query.addBindValue(datalist.at(1));
                 query.addBindValue(datalist.at(2).toInt());
                 query.addBindValue(datalist.at(3));
                 query.addBindValue(datalist.at(4));
                 query.addBindValue(datalist.at(5));
                 query.addBindValue(datalist.at(6));
                 query.addBindValue(datalist.at(7));
                 query.addBindValue(datalist.at(8));
                 query.addBindValue(datalist.at(9));
                 if(!query.exec())
                 {
                 qDebug()<<query.lastError();
                 return SQLPROCWR0NG;
                 }

           }
        }
        else
        {
#if 0
            query.prepare("select * from devRecord where workId=? and recordTime=?");
            query.addBindValue(value.at(0));
            query.addBindValue(value.at(1));
            if(!query.exec())
            {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
            }
#else
            operation = QString("select * from devRecord where workId=%1 and recordTime=%2").arg("'"+value.at(0)+"'").arg("'"+value.at(1)+"'");
            if(!query.exec(operation))
            {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
            }

#endif
            if(query.next() == true)
            {

#if 0
                query.prepare("update devRecord set recordType=?,workId=?,name=?,part=?,icCard=?,temperature=?,recordTime=?,pic=?  where workId=? and recordTime=?");

                query.addBindValue(datalist.at(1));
                query.addBindValue(datalist.at(3));
                query.addBindValue(datalist.at(4));
                query.addBindValue(datalist.at(5));
                query.addBindValue(datalist.at(6));
                query.addBindValue(datalist.at(7));
                query.addBindValue(datalist.at(8));
                query.addBindValue(datalist.at(9));
                query.addBindValue(value.at(0));
                query.addBindValue(value.at(1).toInt());
                if(!query.exec())
                {
                    qDebug()<<"333333";
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
                }
#else

                qDebug()<<QString("查询重复结果：%1,%2,%3,%4,%5,%6,%7")
                          .arg(query.value(0).toString())
                          .arg(query.value(1).toString())
                          .arg(query.value(2).toString())
                          .arg(query.value(3).toString())
                          .arg(query.value(4).toString())
                          .arg(query.value(5).toString())
                          .arg(query.value(9).toString());

                operation = QString("update devRecord set recordType=%1,workId=%2,name=%3,part=%4,icCard=%5,temperature=%6,recordTime=%7 where workId=%8 and recordTime=%9").arg("'"+datalist.at(1)+"'").arg("'"+datalist.at(3)+"'").arg("'"+datalist.at(4)+"'").arg("'"+datalist.at(5)+"'").arg("'"+datalist.at(6)+"'").arg("'"+datalist.at(7)+"'").arg("'"+datalist.at(8)+"'").arg("'"+value.at(0)+"'").arg("'"+value.at(1)+"'");
                if(!query.exec(operation))
                {
                    qDebug()<<query.lastError();
                    return SQLPROCWR0NG;
                }
#endif
            }
            else
            {
                 query.prepare("insert into devRecord (devSn,recordType,recordSn,workId,name,part,icCard,temperature,recordTime,pic) values (?,?,?,?,?,?,?,?,?,?)");
                 query.addBindValue(datalist.at(0));
                 query.addBindValue(datalist.at(1));
                 query.addBindValue(datalist.at(2).toInt());
                 query.addBindValue(datalist.at(3));
                 query.addBindValue(datalist.at(4));
                 query.addBindValue(datalist.at(5));
                 query.addBindValue(datalist.at(6));
                 query.addBindValue(datalist.at(7));
                 query.addBindValue(datalist.at(8));
                 query.addBindValue(datalist.at(9));
                 if(!query.exec())
                 {
                    qDebug()<<query.lastError();
                    return SQLPROCWR0NG;
                 }
           }
        }
        break;

        case E_TABLE_DEPARTS:
        {
            operation +="select *";
            operation +=" from ";
            operation +=tableName;
            operation +=" where ";
            operation +=keyword.at(0);
            operation +="=";
            operation +="\'";
            operation +=value.at(0);
            operation +="\'";

            //qDebug()<<operation;
            if(!query.exec(operation))
            {
                qDebug()<<query.lastError();
                return SQLPROCWR0NG;
            }
            if(query.next() == true)
            {
                #if 0
                operation = QString("update person set name=%1,part=%2,icCard=%3,pic=%4 where workId=%5 ").arg("'"+datalist.at(1)+"'").arg("'"+datalist.at(2)+"'").arg("'"+datalist.at(3)+"'").arg("'"+datalist.at(4)+"'").arg("'"+datalist.at(0)+"'");
                //qDebug()<<operation;
                if(!query.exec(operation))
                {
                    qDebug()<<query.lastError();
                    return SQLPROCWR0NG;
                }
                #endif
            }
            else
           {
                operation.clear();
                operation +="insert into ";
                operation +=tableName;
                operation +=" (";
                operation +=K_DEPTSCODE;
                operation +=",";
                operation +=K_DEPTSINFO;
                operation +=")";

                operation +=" values ";
                operation +=" (";
                operation +="\'"+datalist.at(0)+"\'";
                operation +=",";
                operation +="\'"+datalist.at(1)+"\'";
                operation +=")";
                //qDebug()<<operation;
                if(!query.exec(operation))
                {
                    qDebug()<<query.lastError();
                    return SQLPROCWR0NG;
                }
               }
            }
        break;



        default:
        break;

    }
    return SUCCESS;

}

int faceDb::getRowCount(QString tablename)
{
    QString str;
    str = QString("SELECT * FROM %1").arg(tablename);    
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
    {
        qDebug()<<query2.lastError();
        return 0;
    }
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query2);
    while(queryModel->canFetchMore())
    {
        queryModel->fetchMore();
    }
    int nRecordCount = queryModel->rowCount();
    delete queryModel;
    query2.clear();
    return nRecordCount;
}

int faceDb::getRowCount(QString tablename,QString condition)
{
    QString str;
    str = QString("SELECT * FROM %1 where %2").arg(tablename).arg(condition);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
    {
        qDebug()<<query2.lastError();
        return 0;
    }

    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query2);
    while(queryModel->canFetchMore())
    {
        queryModel->fetchMore();
    }
    int nRecordCount = queryModel->rowCount();
    delete queryModel;
    query2.clear();
    return nRecordCount;
}

QStringList faceDb::getRowContent(QString tablename,QString condition)
{
    QStringList  out; 
    QString str;
    str = QString("SELECT * FROM %1 where %2").arg(tablename).arg(condition);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
    {
        qDebug()<<query2.lastError();
        return out;
    }    

    if(query2.next() == true)
    {
        if(tablename.compare(TABLE_DEVS) == 0)
        {
            /*0:ip,1:port,2:sn,3:clientid,4:devtype,5:passtype,6:devStatus*/
            out.append(query2.value(4).toString());
            out.append(QString::number(query2.value(5).toInt()));
            out.append(query2.value(10).toString());
            out.append(query2.value(11).toString());
            out.append(query2.value(12).toString());
            out.append(query2.value(15).toString());
            out.append(query2.value(16).toString());
        }
    }
    query2.clear();
    return out;
}


QStringList faceDb::getRowContent(QString tablename)
{
    QStringList  out;
    QString str;
    str = QString("SELECT * FROM %1 order by devSN desc").arg(tablename);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
      {
        qDebug()<<query2.lastError();
        return out;
    }

    while(query2.next() == true)
    {
        if(tablename.compare(TABLE_DEVS) == 0)
        {
            /*sn*/
            out.append(query2.value(10).toString());
        }
    }
    query2.clear();
    return out;
}


int faceDb::updataContent(QString tablename,QString condition,QString setValue,QStringList key,QStringList value)
{
    int ret = -1;
    QString str;    
    str = QString("update %1 set %2='%3' where %4='%5'").arg(tablename).arg(condition).arg(setValue).arg(key.at(0)).arg(value.at(0));
    QSqlQuery query2(str,db);  
    if(query2.lastError().isValid())
     {
        qDebug()<<query2.lastError();
        return ret;
     }
    else
    {
        ret = 0;
    }
    query2.clear();
    return ret;
}


int faceDb::begin()
{
    int ret = -1;
    QString str;
    str = QString("BEGIN;");
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
     {
        qDebug()<<query2.lastError();
        return ret;
     }
    else
    {
        ret = 0;
    }
}

int faceDb::commit()
{
    int ret = -1;
    QString str;
    str = QString("COMMIT;");
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
     {
        qDebug()<<query2.lastError();
        return ret;
     }
    else
    {
        ret = 0;
    }
}

bool faceDb::deleteTable(QString tablename)
{
    int ret = false;
    QString str;
    str = QString("delete from %1 ").arg(tablename);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
     {
        qDebug()<<query2.lastError();
        return ret;
     }
    else
    {
        ret = true;
    }
    query2.clear();
    return ret;
}


void faceDb::getDevPar()
{
    QString str;
    str = QString("SELECT * FROM %1").arg(TABLE_DEVPAR);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
      {
        qDebug()<<query2.lastError();
        return;
    }

    while(query2.next() == true)
    {
            /*version*/
            s_devparInfo.version = query2.value(1).toString();
            /*devName*/
            s_devparInfo.devName = query2.value(2).toString();
            /*productKey*/
            s_devparInfo.productKey = query2.value(3).toString();
            /*devsecrect*/
            s_devparInfo.devSecret = query2.value(4).toString();
            /*OSSDomain*/
            s_devparInfo.OSSDomain = query2.value(5).toString();
            /*OSSKeyId*/
            s_devparInfo.OSSKeyId = query2.value(6).toString();
            /*OSSSec*/
            s_devparInfo.OSSSec = query2.value(7).toString();

        }
    query2.clear();
}

bool faceDb::setDevPar()
{
    QSqlQuery query2(db);
    query2.prepare("update devPar set version=?,devName=?,productKey=?,devSecret=?,OSSDomain=?,OSSKeyId=?,OSSSec=?");

    query2.addBindValue(s_devparInfo.version);
    query2.addBindValue(s_devparInfo.devName);
    query2.addBindValue(s_devparInfo.productKey);
    query2.addBindValue(s_devparInfo.devSecret);  
    query2.addBindValue(s_devparInfo.OSSDomain);
    query2.addBindValue(s_devparInfo.OSSKeyId);
    query2.addBindValue(s_devparInfo.OSSSec);

    if(!query2.exec())
    {
        qDebug()<<query2.lastError();
        return false;
    }
    query2.clear();
    return true;
}

void faceDb::getNetConf()
{
    QString str;
    str = QString("SELECT * FROM %1").arg(TABLE_NET);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
      {
        qDebug()<<query2.lastError();
        return;
    }

    while(query2.next() == true)
    {
            s_netInfo.netMode = query2.value(1).toInt();
            s_netInfo.autoIp  = query2.value(2).toInt();
            s_netInfo.sIp     = query2.value(3).toString();
            s_netInfo.sMask   = query2.value(4).toString();
            s_netInfo.sGateWay = query2.value(5).toString();
            s_netInfo.sDns     = query2.value(6).toString();
            s_netInfo.wifiSsid = query2.value(7).toString();
            s_netInfo.wifiPwd  = query2.value(8).toString();
            s_netInfo.wifiMac  = query2.value(9).toString();
            s_netInfo.adhoc = query2.value(10).toInt();
            s_netInfo.wifiAuthMode = query2.value(11).toInt();
            s_netInfo.secMode = query2.value(12).toInt();
            s_netInfo.APN      = query2.value(13).toString();
            s_netInfo.user     = query2.value(14).toString();
            s_netInfo.pwd      = query2.value(15).toString();
            s_netInfo.authMode = query2.value(16).toInt();
            s_netInfo.serverIp = query2.value(17).toString();
            s_netInfo.serverPort = query2.value(18).toInt();

    }
    query2.clear();
}

bool faceDb::setNetConf()
{
    QSqlQuery query2(db);
    query2.prepare("update netConf set netMode=?,autoIp=?,sIp=?,sMask=?,sGateWay=?,sDns=?,wifiSsid=?,"
                   "wifiPwd=?,APN=?,user=?,pwd=?,authMode=?,serverIp=?,serverPort=? where id=? ");

    query2.addBindValue(s_netInfo.netMode);
    query2.addBindValue(s_netInfo.autoIp);
    query2.addBindValue(s_netInfo.sIp);
    query2.addBindValue(s_netInfo.sMask);
    query2.addBindValue(s_netInfo.sGateWay);
    query2.addBindValue(s_netInfo.sDns);
    query2.addBindValue(s_netInfo.wifiSsid);
    query2.addBindValue(s_netInfo.wifiPwd);
    query2.addBindValue(s_netInfo.APN);
    query2.addBindValue(s_netInfo.user);
    query2.addBindValue(s_netInfo.pwd);
    query2.addBindValue(s_netInfo.authMode);
    query2.addBindValue(s_netInfo.serverIp);
    query2.addBindValue(s_netInfo.serverPort);
    query2.addBindValue(1);

    if(!query2.exec())
    {
        qDebug()<<query2.lastError();
        return false;
    }
    query2.clear();
    return true;
}

bool faceDb::setWifiNetConf()
{
    QSqlQuery query2(db);
    query2.prepare("update netConf set netMode=?,autoIp=?,sIp=?,sMask=?,sGateWay=?,sDns=?,wifiSsid=?,"
                   "wifiPwd=?,wifiMac=?,adhoc=?,wifiAuthMode=?,secMode=?,serverIp=?,serverPort=? where id=? ");

    query2.addBindValue(s_netInfo.netMode);
    query2.addBindValue(s_netInfo.autoIp);
    query2.addBindValue(s_netInfo.sIp);
    query2.addBindValue(s_netInfo.sMask);
    query2.addBindValue(s_netInfo.sGateWay);
    query2.addBindValue(s_netInfo.sDns);
    query2.addBindValue(s_netInfo.wifiSsid);
    query2.addBindValue(s_netInfo.wifiPwd);
    query2.addBindValue(s_netInfo.wifiMac);
    query2.addBindValue(s_netInfo.adhoc);
    query2.addBindValue(s_netInfo.wifiAuthMode);
    query2.addBindValue(s_netInfo.secMode);
    query2.addBindValue(s_netInfo.serverIp);
    query2.addBindValue(s_netInfo.serverPort);
    query2.addBindValue(QString("1").toInt());

    if(!query2.exec())
    {
        qDebug()<<query2.lastError();
        return false;
    }
    query2.clear();
    return true;
}

bool faceDb::set4GNetConf()
{
    QSqlQuery query2(db);
    query2.prepare("update netConf set netMode=?,APN=?,user=?,pwd=?,authMode=?,serverIp=?,serverPort=? where id=? ");

    query2.addBindValue(s_netInfo.netMode);
    query2.addBindValue(s_netInfo.APN);
    query2.addBindValue(s_netInfo.user);
    query2.addBindValue(s_netInfo.pwd);
    query2.addBindValue(s_netInfo.authMode);
    query2.addBindValue(s_netInfo.serverIp);
    query2.addBindValue(s_netInfo.serverPort);
    query2.addBindValue(1);

    if(!query2.exec())
    {
        qDebug()<<query2.lastError();
        return false;
    }
    query2.clear();
    return true;
}

bool faceDb::setLanNetConf()
{
    QSqlQuery query2(db);
    query2.prepare("update netConf set netMode=?,autoIp=?,sIp=?,sMask=?,sGateWay=?,sDns=?,serverIp=?,serverPort=? where id=? ");

    query2.addBindValue(s_netInfo.netMode);
    query2.addBindValue(s_netInfo.autoIp);
    query2.addBindValue(s_netInfo.sIp);
    query2.addBindValue(s_netInfo.sMask);
    query2.addBindValue(s_netInfo.sGateWay);
    query2.addBindValue(s_netInfo.sDns);
    query2.addBindValue(s_netInfo.serverIp);
    query2.addBindValue(s_netInfo.serverPort);
    query2.addBindValue(1);

    if(!query2.exec())
    {
        qDebug()<<query2.lastError();
        return false;
    }
    query2.clear();
    return true;
}



void faceDb::getDevConf()
{
    QString str;
    str = QString("SELECT * FROM %1").arg(TABLE_DEVS);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
      {
        qDebug()<<query2.lastError();
        return;
    }

    while(query2.next() == true)
    {
            char *id=nullptr,*pai=nullptr;
            id = s_devInfo.devId;
            id = query2.value(1).toString().toLatin1().data();
            pai = s_devInfo.chepai;
            pai = query2.value(2).toString().toLatin1().data();
            s_devInfo.workMode     = query2.value(3).toInt();
            s_devInfo.delayOff   = query2.value(4).toInt();
            s_devInfo.leftDetect = query2.value(5).toInt();
            s_devInfo.stadbyTime     = query2.value(6).toInt();
            s_devInfo.faceInterval = query2.value(7).toInt();
            s_devInfo.clearTime  = query2.value(8).toInt();
            s_devInfo.shxMode      = query2.value(9).toInt();
            s_devInfo.volume     = query2.value(10).toInt();
            s_devInfo.faceSize      = query2.value(11).toInt();
            s_devInfo.uploadType = query2.value(12).toInt();
            s_devInfo.natStatus = query2.value(13).toInt();
            s_devInfo.timezone = query2.value(14).toInt();
            s_devInfo.temperature = query2.value(15).toInt();
            s_devInfo.noReg = query2.value(16).toInt();
            s_devInfo.lightType = query2.value(17).toInt();
            s_devInfo.personLimit = query2.value(18).toInt();
            s_devInfo.setTts = query2.value(19).toInt();
            s_devInfo.syncTimer = query2.value(20).toInt();
    }
    query2.clear();
}


bool faceDb::setDevConf()
{
 //   QString str;
 //   str = QString("update %1 set %2='%3' where %4='%5'").arg(tablename).arg(condition).arg(setValue).arg(key.at(0)).arg(value.at(0));



    QSqlQuery query2(db);
    query2.prepare("update devConf set chepai=?,workMode=?,delayOff=?,leftDetect=?,standbyTime=?,faceInterval=?,clearTime=?,shxMode=?,"
                   "volume=?,faceSize=?,uploadType=?,natStatus=?,timeZone=?,temperature=?,noReg=?,lightType=?,personLimit=?,setTts=?,syncTimer=? where id=?");

    query2.addBindValue(QString(QLatin1String(s_devInfo.chepai)));
    query2.addBindValue(s_devInfo.workMode);
    query2.addBindValue(s_devInfo.delayOff);
    query2.addBindValue(s_devInfo.leftDetect);
    query2.addBindValue(s_devInfo.stadbyTime);
    query2.addBindValue(s_devInfo.faceInterval);
    query2.addBindValue(s_devInfo.clearTime);
    query2.addBindValue(s_devInfo.shxMode);
    query2.addBindValue(s_devInfo.volume);
    query2.addBindValue(s_devInfo.faceSize);
    query2.addBindValue(s_devInfo.uploadType);
    query2.addBindValue(s_devInfo.natStatus);
    query2.addBindValue(s_devInfo.timezone);
    query2.addBindValue(s_devInfo.temperature);
    query2.addBindValue(s_devInfo.noReg);
    query2.addBindValue(s_devInfo.lightType);
    query2.addBindValue(s_devInfo.personLimit);
    query2.addBindValue(s_devInfo.setTts);
    query2.addBindValue(s_devInfo.syncTimer);
    query2.addBindValue(1);

    if(!query2.exec())
    {
        qDebug()<<query2.lastError();
        return false;
    }
    query2.clear();
    return true;
}


bool faceDb::saveRecord(S_RECORDINFO *s_recordInfo)
{
    QString str;
    str = QString("SELECT * FROM %1 where %2 = %3").arg(TABLE_RECORD).arg(K_USERFID).arg(s_recordInfo->fid);
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

S_RECORDINFO *faceDb::getRecord()
{
    S_RECORDINFO record;
    QString str;
    str = QString("SELECT * FROM %1 where status=0 limit 1").arg(TABLE_RECORD);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
      {
        qDebug()<<query2.lastError();
        return nullptr;
    }

    while(query2.next() == true)
    {
        record.id = query2.value(0).toInt();
        record.devId = query2.value(1).toString();
        record.recordType = query2.value(2).toInt();
        record.fid     = query2.value(3).toInt();
        record.name   = query2.value(4).toString();
        record.temperature = query2.value(5).toInt();
        record.recordTime     = query2.value(6).toInt();
        record.recordTime = query2.value(7).toInt();
        record.pic  = query2.value(8).toInt();
        record.gps      = query2.value(9).toInt();
        record.pGroup     = query2.value(10).toInt();
        record.cnt      = query2.value(11).toInt();
        record.status = query2.value(12).toInt();
        break;
    }
    query2.clear();
    return &record;
}


bool faceDb::upRecord(int id)
{
    QString str;
    str = QString("update %1 set %2=%3 where %4=%5").arg(TABLE_RECORD).arg(K_STATUS).arg(1).arg(K_RECORDID).arg(id);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
     {
        qDebug()<<query2.lastError();
        return false;
     }
    query2.clear();
    return true;
}

bool faceDb::addPerson(S_PERINFO *s_person)
{
    QSqlQuery query2(db);
    QString str;

    str = QString("SELECT * FROM %1 where %2 = %3").arg(TABLE_PERSON).arg(K_USERFID).arg(s_person->fid);
    query2.exec(str);
    if(query2.lastError().isValid())
    {
        qDebug()<<query2.lastError();
        return false;
    }
    if(query2.next()==true)
    {
        query2.prepare("update  person set feature=?,pGroup=? regTime=? where fid=? and name=?");
        query2.addBindValue(s_person->feature);
        query2.addBindValue(s_person->pGroup);
        query2.addBindValue(s_person->regTime);
        query2.addBindValue(s_person->fid);
        query2.addBindValue(s_person->name);

        if(query2.lastError().isValid())
        {
            qDebug()<<query2.lastError();
            return false;
        }
        query2.clear();
        return true;

    }

    query2.prepare("insert into person (fid,name,icCard,regTime,feature,pGroup) values (?,?,?,?,?,?)");
    query2.addBindValue(s_person->fid);
    query2.addBindValue(s_person->name);
    query2.addBindValue(s_person->icCard);
    query2.addBindValue(s_person->regTime);
    query2.addBindValue(s_person->feature);
    query2.addBindValue(s_person->pGroup);

    if(!query2.exec())
    {
       qDebug()<<query2.lastError();
       return false;
    }
    query2.clear();
    return true;
}

QStringList faceDb::getPerson(int id,int *lastId)
{
    QStringList out;

    QString str;
    str = QString("SELECT * FROM %1 where id>%2 limit 10").arg(TABLE_PERSON).arg(id);
    QSqlQuery query2(str,db);
    if(query2.lastError().isValid())
      {
        qDebug()<<query2.lastError();
        return out;
    }

    for(int i=0;i<10;i++)
    {
        if(query2.next() == true)
        {
            *lastId = query2.value(0).toInt();
            out.append(QString::number(query2.value(1).toInt()));
            out.append(query2.value(2).toString());
            out.append(QString(query2.value(11).toByteArray()));
            out.append(QString::number(query2.value(12).toInt()));
        }
        else
        {
            break;
        }
    }
    query2.clear();
    return out;

}
