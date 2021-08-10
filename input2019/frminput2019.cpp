#pragma execution_character_set("utf-8")
#include "frminput2019.h"
#include "ui_frminput2019.h"

frmInput2019 *frmInput2019::self = NULL;
frmInput2019 *frmInput2019::Instance()
{
    if (!self) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new frmInput2019;
        }
    }

    return self;
}

frmInput2019::frmInput2019(QWidget *parent) : QWidget(parent), ui(new Ui::frmInput2019)
{
    ui->setupUi(this);
    this->initForm();
    QTimer::singleShot(100, this, SLOT(init()));
}

frmInput2019::~frmInput2019()
{
    delete ui;
}

void frmInput2019::showEvent(QShowEvent *)
{
    this->update();
}

bool frmInput2019::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this) {
        //处理自身拖动
        static QPoint mousePoint;
        static bool mousePressed = false;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        //按下的时候记住坐标,移动到鼠标松开的位置
        if (event->type() == QEvent::MouseButtonPress) {
            if (mouseEvent->button() == Qt::LeftButton) {
                mousePressed = true;
                mousePoint = mouseEvent->globalPos() - this->pos();
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            mousePressed = false;
            return true;
        } else if (event->type() == QEvent::MouseMove) {
            if (mousePressed && (mouseEvent->buttons() && Qt::LeftButton && position != "bottom")) {
                this->move(mouseEvent->globalPos() - mousePoint);
                this->update();
                return true;
            }
        }
    } else if (watched == ui->labMore) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (inputType == "chinese" && !upper && labCn.first()->isEnabled()) {
                if (!ui->widgetChinese->isVisible()) {
                    ui->widgetLetter->setVisible(false);
                    ui->widgetNumber->setVisible(false);
                    ui->widgetChinese->setVisible(true);
                } else {
                    ui->widgetLetter->setVisible(true);
                    ui->widgetNumber->setVisible(false);
                    ui->widgetChinese->setVisible(false);
                }

                //重新设置图标
                QString strMore = ui->widgetMore->isVisible() ? "up" : "down";
                ui->labMore->setPixmap(QString(":/image/btn_%1_%2.png").arg(strMore).arg(iconType));
                return true;
            }
        }
    } else if (watched == ui->labType) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (inputType == "english") {
                setInputType("chinese");
            } else if (inputType == "chinese") {
                setInputType("english");
            }
        }
    } else if (watched == ui->labType2) {
        if (event->type() == QEvent::MouseButtonPress) {
            setInputType("english");
        }
    } else if (watched == ui->widgetCn) {
        //没有汉字或者按下的地方没有汉字或者当前汉字标签个数过少都不用继续
        if (!labCn.first()->isEnabled() || lastText.isEmpty()) {
            return false;
        }

        //记住最后按下拖动的时间,过短则认为是滑动,启动滑动动画
        static bool pressed = false;
        static QPoint lastPos = QPoint();
        static QDateTime lastTime = QDateTime::currentDateTime();
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (event->type() == QEvent::MouseButtonPress) {
            pressed = true;
            lastPos = mouseEvent->pos();
            animationCn->stop();
            lastTime = QDateTime::currentDateTime();
        } else if (event->type() == QEvent::MouseButtonRelease) {
            pressed = false;
            if (lastPos != mouseEvent->pos()) {
                //判断当前时间和鼠标按下事件比较,时间短则说明是滑动
                QDateTime now = QDateTime::currentDateTime();
                if (lastTime.msecsTo(now) < 600) {
                    //可以改变下面的值来调整幅度
                    bool moveleft = (mouseEvent->pos().x() - lastPos.x()) < 0;
                    int offset = moveleft ? 350 : -350;
                    int value = ui->scrollAreaCn->horizontalScrollBar()->value();
                    animationCn->setStartValue(value);
                    animationCn->setEndValue(value + offset);
                    animationCn->start();
                }
            }
        } else if (event->type() == QEvent::MouseMove) {
            if (pressed && labCn.first()->isEnabled()) {
                //计算滑过的距离
                bool moveleft = (mouseEvent->pos().x() - lastPos.x()) < 0;
                int offset = moveleft ? 5 : -5;
                int value = ui->scrollAreaCn->horizontalScrollBar()->value();
                ui->scrollAreaCn->horizontalScrollBar()->setValue(value + offset);
                return true;
            }
        }
    } else if (watched == ui->widgetMore) {
        //没有汉字或者按下的地方没有汉字或者当前汉字标签个数过少都不用继续
        if (!labMore.first()->isEnabled() || lastText.isEmpty()) {
            return false;
        }

        //记住最后按下拖动的时间,过短则认为是滑动,启动滑动动画
        static bool pressed = false;
        static QPoint lastPos = QPoint();
        static QDateTime lastTime = QDateTime::currentDateTime();
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (event->type() == QEvent::MouseButtonPress) {
            pressed = true;
            lastPos = mouseEvent->pos();
            animationMore->stop();
            lastTime = QDateTime::currentDateTime();
        } else if (event->type() == QEvent::MouseButtonRelease) {
            pressed = false;
            if (lastPos != mouseEvent->pos()) {
                //判断当前时间和鼠标按下事件比较,时间短则说明是滑动
                QDateTime now = QDateTime::currentDateTime();
                if (lastTime.msecsTo(now) < 600) {
                    //可以改变下面的值来调整幅度
                    bool movebottom = (mouseEvent->pos().y() - lastPos.y()) < 0;
                    int offset = movebottom ? 150 : -150;
                    int value = ui->scrollAreaMore->verticalScrollBar()->value();
                    animationMore->setStartValue(value);
                    animationMore->setEndValue(value + offset);
                    animationMore->start();
                }
            }
        } else if (event->type() == QEvent::MouseMove) {
            if (pressed && labMore.first()->isEnabled()) {
                //计算滑过的距离
                bool movebottom = (mouseEvent->pos().y() - lastPos.y()) < 0;
                int offset = movebottom ? 5 : -5;
                int value = ui->scrollAreaMore->verticalScrollBar()->value();
                ui->scrollAreaMore->verticalScrollBar()->setValue(value + offset);
                return true;
            }
        }
    } else if (watched->inherits("QLabel")) {
        QLabel *lab = (QLabel *)watched;
        if (!upper && inputType == "chinese") {
            if (lab->property("labCn").toBool()) {
                //记住最后按下的滚动条位置,如果滚动条一直没有变化则认为单击了标签
                static int lastPosition = 0;
                if (event->type() == QEvent::MouseButtonPress) {
                    lastPosition = ui->scrollAreaCn->horizontalScrollBar()->value();
                    lastText = lab->text();
                } else if (event->type() == QEvent::MouseButtonRelease) {
                    if (lastPosition == ui->scrollAreaCn->horizontalScrollBar()->value() && !lastText.isEmpty()) {
                        insertValue(lab->text());
                        clearChinese();
                    }
                }
            } else if (lab->property("labMore").toBool()) {
                //记住最后按下的滚动条位置,如果滚动条一直没有变化则认为单击了标签
                static int lastPosition = 0;
                if (event->type() == QEvent::MouseButtonPress) {
                    lastPosition = ui->scrollAreaMore->verticalScrollBar()->value();
                    lastText = lab->text();
                } else if (event->type() == QEvent::MouseButtonRelease) {
                    if (lastPosition == ui->scrollAreaMore->verticalScrollBar()->value() && !lastText.isEmpty()) {
                        insertValue(lab->text());
                        clearChinese();
                    }
                }
            }
        }
    } else {
        if (event->type() == QEvent::MouseButtonPress) {
            if (currentWidget != 0) {
                if (!isVisible()) {
                    if (!currentWidget->inherits("QWebView")) {
                        showPanel();
                    }
                }
            } else {
                if (isVisible()) {
                    hidePanel();
                }
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void frmInput2019::initForm()
{
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    setWindowFlags(Qt::Tool | Qt::WindowDoesNotAcceptFocus | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
#else
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
#endif

    currentWidget = 0;
    upper = false;
    number = false;
    onlyControl = false;
    autoHide = false;
    columnCount = 8;
    maxCount = 256;
    dbPath = qApp->applicationDirPath();

    //绑定按钮到事件
    QList<QPushButton *> btns;
    btns << ui->widgetLetter->findChildren<QPushButton *>();
    btns << ui->widgetNumber->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        btn->setProperty("btnInput", true);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(btnClicked()));
    }

    //设置字母属性
    btns.clear();
    btns << ui->widgetLetter1->findChildren<QPushButton *>();
    btns << ui->widgetLetter2->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        btn->setProperty("btnLetter", true);
        btn->setCheckable(true);
    }

    //设置所有按钮输入法不可用+长按自动重复事件
    btns.clear();
    btns << this->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setProperty("noinput", true);
     //   btn->setAutoRepeat(true);
     //   btn->setAutoRepeatDelay(500);
        btn->setAutoRepeat(false);
    }

    //默认最大生成256个,添加到顶部滚动区域中
    for (int i = 0; i < maxCount; i++) {
        QLabel *lab = new QLabel;
        lab->setProperty("labCn", true);
        lab->setEnabled(false);
        ui->layout->addWidget(lab);
        labCn << lab;
    }

    //默认最大生成256个,添加到更多滚动区域中
    int row = 0;
    int column = 0;
    for (int i = 0; i < maxCount; i++) {
        QLabel *lab = new QLabel;
        lab->setProperty("labMore", true);
        lab->setEnabled(false);
        lab->setAlignment(Qt::AlignCenter);
        lab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        ui->gridLayout->addWidget(lab, row, column);
        labMore << lab;

        column++;
        if (column >= columnCount) {
            row++;
            column = 0;
        }
    }

    ui->lab1->setEnabled(false);
    ui->lab2->setEnabled(false);
    ui->labPY->setEnabled(false);
    ui->labMore->setEnabled(false);

    //输入法面板的字体名称和按钮字体大小即汉字区域字体大小
    setFontInfo(this->font().family(), 11, 10);
    //图标固定大小
    setIconSize(20, 20);
    //按钮之间的间隔
    setSpacing(15);
    //顶部汉字区域高度
    setTopHeight(40);

    //输入法面板的显示位置  control--显示在对应输入框的正下方 bottom--填充显示在底部  center--窗体居中显示
    setPosition("bottom");
    //输入法模式 english--英文模式  chinese--中文模式  number--数字特殊字符模式
    setInputType("english");
    //输入法面板的样式  black--黑色  blue--淡蓝色  brown--灰黑色  gray--灰色  silvery--银色
    setStyleName("black");

    //定义动画产生平滑数值
    animationCn = new QPropertyAnimation(ui->scrollAreaCn->horizontalScrollBar(), "value");
    animationCn->setEasingCurve(QEasingCurve::OutCirc);
    animationCn->setDuration(500);

    animationMore = new QPropertyAnimation(ui->scrollAreaMore->verticalScrollBar(), "value");
    animationMore->setEasingCurve(QEasingCurve::OutCirc);
    animationMore->setDuration(500);
}

