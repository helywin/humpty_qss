//
// Created by helywin on 2021/4/26.
//

#ifndef HUMPTY_QSS_GLOBALMOUSELISTENER_HPP
#define HUMPTY_QSS_GLOBALMOUSELISTENER_HPP

#include <QObject>
#include <QEvent>

class GlobalMouseListener : public QObject
{
Q_OBJECT
public:
    static GlobalMouseListener *instance();

private:
    GlobalMouseListener() = default;

signals:
    void mouseEvent(QEvent::Type type, Qt::MouseButton button);
};


#endif //HUMPTY_QSS_GLOBALMOUSELISTENER_HPP
