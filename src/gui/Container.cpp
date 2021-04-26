//
// Created by helywin on 2021/4/26.
//

#include "Container.hpp"

/*
class EventListener : public QObject
{
Q_OBJECT
public:
    explicit EventListener(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void eventOccurred(QObject *watched, QEvent *event);
};
*/

class ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(Container)
    Container *q_ptr;

    explicit ContainerPrivate(Container *p);
};

ContainerPrivate::ContainerPrivate(Container *p) :
        q_ptr(p)
{

}

Container::Container(QWidget *parent) :
        QFrame(parent),
        d_ptr(new ContainerPrivate(this))
{
    Q_D(Container);
//    connect(d->mEventListener, &EventListener::eventOccurred,
//            [this](QObject *watched, QEvent *event) {
//                onEventOccurred(watched, event);
//            });
}

Container::~Container()
{

}

void Container::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{

}

void Container::setListenGlobalMouseEvent(bool enable)
{

}

void Container::onGlobalMouseEvent(QEvent::Type type, Qt::MouseButton button)
{

}

/*EventListener::EventListener(QObject *parent) : QObject(parent)
{

}

bool EventListener::eventFilter(QObject *watched, QEvent *event)
{
    emit eventOccurred(watched, event);
    return false;
}*/

#include "Container.moc"