void frmInput2019::init()
{
    if (onlyControl) {
        ui->labPY->setVisible(false);
        this->installEventFilter(this);
        ui->labType->installEventFilter(this);
        ui->labType2->installEventFilter(this);
        ui->labMore->installEventFilter(this);
        ui->widgetCn->installEventFilter(this);
        ui->widgetMore->installEventFilter(this);

        foreach (QLabel *lab, labCn) {
            lab->installEventFilter(this);
        }

        foreach (QLabel *lab, labMore) {
            lab->installEventFilter(this);
        }
    } else {
        //绑定全局改变焦点信号槽
        connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(focusChanged(QWidget *, QWidget *)));
        qApp->installEventFilter(this);
    }

    py.open(dbPath);
    readChinese();
}

void frmInput2019::btnClicked()
{
    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();
    QString btnText = btn->text();
    QString labText = ui->labPY->text();
    ui->scrollAreaCn->horizontalScrollBar()->setValue(0);
    ui->scrollAreaMore->verticalScrollBar()->setValue(0);

    if (objectName == "btnUpper") {
        upper = !upper;
        setUpper(upper);
        clearChinese();
        ui->labPY->clear();
    } else if (objectName == "btnNumber") {
        setInputType("number");
        
        number = false;
        setNumber(number);
    } else if (objectName == "btnSymbol") {
        number = !number;
        setNumber(number);
    } else if (objectName == "btnDelete" || objectName == "btnDelete2") {
        //如果当前是中文模式,则删除对应拼音,删除完拼音之后再删除对应文本输入框的内容
        int len = labText.length();
        if (inputType == "chinese" && len > 0) {
            ui->labPY->setText(labText.left(len - 1));
            selectChinese();
        } else {
            deleteValue();
        }

        ui->scrollAreaCn->horizontalScrollBar()->setValue(0);
        ui->scrollAreaMore->verticalScrollBar()->setValue(0);
    } else if (objectName == "btnSpace" || objectName == "btnSpace2") {
        //如果中文模式而且有待输入字母,判断是否有中文则插入第一个中文否则插入字母
        if (inputType == "chinese" && !labText.isEmpty()) {
            QString text = labCn.first()->text();
            text.isEmpty() ? insertValue(labText) : insertValue(text);
            clearChinese();
        } else {
            insertValue(" ");
        }
    } else if (objectName == "btnEnter" || objectName == "btnEnter2") {
        //如果中文模式而且有待输入字母则立即插入字母
        if (inputType == "chinese" && !labText.isEmpty()) {
            insertValue(labText);
            clearChinese();
        }

        if (currentWidget != 0 && !onlyControl) {
            //如果手动指定了属性隐藏,或者不是多行文本则自动隐藏
            //如果单行文本不希望回车隐藏则设置属性 ui->txt->setProperty("enterhide", false);
            QVariant enterHide = currentWidget->property("enterhide");
            if (enterHide.isValid()) {
                if (enterHide.toBool()) {
                    hidePanel();
                }
            } else {
                if (!currentWidget->inherits("QTextEdit") && !currentWidget->inherits("QPlainTextEdit") && !currentWidget->inherits("QTextBrowser")) {
                    hidePanel();
                }
            }
            //插入回车符
            QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier, QString("\n"));
            //发送回车信号
            QApplication::sendEvent(currentWidget, &keyPress);
        } else {
            insertValue("\n");
        }
        emit sendClearFocus();
    } else if (objectName == "btnClose" || objectName == "btnClose2") {
        hidePanel();
        emit sendClearFocus();
    } else {
        //如果是&按钮，因为对应&被过滤,所以真实的text为去除前面一个&字符
        if (btnText == "&&") {
            btnText = "&";
        }

        //当前不是中文模式,则单击按钮对应text为传递参数,大写优先
        if (inputType != "chinese" || upper) {
            insertValue(btnText);
        } else {
            if (btn->property("btnLetter").toBool()) {
                ui->labPY->setText(labText + btnText);
                selectChinese();
            }
        }
    }
}

