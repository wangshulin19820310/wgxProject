#include "mainwindow.h"
#include <QApplication>
#include "input2019/frminput2019.h"
#include "comFunc/common_input.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


#if 1
    frmInput2019::Instance()->hide();
    frmInput2019::Instance()->setFontInfo("MicroSoft Yahei", 10, 10);
  //  frmInput2019::Instance()->setFixedSize(720, 528);
    frmInput2019::Instance()->setFixedSize(1280, 400);
    frmInput2019::Instance()->setIconSize(20, 20);
    frmInput2019::Instance()->setTopHeight(40);
    frmInput2019::Instance()->setPosition("bottom");
#endif
    return a.exec();
}
