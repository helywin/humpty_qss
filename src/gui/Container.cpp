//
// Created by helywin on 2021/4/26.
//

#include "Container.hpp"
#include <cassert>

#include "StateDisplay.hpp"
#include "Utils.hpp"
#include "GlobalMouseListener.hpp"
#include "Container_p.hpp"

using namespace Com;

ContainerPrivate::ContainerPrivate(Container *p) :
        q_ptr(p)
{
    Q_Q(Container);
    initWidget(mLayout, q);
    initWidget(mTitle, q);
    initWidget(mStatesContainer, q);
    initWidget(mStatesLayout, mStatesContainer);
    mStatesLayout = new QFormLayout;
    mLayout->addWidget(mTitle);
    mLayout->addWidget(mStatesContainer);
}

Container::Container(QWidget *parent) :
        QFrame(parent),
        d_ptr(new ContainerPrivate(this))
{
    Q_D(Container);
}

Container::Container(ContainerPrivate &d, QWidget *parent) :
        QFrame(parent),
        d_ptr(&d)
{

}


Container::~Container()
{

}

void Container::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{

}

#ifdef _WINDOWS
void Container::setListenGlobalMouseEvent(bool enable)
{
    Q_D(Container);
    if (enable == d->mListenGlobalMouseEvent) {
        return;
    } else if (enable) {
        connect(GlobalMouseListener::instance(), &GlobalMouseListener::mouseEvent,
                this, &Container::onGlobalMouseEvent);
    } else {
        disconnect(GlobalMouseListener::instance(), &GlobalMouseListener::mouseEvent,
                   this, &Container::onGlobalMouseEvent);
    }
    d->mListenGlobalMouseEvent = enable;
}
#endif

void Container::onGlobalMouseEvent(QEvent::Type type, Qt::MouseButton button)
{

}

void Container::setListenWidget(QWidget *w)
{
    Q_D(Container);
    d->mListenWidget = w;
    w->installEventFilter(this);
}

bool Container::eventFilter(QObject *watched, QEvent *event)
{
    auto w = dynamic_cast<QWidget *>(watched);
    if (w) {
        onListenedWidgetEventOccurred(w, event);
    }
    return QObject::eventFilter(watched, event);
}

QStandardItemModel *Container::objectTree()
{
    Q_D(Container);
    if (!d->mListenWidget) {
        return nullptr;
    }
    return nullptr;
}

void Container::setWidget(QWidget *w, WidgetPosition wp)
{
    Q_D(Container);
    d->mLayout->insertWidget(wp, w);
}

void Container::addControlStateDisplay(const QString &name, ControlStates states)
{
    Q_D(Container);
    assert(!d->mStateDisplayList.contains(name));
    auto display = new StateDisplay(d->mStatesContainer);
    d->mStateDisplayList[name] = display;
    display->setAllStates(states);
}

StateDisplay *Container::stateDisplay(const QString &name)
{
    Q_D(Container);
    assert(!d->mStateDisplayList.contains(name));
    return d->mStateDisplayList[name];
}

/*EventListener::EventListener(QObject *parent) : QObject(parent)
{

}

bool EventListener::eventFilter(QObject *watched, QEvent *event)
{
    emit eventOccurred(watched, event);
    return false;
}*/