void frmInput2019::focusChanged(QWidget *oldWidget, QWidget *nowWidget)
{
    //qDebug() << "oldWidget:" << oldWidget << "nowWidget:" << nowWidget;
    this->currentWidget = nowWidget;
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget)) {
        //如果对应属性noinput为真或者只读则不显示
        if (nowWidget->property("noinput").toBool() || nowWidget->property("readOnly").toBool()) {
            currentWidget = 0;
            QTimer::singleShot(0, this, SLOT(hidePanel()));
            return;
        }

        if (nowWidget->inherits("QWidget")) {
            //合法的输入控件,可以自行增加
            QStringList classNames;
            classNames << "QLineEdit" << "QTextEdit" << "QPlainTextEdit" << "QAbstractSpinBox" << "QComboBox";
            classNames << "QQuickWidget" << "QWebView" << "QtWebEngineCore::RenderWidgetHostViewQtDelegateWidget";

            //查找当前焦点控件是否属于合法输入的控件
            bool exist = false;
            foreach (QString className, classNames) {
                if (nowWidget->inherits(className.toLatin1().constData())) {
                    //如果当前是下拉框则判断下拉框可编辑属性是否为真
                    if (className != "QComboBox" || nowWidget->property("editable").toBool()) {
                        exist = true;
                        break;
                    }
                }
            }

            if (exist) {
                showPanel();
                movePosition();
            } else {
                currentWidget = 0;
                hidePanel();
            }
        }
    }
}

