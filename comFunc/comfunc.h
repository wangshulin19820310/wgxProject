#ifndef COMFUNC_H
#define COMFUNC_H

#include <QWidget>
#include "include.h"
#include <memory>

class comFunc : public QWidget
{
    Q_OBJECT
public:
    explicit comFunc(QWidget *parent = nullptr);

public:
    std::string qstringTostd(QString source);
    QString stddToqstring(std::string source);
    QStringList readConfig(QString fileName);
    QStringList analysisComten(QStringList comtent);
    bool is_ipaddr(const QString &ip);
    void rmAllFile(QFileInfo &file);
    S_FILEFLAG *UsbExit(QString path);
    void displayProgress(int max,QString content);
    bool flag_cancel = false;
    void execShell(const std::string sCmdShell, std::string& sInfo);
    void trim(std::string& str);
    bool isFileExist(QString fullFileName);
    bool isDirExist(QString fullPath);
    bool copyFile(const std::string sSrcFile, const std::string sDestFile);

signals:
    void setProgress(int);

private slots:
    void dialogCancel();

 private:
        QProgressDialog *progressDilalog = nullptr;

};

#endif // COMFUNC_H
