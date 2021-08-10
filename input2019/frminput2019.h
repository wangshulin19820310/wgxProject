#ifndef FRMINPUT2019_H
#define FRMINPUT2019_H

#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

#include "googlepinyin.h"

namespace Ui
{
    class frmInput2019;
}

class frmInput2019 : public QWidget
{
    Q_OBJECT

public:
    //单例模式,保证一个程序只存在一个输入法实例对象
    static frmInput2019 *Instance();
    explicit frmInput2019(QWidget *parent = 0);
    ~frmInput2019();

protected:
    void showEvent(QShowEvent *);
    //事件过滤器
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::frmInput2019 *ui;
    static frmInput2019 *self;

    //当前大小写状态
    bool upper;
    //当前数字字符状态
    bool number;
    //是否仅仅当做控件使用
    bool onlyControl;
    //是否自动隐藏没有汉字的标签
    bool autoHide;
    //汉字面板最大列数
    int columnCount;
    //显示汉字的标签的最大数量
    int maxCount;

    //数据库路径
    QString dbPath;
    //当前输入法面板位置类型
    QString position;
    //当前输入法类型
    QString inputType;
    //当前样式颜色
    QString styleName;
    //当前样式对应图标的类型
    QString iconType;

    //当前焦点的对象
    QWidget *currentWidget;
    //顶部汉字标签数组
    QList<QLabel *>labCn;
    //更多汉字标签数组
    QList<QLabel *>labMore;
    //所有拼音链表
    QStringList allPY;
    //谷歌拼音内核
    GooglePinYin py;

    //记住颜色用来设置中英切换
    QString mainTextColor;
    QString btnHoveColor;

    //记住最后按下的汉字
    QString lastText;

    //动画滑动
    QPropertyAnimation *animationCn;
    QPropertyAnimation *animationMore;

    //存储本地选中的字库文件,chinese_select.txt
    QStringList selectKey, selectValue;
    //存储本地用户自定义的字库文件,chinese_user.txt
    QStringList userKey, userValue;

private slots:
    //初始化窗体数据
    void initForm();
    //初始化数据库以及其他
    void init();

    //输入法面板按键处理
    void btnClicked();

    //焦点改变事件槽函数处理
    void focusChanged(QWidget *oldWidget, QWidget *nowWidget);
    //移动到合适的位置
    void movePosition();

    //查询汉字
    void selectChinese();
    //显示查询到的汉字
    void showChinese();
    //清空当前汉字信息
    void clearChinese();

    //读取优先词库文件
    void readChinese();
    //保存当前选中汉字到文件
    void saveChinese(const QString &value);

    //插入值到当前焦点控件
    void insertValue(const QString &value);
    //清空值,恢复到初始状态
    void clearValue();
    //删除当前焦点控件的一个字符
    void deleteValue();

    //显示输入法面板
    void showPanel();
    //隐藏输入法面板
    void hidePanel();

public Q_SLOTS:
    //设置字体名称及大小,包括按钮的字体大小和汉字部分的字体大小
    void setFontInfo(const QString &fontName, int btnFontSize, int labFontSize);
    //设置图标尺寸
    void setIconSize(int iconWidth, int iconHeight);
    //设置按钮之间的空隙间隔
    void setSpacing(int spacing);
    //设置顶部汉字区域高度
    void setTopHeight(int topHeight);

    //设置仅仅当做控件使用
    void setOnlyControl(bool onlyControl);
    //设置是否自动隐藏没有汉字的标签
    void setAutoHide(bool autoHide);
    //设置汉字面板列数
    void setColumnCount(int columnCount);
    //设置最大汉字标签数量
    void setMaxCount(int maxCount);

    //设置字库路径
    void setDbPath(const QString &dbPath);
    //设置输入法弹出位置
    void setPosition(const QString &position);
    //设置当前输入法类型
    void setInputType(const QString &inputType);
    //改变字母大小写
    void setUpper(bool isUpper);
    //改变数字字符
    void setNumber(bool isNumber);

    //设置输入法颜色样式
    void setStyleName(const QString &styleName);
    //改变输入法面板样式
    void setColor(const QString &mainBackgroundColor, const QString &mainTextColor,
                  const QString &btnBackgroundColor, const QString &btnHoveColor,
                  const QString &btnHoveTextColor, const QString &labHoveColor,
                  const QString &labHoveTextColor);

signals:
    void receiveValue(const QString &value);
    void sendClearFocus();
};

#endif // FRMINPUT2019_H