void frmInput2019::movePosition()
{
    //根据用户选择的输入法位置设置-居中显示-底部填充-显示在输入框正下方
    static int deskWidth = qApp->desktop()->availableGeometry().width();
    static int deskHeight = qApp->desktop()->availableGeometry().height();
    int width = this->width();
    int height = this->height();

    if (position == "center") {
        QPoint pos = QPoint(deskWidth / 2 - width / 2, deskHeight / 2 - height / 2);
        this->setGeometry(pos.x(), pos.y(), width, height);
    } else if (position == "bottom") {
        this->setGeometry(0, deskHeight - height, deskWidth, height);
    } else if (position == "control") {
        QRect rect = currentWidget->rect();
        QPoint pos = QPoint(rect.left(), rect.bottom() + 2);
        pos = currentWidget->mapToGlobal(pos);

        int x = pos.x();
        if (x + width > deskWidth) {
            x = deskWidth - width;
        }

        int y = pos.y();
        if (y + height > deskHeight) {
            y = y - height - rect.height() - 2;
        }

        this->setGeometry(x, y, width, height);
    }
}

void frmInput2019::selectChinese()
{
    //清空汉字
    clearChinese();

    //对当前的拼音查询汉字,如果超过最大数量的汉字则取前面
    QString labText = ui->labPY->text();
    int len = labText.length();
    if (len > 12) {
        qDebug() << "input too long";
        return;
    }

    int count = py.select(labText);
    count = count > maxCount ? maxCount : count;

    //插入用户造词汉字,优先级最高,插入在最前面
    int indexUser = userKey.indexOf(labText);
    if (indexUser >= 0) {
        QString chineses = userValue.at(indexUser);
        QStringList list = chineses.split("|");
        for (int i = list.count() - 1; i >= 0; i--) {
            QString chinese = list.at(i);
            if (!chinese.isEmpty()) {
                allPY.insert(0, chinese);
            }
        }
    }

    //插入用户选定的词,优先级中间,紧随用户造词其后
    int indexSelect = selectKey.indexOf(labText);
    if (indexSelect >= 0) {
        QString chineses = selectValue.at(indexSelect);
        QStringList list = chineses.split("|");
        for (int i = 0; i < list.count(); i++) {
            QString chinese = list.at(i);
            if (!chinese.isEmpty() && !allPY.contains(chinese)) {
                allPY << chinese;
            }
        }
    }

    //插入查询到的汉字,已经在汉字队列中的过滤
    for (int i = 0; i < count; i++) {
        QString text = py.getChinese(i);
        if (!text.isEmpty() && !allPY.contains(text)) {
            allPY << text;
        }
    }

    //显示汉字
    showChinese();
}

