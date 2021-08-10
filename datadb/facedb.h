#ifndef FACEDB_H
#define FACEDB_H

#include "include.h"


/*表名宏定义*/
#define  TABLE_DEVPAR    "devPar"

#define  TABLE_DEVS         "devConf"

#define  TABLE_PERSON       "person"

#define  TABLE_RECORD       "devRecord"

#define  TABLE_NET          "netConf"


#define  TABLE_AUTHORITY    "workIdAuthority"



/*关键字定义*/


#define  K_USERSN           "userSn"
#define  K_WORKID           "workId"
#define  K_NAME             "name"
#define  K_PART             "part"
#define  K_ICCARD           "icCard"
#define  K_LEVEL            "level"
#define  K_PASSWORD         "passWord"
#define  K_DEVINFO          "devInfo"

#define  K_DEPTSCODE         "deptsCode"
#define  K_DEPTSINFO         "deptsInfo"


#define  K_DEVSN            "devId"
#define  K_STATUS           "status"
#define  K_PIC              "pic"
#define  K_USERNAME         "name"
#define  K_USERFID          "fid"
#define  K_RECORDTIME       "recordTime"
#define  K_RECORDID         "id"

#define  COLUMNTYPE         "VARCHAR(40)"
#define  COLUMNTYPE_10      "VARCHAR(10)"


/*表操作宏定义*/
typedef enum {
    E_TABLE_SYSTEMPAR = 1,
    E_TABLE_DEVS,
    E_TABLE_PERSON,
    E_TABLE_RECORD,
    E_TABLE_DEPARTS

} e_tableDefine;

typedef enum {
    E_SINGLE = 1,
    E_MULTI,
    E_EDITE,

} e_operDefine;



class faceDb
{
public:
    faceDb();
    static faceDb *Instance();
    QSqlDatabase db;

    bool initDb();
    bool checkTableIsExist(const char *tableName);
    bool selectTable(e_tableDefine operation);
    QString selectAndUpdate(QString tableName,e_operDefine oper,QStringList keyword,QStringList value,QStringList datalist);
    QString selectAndGet(QString tableName,QStringList keyword,QStringList value,QStringList *datalist);
    int getRowCount(QString tablename);
    int getRowCount(QString tablename,QString condition);
    QStringList getRowContent(QString tablename);
    QStringList getRowContent(QString tablename, QString condition);
    int updataContent(QString tablename, QString condition, QString setValue, QStringList key, QStringList value);
    bool checkColumnExist(const char *tableName,const char *column);
    bool addColumn(const char *tableName,const char *column,const char *type);
   // QString selectAndInsert(QString tableName, QStringList keyword, QStringList value);
    bool deleteTable(QString tablename);


	/*获取设备参数	*/
    void getDevPar();
    /*设置设备参数	*/
    bool setDevPar();
	/*获取网络配置	*/
	void getNetConf();
	/*设置网络配置	*/
    bool setNetConf();
    bool setWifiNetConf();
    bool set4GNetConf();
    bool setLanNetConf();
	/*获取设备配置	*/
    void getDevConf();
	/*设置设备配置	*/
    bool setDevConf();
	/*保存记录	*/
    bool saveRecord(S_RECORDINFO *s_recordInfo);
	/*获取一条没有上传过的记录	*/
    S_RECORDINFO *getRecord();
	/*更新记录状态	*/
    bool upRecord(int id);
	/*新增人员	*/
    bool addPerson(S_PERINFO *s_person);
	/*获取人员信息        QStringList 规律 ：fid name feature group	*/
    QStringList getPerson(int id,int *lastId);
	

    int begin();
    int commit();
private:
     static faceDb *self;
     QSqlQuery query;
     QStringList tableList;
     bool isOpen = false;

};

#endif // FACEDB_H
