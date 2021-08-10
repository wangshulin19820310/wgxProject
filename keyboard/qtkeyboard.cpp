﻿#include "qtkeyboard.h"
#include "ui_qtkeyboard.h"
#include <QDebug>
#include <QTimer>
#include <QDesktopWidget>

QKeyBoard* QKeyBoard::_instance = nullptr;
#define str_board_close "Close"
#define SHIFT_ON "CAPS"
#define SHIFT_OFF "caps"

QKeyBoard::QKeyBoard(QWidget *parent) : QWidget(parent),
    ui(new Ui::QKeyBoard),
    isShiftOn(false),
    lineEdit(nullptr),
    mousePressed(false)
{
    availableGeometry = QApplication::desktop()->availableGeometry();

    ui->setupUi(this);
    ui->btna->setProperty("btnLetter", true);
    ui->btnb->setProperty("btnLetter", true);
    ui->btnc->setProperty("btnLetter", true);
    ui->btnd->setProperty("btnLetter", true);
    ui->btne->setProperty("btnLetter", true);
    ui->btnf->setProperty("btnLetter", true);
    ui->btng->setProperty("btnLetter", true);
    ui->btnh->setProperty("btnLetter", true);
    ui->btni->setProperty("btnLetter", true);
    ui->btnj->setProperty("btnLetter", true);
    ui->btnk->setProperty("btnLetter", true);
    ui->btnl->setProperty("btnLetter", true);
    ui->btnm->setProperty("btnLetter", true);
    ui->btnn->setProperty("btnLetter", true);
    ui->btno->setProperty("btnLetter", true);
    ui->btnp->setProperty("btnLetter", true);
    ui->btnq->setProperty("btnLetter", true);
    ui->btnr->setProperty("btnLetter", true);
    ui->btns->setProperty("btnLetter", true);
    ui->btnt->setProperty("btnLetter", true);
    ui->btnu->setProperty("btnLetter", true);
    ui->btnv->setProperty("btnLetter", true);
    ui->btnw->setProperty("btnLetter", true);
    ui->btnx->setProperty("btnLetter", true);
    ui->btny->setProperty("btnLetter", true);
    ui->btnz->setProperty("btnLetter", true);

   this->resize(200,200);
#if 0
    QString btnStyle =
        "QPushButton{\
            color: rgb(255, 255, 255);\
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(166,164,208), stop:0.3 rgb(171,152,230), stop:1 rgb(152,140,220));\
            border:1px;\
            border-radius:5px; \
            padding:10px;  \
        }\
        QPushButton:pressed{    \
            color: rgb(255, 255, 255); \
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(240,156,121), stop:0.3 rgb(220,160,140), stop:1 rgb(230,140,120));  \
            border:1px;  \
            border-radius:5px; \
            padding:10px; \
        }";
#else
    QString btnStyle =
            "QPushButton{\
                color: #f3f3f3;\
                background-color: #313131;\
                border:1px;\
                border-radius:3px; \
                padding:10px;  \
            }\
            QPushButton:pressed{    \
                color: #f3f3f3; \
                background-color: #24B1DF;  \
                border:1px;  \
                border-radius:3px; \
                padding:10px; \
            }";
        this->setStyleSheet("background-color:#000000;");
#endif
    QFont font;
    font.setPixelSize(availableGeometry.height()/40);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    /*connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)),
            this, SLOT(slot_onApplicationFocusChanged(QWidget*, QWidget*)));*/
    qApp->installEventFilter(this);

    QList<QPushButton*> btns = this->findChildren<QPushButton*>();
    foreach (QPushButton *button, btns) {
        connect(button, SIGNAL(clicked()), this, SLOT(btn_clicked()));
        button->setFont(font);
        button->setStyleSheet(btnStyle);
    }

    hidePanel();
}

QKeyBoard::~QKeyBoard()
{
    delete ui;
    _instance = nullptr;
}

