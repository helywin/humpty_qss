//
// Created by helywin on 2021/4/26.
//

#include "Container.hpp"

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

void ContainerPrivate::addControlStateDisplay(const QString &name, ControlStates states)
{
    assert(!mStateDisplayList.contains(name));
    auto display = new StateDisplay(mStatesContainer);
    mStateDisplayList[name] = display;
    display->setAllStates(states);
}

StateDisplay *ContainerPrivate::stateDisplay(const QString &name)
{
    assert(!mStateDisplayList.contains(name));
    return mStateDisplayList[name];
}

StateDisplay *ContainerPrivate::mainStateDisplay()
{
    assert(!mStateDisplayList.contains(mListenWidget->metaObject()->className()));
    return mStateDisplayList[mListenWidget->metaObject()->className()];;
}

void ContainerPrivate::setListenWidget(QWidget *w)
{
    Q_Q(Container);
    mListenWidget = w;
    w->installEventFilter(q);
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

void Container::onGlobalMouseEvent(QEvent::Type type, Qt::MouseButton button)
{

}

void Container::setListenWidget(QWidget *w)
{
    Q_D(Container);
    d->mListenWidget = w;
    w->installEventFilter(this);
}

void Container::setWidget(QWidget *w, WidgetPosition wp)
{
    Q_D(Container);
    d->mLayout->insertWidget(wp, w);
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

QWidget *Container::listened()
{
    Q_D(Container);
    return d->mListenWidget;
}

/*EventListener::EventListener(QObject *parent) : QObject(parent)
{

}

bool EventListener::eventFilter(QObject *watched, QEvent *event)
{
    emit eventOccurred(watched, event);
    return false;
}*/