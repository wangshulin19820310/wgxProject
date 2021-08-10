#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QDebug>
#include "include.h"
class CustomTabStyle : public QProxyStyle
{

public:

    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                            const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab)
        {
            s.transpose();
            s.rwidth() = 150; // 设置每个tabBar中item的大小
            s.rheight() = 80;
        }
        return s;
    }


   void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        QPen pen,pen1;       
        pen.setWidth(5);
        pen.setColor(QColor(0x2966ff));
        pen1.setWidth(15);
        pen1.setColor(QColor(0xffffff));
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
                {
                    QRect allRect = tab->rect;                    
                    if (tab->state & QStyle::State_Selected)
                    {
                         painter->save();                      
                          painter->setPen(pen1);
                          painter->setBrush(QBrush(0xffffff));
                          painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                          painter->restore();

                          painter->setPen(pen);
                          painter->drawLine(allRect.bottomLeft(),allRect.bottomRight());
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);                
                if (tab->state & QStyle::State_Selected)
                {
                   // painter->setPen(0xf8fcff);
                      painter->setPen(0x2966ff);
                }
                else
                {
                    painter->setPen(0x999999);
                }                 
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }
        if (element == CE_TabBarTab)
        {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }

};

#endif // CUSTOMTABSTYLE_H
