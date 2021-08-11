#include "comfunc.h"

comFunc::comFunc(QWidget *parent) : QWidget(parent)
{

}


std::string comFunc::qstringTostd(QString source)
{
    return (source.toStdString());
}


QString comFunc::stddToqstring(std::string source)
{
    return (QString::fromStdString(source));
}

QStringList comFunc::analysisComten(QStringList comtent)
{
    QStringList out,ip,port,key,sec;
    QStringList ip1,port1,key1,sec1;
    if(comtent.size() == 0)
    {
        return out;
    }

     foreach (const QString &str,comtent)
     {
       if(str.contains("YtlfServerIp="))
       {
           ip +=str;
           ip1 = QString(ip.at(0)).split("YtlfServerIp=");
            qDebug()<<ip1.at(1);
       }
       if(str.contains("YtlfServerPort="))
       {
           port +=str;
           port1 = QString(port.at(0)).split("YtlfServerPort=");
           qDebug()<<port1.at(1);
       }
       if(str.contains("YtlfApiKey="))
       {
           key +=str;
           key1 = QString(key.at(0)).split("YtlfApiKey=");
           qDebug()<<key1.at(1);
       }
       if(str.contains("YtlfApiSecret="))
       {
           sec +=str;
           sec1 = QString(sec.at(0)).split("YtlfApiSecret=");
           qDebug()<<sec1.at(1);
       }
     }

    if(ip1.size()>1)
    {
        out.append(ip1.at(1));
    }
    if(port1.size()>1)
    {
        out.append(port1.at(1));
    }
    if(key1.size()>1)
    {
        out.append(key1.at(1));
    }
    if(sec1.size()>1)
    {
        out.append(sec1.at(1));
    }

    return out;
}



QStringList comFunc::readConfig(QString fileName)
{
    QStringList out;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            out.append(str.trimmed());
        }
        file.close();
    }
    return out;
}


bool comFunc::is_ipaddr(const QString &ip)
{
    QRegExp rx2("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");

    if(rx2.exactMatch(ip))
    {
        return true;
    }
    return false;
}


void comFunc::rmAllFile(QFileInfo &file)
{
    if(file.isDir())
    {
        QDir dir(file.filePath());
        foreach(auto fileName, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries))
        {
            rmAllFile(fileName);
        }
       // dir.rmdir(file.filePath());
    }
    file.absoluteDir().remove(file.fileName());
}


void comFunc::displayProgress(int max,QString content)
{

    progressDilalog = new QProgressDialog(content," ",0,max,this);
  //  progressDilalog->setWindowFlags(Qt::FramelessWindowHint);//无边框
    progressDilalog->setWindowModality(Qt::WindowModal);//模态对话框
    progressDilalog->setWindowTitle(QObject::tr("请稍后"));
    //少于设置时间就不显示进度条对话框，设置为0始终显示对话框
    progressDilalog->setMinimumDuration(0);
    progressDilalog->setStyleSheet("QLabel{font-family: 微软雅黑;"
                             "font-weight:bold;"
                             "font-size:25px;"
                             "color: #303030;}"
                         "QProgressDialog{background-image: url(:/no.png);}"
                         "QProgressBar{border: 1px solid grey;"         //外边框
                                 "border-color:rgb(128, 128, 128);"     //外边框颜色
                                 "text-align: center;"                  //字体对齐方式
                                 "background: rgb(255, 255, 255);}"
                         "QProgressBar::chunk { border: none;"
                                 "background: #2966ff;}"     //进度条颜色
                         "QPushButton{max-width:0px;"                    //宽
                                "min-height:40px;"                        //高
                                "background-color:rgba(255,255,255,0);}"    //设置按钮为透明
                         );



    progressDilalog->setFixedSize(400, 200);//设置大小
 //   progressDilalog->move((this->width() - progressDilalog->width())/2,(this->height() - progressDilalog->height())/2);

    connect(this,SIGNAL(setProgress(int)),progressDilalog,SLOT(setValue(int)));
    connect(progressDilalog,&QProgressDialog::canceled,this,&comFunc::dialogCancel);
    progressDilalog->setWindowModality(Qt::ApplicationModal);
    //显示处理框
    progressDilalog->show();

}


void comFunc::dialogCancel()
{
    flag_cancel = true;
}

