//
// Created by helywin on 2020/8/17.
//

#ifndef SOLAR_PRO_WIDGETCOM_HPP
#define SOLAR_PRO_WIDGETCOM_HPP

#include <utility>
#include <QFlags>
#include <QEvent>
#include <QStringList>
#include <QMetaEnum>
#include "Utils_Export.hpp"

class QWidget;
class QObject;
class QSettings;
class QVariant;
class QString;

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

template<typename WPP, size_t N, size_t K, typename WP>
inline void initWidgets(WPP (&w)[N][K], WP *wp)
{
    using W = typename std::remove_pointer<WPP>::type;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            w[i][j] = new W(wp);
        }
    }
}

COM_UTILS_API void drawPrimitive(QWidget *w);
COM_UTILS_API void setIfNotExists(QSettings *settings, const QString &key, const QVariant &variant);

enum EventBlockFlag
{
    eb_mouse,
    eb_key,
};

Q_DECLARE_FLAGS(EventBlockFlags, EventBlockFlag)
Q_DECLARE_FLAGS(EventFlags, QEvent::Type)

COM_UTILS_API void blockEvent(QWidget *widget, EventBlockFlags flags, QObject *parent);
COM_UTILS_API void blockEvent(QWidget *widget, EventFlags flags, QObject *parent);
COM_UTILS_API void changeFonts(QWidget *widget, const QString &family);

template<typename T>
inline QString enumToString(T e)
{
    static_assert(std::is_enum_v<T>);
    if (QMetaEnum::fromType<T>().isFlag()) {
        return QMetaEnum::fromType<T>().valueToKeys(e);
    } else {
        return QMetaEnum::fromType<T>().valueToKey(e);
    }
}

template<typename T>
inline QStringList flagsToStringList(QFlags<T> flags)
{
    static_assert(std::is_enum_v<T>);
    int keyCount = QMetaEnum::fromType<T>().keyCount();
    QStringList result;
    for (int i = 0; i < keyCount; ++i) {
        const char *key = QMetaEnum::fromType<T>().key(i);
        if (flags.testFlag((T)QMetaEnum::fromType<T>().value(i))) {
            result.append(key);
        }
    }
    return result;
}

}   // namespace Com

#endif   //SOLAR_PRO_WIDGETCOM_HPP