void frmInput2019::showChinese()
{
    //清空原有汉字
    for (int i = 0; i < maxCount; i++) {
        labCn.at(i)->clear();
        labMore.at(i)->clear();
        labCn.at(i)->setEnabled(false);
        labMore.at(i)->setEnabled(false);
    }

    //设置当前拼音对应的汉字
    for (int i = 0; i < allPY.count(); i++) {
        labCn.at(i)->setText(allPY.at(i));
        labMore.at(i)->setText(allPY.at(i));
        labCn.at(i)->setEnabled(true);
        labMore.at(i)->setEnabled(true);
        if (autoHide) {
            labCn.at(i)->setVisible(true);
            labMore.at(i)->setVisible(true);
        }
    }

    //将没有文字的标签设置不可见
    if (autoHide) {
        if (allPY.count() > 0) {
            for (int i = allPY.count() - 1; i < maxCount; i++) {
                labCn.at(i)->setVisible(false);
                labMore.at(i)->setVisible(false);
            }
        }
    }
}

void frmInput2019::clearChinese()
{
    allPY.clear();
    for (int i = 0; i < maxCount; i++) {
        labCn.at(i)->clear();
        labMore.at(i)->clear();
        labCn.at(i)->setEnabled(false);
        labMore.at(i)->setEnabled(false);
    }
}

void frmInput2019::readChinese()
{
    //载入本地优先词库
    selectKey.clear();
    selectValue.clear();
    QFile fileSelect(dbPath + "/chinese_select.txt");
    if (fileSelect.open(QFile::ReadOnly | QFile::Text)) {
        while(!fileSelect.atEnd()) {
            QString line = fileSelect.readLine();
            line = line.replace("\n", "");
            QStringList list = line.split(" ");
            selectKey << list.at(0);
            selectValue << list.at(1);
        }

        fileSelect.close();
    }

    //载入本地自定义词库
    userKey.clear();
    userValue.clear();
    QFile fileUser(dbPath + "/chinese_user.txt");
    if (fileUser.open(QFile::ReadOnly | QFile::Text)) {
        while(!fileUser.atEnd()) {
            QString line = fileUser.readLine();
            line = line.replace("\n", "");
            QStringList list = line.split(" ");
            userKey << list.at(0);
            userValue << list.at(1);
        }

        fileUser.close();
    }
}

void frmInput2019::saveChinese(const QString &value)
{
    //当前不处于中文输入状态不用处理
    if (inputType != "chinese" || value.isEmpty() || lastText.isEmpty()) {
        return;
    }

    //将当前选中汉字作为优先词库插入文件
    QString pinyin = ui->labPY->text();
    int index = selectKey.indexOf(pinyin);

    //如果当前字母已经在优先词库文件,则更新该字母对应的汉字
    if (index >= 0) {
        QStringList temp;
        QString chineses = selectValue.at(index);
        QStringList list = chineses.split("|");

        //已经存在则当前不需要加入,后面直接插入到最前面即可
        bool exist = list.contains(value);
        foreach (QString str, list) {
            if (exist && str == value) {
                continue;
            }
            if (!str.isEmpty()) {
                temp << str;
            }
        }

        temp.insert(0, value);
        selectValue[index] = temp.join("|");
    } else {
        selectKey << pinyin;
        selectValue << value;
    }

    QStringList list;
    int count = selectKey.count();
    for (int i = 0; i < count; i++) {
        list << QString("%1 %2").arg(selectKey.at(i)).arg(selectValue.at(i));
    }

    //重新保存优先词库文件
    QString data = list.join("\n");
    QFile file(dbPath + "/chinese_select.txt");
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(data.toUtf8());
    }
}

