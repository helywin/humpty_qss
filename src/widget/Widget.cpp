//
// Created by helywin on 2020/9/5.
//

#include "Widget.hpp"
#include <QWidget>
#include <QStyleOption>
#include <QPainter>

namespace Com
{


void drawPrimitive(QWidget *w)
{
    QStyleOption opt;
    opt.initFrom(w);
    QPainter p(w);
    w->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, w);
}

}