void QKeyBoard::changeInputType(bool on)
{
    QString str;
    ui->btnShift->setText(on?SHIFT_ON:SHIFT_OFF);
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    foreach (QPushButton *button, buttons) {
        if (button->property("btnLetter").toBool()) {
            if (on)
                button->setText(button->text().toUpper());
            else
                button->setText(button->text().toLower());
        } else if(! button->objectName().compare("btnApostrophe")){
            button->setText(on?"\"":"\'");
        } else if(! button->objectName().compare("btnBackslash")){
            button->setText(on?"|":"\\");
        } else if(! button->objectName().compare("btnComma")){
            button->setText(on?"<":",");
        } else if(! button->objectName().compare("btnDash")){
            button->setText(on?"_":"-");
        } else if(! button->objectName().compare("btnDot")){
            button->setText(on?">":".");
        } else if(! button->objectName().compare("btnEquals")){
            button->setText(on?"+":"=");
        } else if(! button->objectName().compare("btnSemicolon")){
            button->setText(on?":":";");
        } else if(! button->objectName().compare("btnSlash")){
            button->setText(on?"?":"/");
        } else if(! button->objectName().compare("btnOther1")){
            button->setText(on?"~":"`");
        } else if(! button->objectName().compare("btnOther2")){
            button->setText(on?"{":"[");
        } else if(! button->objectName().compare("btnOther3")){
            button->setText(on?"}":"]");
        } else if(! button->objectName().compare("btn1")){
            button->setText(on?"!":"1");
        } else if(! button->objectName().compare("btn2")){
            button->setText(on?"@":"2");
        } else if(! button->objectName().compare("btn3")){
            button->setText(on?"#":"3");
        } else if(! button->objectName().compare("btn4")){
            button->setText(on?"$":"4");
        } else if(! button->objectName().compare("btn5")){
            button->setText(on?"%":"5");
        } else if(! button->objectName().compare("btn6")){
            button->setText(on?"^":"6");
        } else if(! button->objectName().compare("btn7")){
            button->setText(on?"&":"7");
        } else if(! button->objectName().compare("btn8")){
            button->setText(on?"*":"8");
        } else if(! button->objectName().compare("btn9")){
            button->setText(on?"(":"9");
        } else if(! button->objectName().compare("btn0")){
            button->setText(on?")":"0");
        }
    }
}

void QKeyBoard::showPanel()
{
    changeInputType(isShiftOn);
    this->setVisible(true);
}

void QKeyBoard::hidePanel()
{
    this->setVisible(false);
}

void QKeyBoard::slot_onApplicationFocusChanged(QWidget *, QWidget *nowWidget)
{
    if (nowWidget != nullptr && !this->isAncestorOf(nowWidget)) {
        if (nowWidget->inherits("QLineEdit")) {
            lineEdit = (QLineEdit*)nowWidget;
            showPanel();
        } else {
            hidePanel();
        }

        resize(availableGeometry.width(), height());
        move(0, availableGeometry.height() - height());
    }
}

void QKeyBoard::focusLineEdit(QLineEdit *edit)
{
	if(!edit || lineEdit == edit)
		return;
    if(lineEdit)lineEdit->clearFocus();
	lineEdit = edit;
    lineEdit->setFocus();
	resize(availableGeometry.width(), height());
	move(0, availableGeometry.height() - height());
}

void QKeyBoard::btn_clicked()
{
    if (lineEdit == nullptr)
        return;

    QPushButton *button = (QPushButton*)sender();
    QString objectName = button->objectName();
    if (objectName == "btnBackspace") {
        lineEdit->backspace();
    } else if (objectName == "btnClose") {
        // foucs other widget first.
        if (lineEdit && lineEdit->parentWidget())
            lineEdit->parentWidget()->setFocus();
        hidePanel();
    } else if (objectName == "btnSpace") {
        insertValue(" ");
    } else if (objectName == "btnShift") {
        isShiftOn = (isShiftOn == true) ? false : true;
        changeInputType(isShiftOn);
    } else {
        insertValue(button->text());
    }
}

void QKeyBoard::insertValue(const QString &value)
{
    if (lineEdit->text().length() < 20)
        lineEdit->insert(value);
}

void QKeyBoard::mousePressEvent(QMouseEvent *event)
{
    /*if (event->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = event->globalPos() - this->pos();
        event->accept();
    }*/
}

void QKeyBoard::mouseMoveEvent(QMouseEvent *event)
{
    /*if (mousePressed && event->buttons() == Qt::LeftButton) {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }*/
}

void QKeyBoard::mouseReleaseEvent(QMouseEvent *)
{
    /*mousePressed = false;
    emit sigMouseRelease();*/
}