void frmInput2019::insertValue(const QString &value)
{
    //仅仅当做独立的面板使用,发送选择的内容即可
    if (onlyControl) {
        saveChinese(value);
        clearValue();
        emit receiveValue(value);
        return;
    }

    if (currentWidget == 0) {
        return;
    }

    //保存当前选中汉字到文件,作为优先显示的汉字
    saveChinese(value);

    //如果某个控件开启了永远大写则强制转换为大写
    QString text = currentWidget->property("upper").toBool() ? value.toUpper() : value;
    if (!text.isEmpty()) {
        QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(text));
        QApplication::sendEvent(currentWidget, &keyPress);
    }

    clearValue();
}

void frmInput2019::clearValue()
{
    //清空拼音标签,同时滚动条恢复
    ui->labPY->clear();
    ui->scrollAreaCn->horizontalScrollBar()->setValue(0);
    ui->scrollAreaMore->verticalScrollBar()->setValue(0);

    //如果是汉字面板显示则隐藏汉字面板并显示字母面板
    if (ui->widgetChinese->isVisible()) {
        ui->widgetLetter->setVisible(true);
        ui->widgetNumber->setVisible(false);
        ui->widgetChinese->setVisible(false);

        //重新设置图标
        QString strMore = ui->widgetMore->isVisible() ? "up" : "down";
        ui->labMore->setPixmap(QString(":/image/btn_%1_%2.png").arg(strMore).arg(iconType));
    }
}

void frmInput2019::deleteValue()
{
    if (currentWidget == 0) {
        return;
    }

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, QString());
    QApplication::sendEvent(currentWidget, &keyPress);
}

void frmInput2019::showPanel()
{
    if (!isEnabled()) {
        return;
    }

    //根据不同的标识符切换到不同的面板
    QString flag = currentWidget->property("flag").toString();
    if (currentWidget->inherits("QAbstractSpinBox")) {
        flag = "number";
    }

    //如果是父控件中的子控件需要过滤,比如表格控件自动出现的文本框,需要判断是否设置了number
    if (flag != "number") {
        QWidget *father = (QWidget *)currentWidget->parent();
        if (father != 0) {
            father = (QWidget *)father->parent();
            if (father != 0) {
                flag = father->property("flag").toString();
            }
        }
    }

    if (flag == "number") {
        if (inputType != "number") {
            this->setVisible(false);
            setInputType("number");
        }

        number = false;
        setNumber(number);
    } else {
        if (inputType == "number") {
            this->setVisible(false);
            setInputType("english");
        }
    }

    this->setVisible(true);
}

void frmInput2019::hidePanel()
{
    clearChinese();
    ui->labPY->clear();
    this->setVisible(false);
}

void frmInput2019::setFontInfo(const QString &fontName, int btnFontSize, int labFontSize)
{
    QFont btnFont(fontName, btnFontSize);
    QFont labFont(fontName, labFontSize);

    QList<QPushButton *> btns = this->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        btn->setFont(btnFont);
    }

    ui->labType->setFont(btnFont);
    ui->labType2->setFont(btnFont);
    ui->labPY->setFont(labFont);

    foreach (QLabel *lab, labCn) {
        lab->setFont(labFont);
    }

    foreach (QLabel *lab, labMore) {
        lab->setFont(labFont);
        lab->setFixedHeight(labFontSize + 30);
    }
}

void frmInput2019::setIconSize(int iconWidth, int iconHeight)
{
    QSize sizeUpper(iconWidth + 3, iconHeight);
    QSize sizeDelete(iconWidth + 10, iconHeight);
    QSize sizeEnter(iconWidth + 3, iconHeight);
    QSize sizeClose(iconWidth + 5, iconHeight + 3);
    ui->btnUpper->setIconSize(sizeUpper);
    ui->btnDelete->setIconSize(sizeDelete);
    ui->btnDelete2->setIconSize(sizeDelete);
    ui->btnEnter->setIconSize(sizeEnter);
//    ui->btnEnter2->setIconSize(sizeEnter);
    ui->btnClose->setIconSize(sizeClose);
    ui->btnClose2->setIconSize(sizeClose);
}

