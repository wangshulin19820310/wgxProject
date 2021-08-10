#ifndef COMMON_INPUT_H
#define COMMON_INPUT_H

#include <QDialog>
//#include "keyboard/qtkeyboard.h"
#include "input2019/frminput2019.h"
namespace Ui {
class common_input;
}

class common_input : public QDialog
{
    Q_OBJECT

public:
    explicit common_input(QWidget *parent = 0);
    ~common_input();

public:
    void input_text(QString title, QString mac);
	void input_num(int inputnum);
    bool is_ipaddr(const QString &ip);
    void disfont(int num);
	int num = 20;    


private slots:
    void on_bt_cancel_clicked();

    void on_bt_confirm_clicked();



signals:
    void senddata(QString intputdata,bool flag);


private:
    Ui::common_input *ui;
  //  QKeyBoard* m_keyBoard=nullptr;//软键盘
};

#endif // COMMON_INPUT_H
