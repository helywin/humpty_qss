//
// Created by helywin on 2020/8/17.
//

#ifndef SOLAR_PRO_WIDGETCOM_HPP
#define SOLAR_PRO_WIDGETCOM_HPP

#include <utility>
#include "Widget_Export.hpp"

class QWidget;
class QObject;

namespace Com
{

// 几个偷懒的公用函数, 用来初始化控件

template<typename W, typename WP>
inline void initWidget(W *&w, WP *wp)
{
    w = new W(wp);
}

template<typename O>
inline void initObject(O *&o, QObject *op)
{
    o = new O(op);
}

template<typename WPP, size_t N, typename WP>
inline void initWidgets(WPP (&w)[N], WP *wp)
{
    using W = typename std::remove_pointer<WPP>::type;
    for (int i = 0; i < N; ++i) {
        w[i] = new W(wp);
    }
}

COM_WIDGET_API void drawPrimitive(QWidget *w);

}

#endif //SOLAR_PRO_WIDGETCOM_HPP
