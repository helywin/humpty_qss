//
// Created by helywin on 2021/4/26.
//

#include "Container.hpp"
#include <cassert>
#include <QDebug>

#include "StateDisplay.hpp"
#include "Utils.hpp"
#include "GlobalMouseListener.hpp"
#include "Container_p.hpp"

using namespace Com;

ContainerPrivate::ContainerPrivate(Container *p) :
        q_ptr(p)
{

}

void ContainerPrivate::init()
{
    Q_Q(Container);
    initWidget(mLayout, q);
    initWidget(mTitle, q);
    initWidget(mStatesContainer, q);
//    initWidget(mStatesLayout, mStatesContainer);
    mStatesLayout = new QFormLayout;
    mStatesContainer->setLayout(mStatesLayout);
    mLayout->addWidget(mTitle);
    mLayout->addWidget(mStatesContainer);
}


void ContainerPrivate::addControlStateDisplay(const QString &name, ControlStates states,
                                              bool isMainControl)
{
    assert(!mStateDisplayList.contains(name));
    auto display = new StateDisplay(mStatesContainer);
    if (isMainControl) {
        mMainControlName = name;
    }
    mStateDisplayList[name] = display;
    display->setAllStates(states);
    mStatesLayout->addRow(name, display);
}

StateDisplay *ContainerPrivate::stateDisplay(const QString &name)
{
    assert(mStateDisplayList.contains(name));
    return mStateDisplayList[name];
}

StateDisplay *ContainerPrivate::mainStateDisplay()
{
    assert(!mMainControlName.isEmpty() && mStateDisplayList.contains(mMainControlName));
    return mStateDisplayList[mMainControlName];;
}

void ContainerPrivate::setListenWidget(QWidget *w)
{
    Q_Q(Container);
    mListenWidget = w;
    mListenWidget->setProperty("test", true);
    if (w->isEnabled()) {
        w->installEventFilter(q);
    }
}

Container::Container(QWidget *parent) :
        QFrame(parent),
        d_ptr(new ContainerPrivate(this))
{
    Q_D(Container);
}

Container::Container(ContainerPrivate &dd, QWidget *parent) :
        QFrame(parent),
        d_ptr(&dd)
{
    Q_D(Container);
    d->init();
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
    d->setListenWidget(w);
}

void Container::setWidget(QWidget *w, WidgetPosition wp)
{
    Q_D(Container);
    d->mLayout->insertWidget(wp, w, 0, Qt::AlignCenter);
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