void frmInput2019::setSpacing(int spacing)
{
    ui->widgetLetter1->layout()->setSpacing(spacing);
    ui->widgetLetter2->layout()->setSpacing(spacing);
    ui->widgetLetter3->layout()->setSpacing(spacing);
    ui->widgetNumber1->layout()->setSpacing(spacing);
//    ui->widgetNumber2->layout()->setSpacing(spacing);
    ui->widgetNumber3->layout()->setSpacing(spacing);

    ui->widgetLetter->layout()->setSpacing(spacing);
    ui->widgetNumber->layout()->setSpacing(spacing);
}

void frmInput2019::setTopHeight(int topHeight)
{
    ui->widgetTop->setFixedHeight(topHeight);
}

void frmInput2019::setOnlyControl(bool onlyControl)
{
    this->onlyControl = onlyControl;
}

void frmInput2019::setAutoHide(bool autoHide)
{
    this->autoHide = autoHide;
}

void frmInput2019::setColumnCount(int columnCount)
{
    this->columnCount = columnCount;
}

void frmInput2019::setMaxCount(int maxCount)
{
    this->maxCount = maxCount;
}

void frmInput2019::setDbPath(const QString &dbPath)
{
    this->dbPath = dbPath;
}

void frmInput2019::setPosition(const QString &position)
{
    this->position = position;
}

void frmInput2019::setInputType(const QString &inputType)
{
    //每次模式更改都把原有的汉字清空
    clearChinese();
    ui->labPY->clear();
    ui->scrollAreaCn->horizontalScrollBar()->setValue(0);
    ui->scrollAreaMore->verticalScrollBar()->setValue(0);

    this->inputType = inputType;
    if (inputType == "number") {
        ui->widgetLetter->setVisible(false);
        ui->widgetNumber->setVisible(true);
        ui->widgetChinese->setVisible(false);
    } else if (inputType == "english") {
        ui->widgetLetter->setVisible(true);
        ui->widgetNumber->setVisible(false);
        ui->widgetChinese->setVisible(false);
   //     ui->labType->setText(QString("<font color='%1'>中/</font><font color='%2' size='4'>英</font>").arg(mainTextColor).arg(btnHoveColor));
             ui->labType->setText(QString("<font color='%1'>CH/</font><font color='%2' size='4'>EN</font>").arg(mainTextColor).arg(btnHoveColor));
    } else if (inputType == "chinese") {
        ui->widgetLetter->setVisible(true);
        ui->widgetNumber->setVisible(false);
        ui->widgetChinese->setVisible(false);
  //      ui->labType->setText(QString("<font color='%2' size='4'>中</font><font color='%1'>/英</font>").arg(mainTextColor).arg(btnHoveColor));
              ui->labType->setText(QString("<font color='%2' size='4'>CH</font><font color='%1'>/EN</font>").arg(mainTextColor).arg(btnHoveColor));
    }

    //重新设置图标
    QString strMore = ui->widgetMore->isVisible() ? "up" : "down";
    ui->labMore->setPixmap(QString(":/image/btn_%1_%2.png").arg(strMore).arg(iconType));
}

void frmInput2019::setUpper(bool isUpper)
{
    QList<QPushButton *> btns = ui->widgetLetter->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        QString text = isUpper ? btn->text().toUpper() : btn->text().toLower();
        btn->setText(text);
    }

    //改变图标
    QString strUpper = upper ? "upper" : "lower";
    ui->btnUpper->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg(strUpper).arg(iconType)));
}

void frmInput2019::setNumber(bool isNumber)
{
    //可自行更改特殊符号
    QStringList chars;

    if(false != isNumber)
    {
        ui->btnSymbol->setText("123");
        chars << "?" << ":" << ";" << "/" << "&&" << "%" << "+" << "(" << ")" << "-" << "<" << ">" << "!" << "\"" << "_";
    }
    else
    {
        ui->btnSymbol->setText("!&&%");
        chars << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "." << "*" << "," << "@" << "#";
    }
        
    ui->btn0->setText(chars.at(0));
    ui->btn1->setText(chars.at(1));
    ui->btn2->setText(chars.at(2));
    ui->btn3->setText(chars.at(3));
    ui->btn4->setText(chars.at(4));
    ui->btn5->setText(chars.at(5));
    ui->btn6->setText(chars.at(6));
    ui->btn7->setText(chars.at(7));
    ui->btn8->setText(chars.at(8));
    ui->btn9->setText(chars.at(9));
    ui->btn10->setText(chars.at(10));
    ui->btn11->setText(chars.at(11));
    ui->btn12->setText(chars.at(12));
    ui->btn13->setText(chars.at(13));
    ui->btn14->setText(chars.at(14));
}

