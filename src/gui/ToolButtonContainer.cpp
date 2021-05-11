//
// Created by jiang on 2021/4/30.
//

#include "ToolButtonContainer.hpp"
#include "Container_p.hpp"
#include <QDebug>
#include <QAbstractButton>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include <cassert>


class ToolButtonContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(ToolButtonContainer)

    explicit ToolButtonContainerPrivate(ToolButtonContainer *p);
};

ToolButtonContainerPrivate::ToolButtonContainerPrivate(ToolButtonContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(ToolButtonContainer);
}

ToolButtonContainer::ToolButtonContainer(QWidget *parent) :
        Container(*new ToolButtonContainerPrivate(this), parent)
{
    Q_D(ToolButtonContainer);
}

ToolButtonContainer::~ToolButtonContainer()
{

}

void ToolButtonContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(ToolButtonContainer);
    if (watched->metaObject()->className() == QString("QMenu")) {
        if (event->type() == QEvent::Hide) {
            qDebug() << "hide";
            d->mainStateDisplay()->setState(cs_pressed, false);
        } else if (event->type() == QEvent::Show) {
            d->mainStateDisplay()->setState(cs_pressed, true);
        }
        return;
    }
    switch (event->type()) {
        case QEvent::Enter:
            d->mainStateDisplay()->setState(cs_hover, true);
            break;
        case QEvent::Leave:
            d->mainStateDisplay()->setState(cs_hover, false);
            break;
        case QEvent::FocusIn:
            d->mainStateDisplay()->setState(cs_focus, true);
            break;
        case QEvent::FocusOut:
            d->mainStateDisplay()->setState(cs_focus, false);
            break;
        default:
            break;
    }
}

void ToolButtonContainer::setListenWidget(QWidget *w)
{
    Q_D(ToolButtonContainer);
    Container::setListenWidget(w);
    auto ab = dynamic_cast<QToolButton *>(w);
    assert(ab);
    if (ab->isEnabled()) {
        ControlStates states(cs_none);
        states |= cs_pressed;
        states |= cs_hover;
        if (ab->isCheckable()) {
            states |= cs_checked;
            connect(ab, &QToolButton::clicked, [this](bool checked) {
                d_ptr->mainStateDisplay()->setState(cs_checked, checked);
            });
        }
        d->addControlStateDisplay(w->metaObject()->className(), states, true);
    } else {
        d->addControlStateDisplay(w->metaObject()->className(), cs_disabled, true);
    }

    connect(ab, &QToolButton::pressed, [this] {
        d_ptr->mainStateDisplay()->setState(cs_pressed, true);
    });
    connect(ab, &QToolButton::released, [this] {
        d_ptr->mainStateDisplay()->setState(cs_pressed, false);
    });

}
