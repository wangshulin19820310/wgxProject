#include "common_input.h"
#include "ui_common_input.h"

#include "include.h"
#include "../input2019/frminput2019.h"


common_input::common_input(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::common_input)
{
    ui->setupUi(this);
//    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint);

    QFont font;
    ui->le_input->setPlaceholderText(QObject::tr("请点击输入框输入内容"));
    ui->le_input->setFont(font);
    ui->le_input->setMaxLength(num);
    ui->le_input->setStyleSheet("border:1px groove gray;"
                                    "border-radius:5px;"
                                    "padding:1px 2px;"
                                    "color:#333333");

    ui->bt_confirm->setFont(font);
    ui->bt_confirm->setText(QObject::tr("确定"));
    ui->bt_confirm->setStyleSheet("border:2px groove white;"
                                  "border-radius:10px;"
                                  "padding:2px 4px;"
                                  "background-color:#1671f1;"
                                  "color:white;");
    ui->bt_cancel->setFont(font);
    ui->bt_cancel->setText(QObject::tr("取消"));
    ui->bt_cancel->setStyleSheet("border:2px groove white;"
                                  "border-radius:10px;"
                                  "padding:2px 4px;"
                                  "background-color:rgba(136,136,136,100);"
                                  "color:white;");
    /*
    ui->widget->setStyleSheet("border:2px groove gray;"
                                  "border-radius:5px;"
                                  "padding:2px 4px;"
                                  "background-color:rgb(255,255,255);"
                                  "color:white;");
                                  */




    //软键盘
 //   m_keyBoard = QKeyBoard::getInstance();
 //   m_keyBoard->hidePanel();

}

common_input::~common_input()
{
    delete ui;
}

void common_input::on_bt_cancel_clicked()
{
    QString sendmessage = "";
    bool flag = 0;
    emit senddata(sendmessage,flag);
    frmInput2019::Instance()->hide();
 //   m_keyBoard->isHidden();
    close();
}

void common_input::on_bt_confirm_clicked()
{
    bool flag = 1;
    emit senddata(ui->le_input->text(),flag);
    frmInput2019::Instance()->hide();
 //   m_keyBoard->isHidden();
    close();
}


void common_input::input_text(QString title, QString mac)
{
    ui->lb_title->setText(title);
    ui->lb_mac->setText(mac);
}

void common_input::input_num(int inputnum)
{
	ui->le_input->setMaxLength(inputnum);

}



bool common_input::is_ipaddr(const QString &ip)
{
    QRegExp rx2("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");

    if(rx2.exactMatch(ip))
    {
        return true;
    }
    return false;
}


void common_input::disfont(int num)
{
    QFont font;
    font.setPointSize(num);

    ui->le_input->setPlaceholderText("请点击输入框输入内容");
    ui->le_input->setFont(font);
    ui->le_input->setMaxLength(20);
    ui->le_input->setStyleSheet("border:1px groove gray;"
                                    "border-radius:5px;"
                                    "padding:1px 2px;"
                                    "color:#333333");

    ui->lb_title->setFont(font);    
    ui->lb_mac->setFont(font);
    ui->lb_title->setStyleSheet("color:#333333;font:10pt");
    ui->lb_mac->setStyleSheet("color:#333333;font:10pt");


}