S_FILEFLAG *comFunc::UsbExit(QString path)
{
    S_FILEFLAG fileFlag={false,false,false,false};
#if 0

    std::string path,shellCmd;
    shellCmd = "mount | grep /dev/sd | awk '{printf $3}'";
    execShell(shellCmd,path);
    if(path.length() == 0)
    {
        QMessageBox megboxWrong;
        megboxWrong.setText(QObject::tr("U盘检测失败！"));
        megboxWrong.setWindowTitle(QObject::tr("reportPath提示"));
        megboxWrong.setIcon(QMessageBox::Information);
        QPushButton *okbut = megboxWrong.addButton(QObject::tr("确 定"),QMessageBox::YesRole);
        megboxWrong.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        megboxWrong.exec();
        return &fileFlag;
    }
    printf("usb path:%s\n",path.c_str());
    std::string src,des;
    if(isFileExist(stddToqstring(path)+"/face"))
    {
         src = (path)+"/face";
         des = qstringTostd(qApp->applicationDirPath())+"upApp";
         if(copyFile(src,des))
         {
                fileFlag.flag_up = true;
         }
    }
    if(isFileExist(stddToqstring(path)+"/TerminaManage.cnf"))
    {

         if(copyFile(path+"/TerminaManage.cnf",qstringTostd(qApp->applicationDirPath())+"TerminaManage.cnf"))
         {
                fileFlag.flag_conf = true;
         }
    }
    if(isFileExist(stddToqstring(path)+"/standby.png"))
    {
         if(copyFile(path+"/standby.png",qstringTostd(qApp->applicationDirPath())+"standby.png"))
         {
                fileFlag.flag_stand = true;
         }
    }
    if(isDirExist(stddToqstring(path)+"/voice/"))
    {
         if(copyFile(path+"/voice/*.aac",qstringTostd(qApp->applicationDirPath())+"/wav/"))
         {
                fileFlag.flag_voice= true;
         }
    }std::string src,des;
    if(isFileExist(stddToqstring(path)+"/face"))
    {
         src = (path)+"/face";
         des = qstringTostd(qApp->applicationDirPath())+"upApp";
         if(copyFile(src,des))
         {
                fileFlag.flag_up = true;
         }
    }
    if(isFileExist(stddToqstring(path)+"/TerminaManage.cnf"))
    {

         if(copyFile(path+"/TerminaManage.cnf",qstringTostd(qApp->applicationDirPath())+"TerminaManage.cnf"))
         {
                fileFlag.flag_conf = true;
         }
    }
    if(isFileExist(stddToqstring(path)+"/standby.png"))
    {
         if(copyFile(path+"/standby.png",qstringTostd(qApp->applicationDirPath())+"standby.png"))
         {
                fileFlag.flag_stand = true;
         }
    }
    if(isDirExist(stddToqstring(path)+"/voice/"))
    {
         if(copyFile(path+"/voice/*.aac",qstringTostd(qApp->applicationDirPath())+"/wav/"))
         {
                fileFlag.flag_voice= true;
         }
    }
#else
    qDebug()<<"path:"<<path;

    QDir dir;
    dir.setPath(path);
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();
    QString src,des;

    if (list.count() <= 0)
    {
        qDebug() << "file is null";
        return &fileFlag;
    }
    else
    {

        if(isFileExist(path+"/face"))
        {
             src = path+"/face";
             des = qApp->applicationDirPath()+"upApp";
             if(QFile::copy(src,des))
             {
                    fileFlag.flag_up = true;
             }
        }

        if(isFileExist(path+"/TerminaManage.cnf"))
        {

             if(QFile::copy(path+"/TerminaManage.cnf",qApp->applicationDirPath()+"TerminaManage.cnf"))
             {
                    fileFlag.flag_conf = true;
             }
        }
        if(isFileExist(path+"/standby.png"))
        {
             if(QFile::copy(path+"/standby.png",qApp->applicationDirPath()+"standby.png"))
             {
                    fileFlag.flag_stand = true;
             }
        }
        if(isDirExist(path+"/voice/"))
        {
             if(QFile::copy(path+"/voice/*.aac",qApp->applicationDirPath()+"/wav/"))
             {
                    fileFlag.flag_voice= true;
             }
        }

    }
    return (&fileFlag);
     #endif
}

void comFunc::execShell(const std::string sCmdShell, std::string& sInfo)
{
    printf("[SystemConf::execShell[%s]\n", sCmdShell.c_str());

    sInfo = "";
    FILE* fP = popen(sCmdShell.c_str(), "r");
    std::shared_ptr<FILE> ptrFile(fP, [](FILE* fP) {if(fP)pclose(fP); });
  //    std::make_shared<FILE> ptrFile(fP, [](FILE* fP) {if(fP)pclose(fP); });
    if (nullptr == ptrFile) {
        printf("[SystemInfo::execShell]failed to popen %s\n", sCmdShell.c_str());
        return;
    }
    char cInfo[1024] = { 0 };
    while (fgets(cInfo, sizeof(cInfo) - 1, ptrFile.get()) != NULL)
    {
        sInfo += cInfo;
    }

    trim(sInfo);
  //  pclose(fP);
    printf("[SystemInfo::execShell]execShell[%s] Res[%s]\n", sCmdShell.c_str(), sInfo.c_str());
}

void comFunc::trim(std::string& str)
{
    //trim head

    int len = str.length();

    int i = 0;

    while (isspace(str[i]) && str[i] != '\0') {
        i++;
    }

    if (i != 0) {
        str = std::string(str, i, len - i);
    }

    //trim tail
    len = str.length();

    for (i = len - 1; i >= 0; --i) {
        if (!isspace(str[i])) {
            break;
        }
    }

    str = std::string(str, 0, i + 1);
}



bool comFunc::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}

bool comFunc::isDirExist(QString fullPath)
{
    QFileInfo fileInfo(fullPath);
    if(fileInfo.isDir())
    {
      return true;
    }
    return false;
}


bool comFunc::copyFile(const std::string sSrcFile, const std::string sDestFile)
{
    char cCmdShell[1024] = { 0 };
    sprintf(cCmdShell, "cp -f %s  %s", sSrcFile.c_str(), sDestFile.c_str());
    std::string sInfo;
    execShell(cCmdShell, sInfo);
    return 0==sInfo.length();
}