void frmInput2019::setStyleName(const QString &styleName)
{
    this->styleName = styleName;
    this->iconType = "white";
    if (styleName == "black") {
        setColor("#191919", "#F3F3F3", "#313131", "#24B1DF", "#F3F3F3", "#F95717", "#F3F3F3");
    } else if (styleName == "blue") {
        setColor("#377FC9", "#F3F3F3", "#4189D3", "#386487", "#F3F3F3", "#386487", "#F3F3F3");
    } else if (styleName == "gray") {
        setColor("#989898", "#FFFFFF", "#A2A2A2", "#333333", "#F3F3F3", "#2E2E2E", "#F3F3F3");
    } else if (styleName == "brown") {
        setColor("#667481", "#F3F3F3", "#566373", "#4189D3", "#F3F3F3", "#4189D3", "#F3F3F3");
    } else if (styleName == "silvery") {
        setColor("#868690", "#000002", "#C3C2C7", "#F0F0F0", "#000002", "#F0F0F0", "#000002");
        this->iconType = "black";
    }

    QString strMore = ui->widgetMore->isVisible() ? "up" : "down";
    QString strUpper = upper ? "upper" : "lower";
    ui->labMore->setPixmap(QString(":/image/btn_%1_%2.png").arg(strMore).arg(iconType));
    ui->btnUpper->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg(strUpper).arg(iconType)));
    ui->btnDelete->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg("delete").arg(iconType)));
    ui->btnDelete2->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg("delete").arg(iconType)));
    ui->btnEnter->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg("enter").arg(iconType)));
    //ui->btnEnter2->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg("enter").arg(iconType)));
    ui->btnClose->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg("close").arg(iconType)));
    ui->btnClose2->setIcon(QIcon(QString(":/image/btn_%1_%2.png").arg("close").arg(iconType)));
}

void frmInput2019::setColor(const QString &mainBackgroundColor, const QString &mainTextColor,
                            const QString &btnBackgroundColor, const QString &btnHoveColor,
                            const QString &btnHoveTextColor, const QString &labHoveColor,
                            const QString &labHoveTextColor)
{
    this->mainTextColor = mainTextColor;
    this->btnHoveColor = btnHoveColor;

    QStringList qss;
    qss.append(QString("QScrollArea{border:none;background:rgba(255,255,255,0);}QWidget#widgetCn,QWidget#widgetMore{background:rgba(0,0,0,0);}"));
    qss.append(QString("QWidget#widgetTop,QWidget#widgetLetter,QWidget#widgetNumber,QWidget#widgetChinese{background-color:%1;}").arg(mainBackgroundColor));
    qss.append(QString("#labType,#labType2,QPushButton{border-width:0px;border-radius:3px;color:%1;}").arg(mainTextColor));
    qss.append(QString("#labType,#labType2,QPushButton{padding:0px;background-color:%1;}").arg(btnBackgroundColor));
    //如果仅仅是触摸屏请用下面这句
    qss.append(QString("QPushButton:pressed{background-color:%1;color:%2;}").arg(btnHoveColor).arg(btnHoveTextColor));
 //   qss.append(QString("QPushButton:pressed,QPushButton:hover{background-color:%1;color:%2;}").arg(btnHoveColor).arg(btnHoveTextColor));
    qss.append(QString("QLabel{padding:0px 5px 0px 5px;border-width:0px;border-radius:3px;color:%1;background:none;}").arg(mainTextColor));
    qss.append(QString("QLabel:pressed,QLabel:hover{background-color:%1;color:%2;}").arg(labHoveColor).arg(labHoveTextColor));
    qss.append(QString("QLabel:disabled{background:none;}"));
    this->setStyleSheet(qss.join(""));

    // ui->labType->setText(QString("<font color='%1'>中/</font><font color='%2' size='4'>英</font>").arg(mainTextColor).arg(btnHoveColor));
    // ui->labType2->setText(QString("<font color='%2' size='4'>中</font><font color='%1'>/英</font>").arg(mainTextColor).arg(btnHoveColor));
    ui->labType->setText(QString("<font color='%1'>CH/</font><font color='%2' size='4'>EN</font>").arg(mainTextColor).arg(btnHoveColor));
    // ui->labType2->setText(QString("<font color='%2' size='4'>CH</font><font color='%1'>/EN</font>").arg(mainTextColor).arg(btnHoveColor));
    ui->labType2->setText(QString("<font color='%1'>TYPE</font>").arg(btnHoveColor));
    qApp->processEvents();
}
