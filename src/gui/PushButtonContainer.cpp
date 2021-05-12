//
// Created by jiang on 2021/5/11.
//

#include "PushButtonContainer.hpp"
#include "Container_p.hpp"
#include <QDebug>
#include <QAbstractButton>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include <cassert>
#include <QStyleOptionButton>


class PushButtonContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(PushButtonContainer)
    QString mMenuArrow;

    explicit PushButtonContainerPrivate(PushButtonContainer *p);
};

PushButtonContainerPrivate::PushButtonContainerPrivate(PushButtonContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(PushButtonContainer);
}

PushButtonContainer::PushButtonContainer(QWidget *parent) :
        Container(*new PushButtonContainerPrivate(this), parent)
{
    Q_D(PushButtonContainer);
}

PushButtonContainer::~PushButtonContainer()
{

}

void PushButtonContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(PushButtonContainer);
    if (dynamic_cast<QMenu *>(watched)) {
        if (event->type() == QEvent::Hide) {
            qDebug() << "hide";
            d->mainStateDisplay()->setState(cs_pressed, false);
        } else if (event->type() == QEvent::Show) {
            d->mainStateDisplay()->setState(cs_pressed, true);
        }
        return;
    }
    auto pushButton = dynamic_cast<QPushButton *>(watched);
    if (!pushButton) {
        return;
    }
    QStyleOptionButton opt;
    opt.features;
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
        case QEvent::HoverMove:
            if (pushButton->menu()) {

            }
            break;
        default:
            break;
    }
}

void PushButtonContainer::setListenWidget(QWidget *w)
{
    Q_D(PushButtonContainer);
    Container::setListenWidget(w);
    auto pushButton = dynamic_cast<QPushButton *>(w);
    assert(pushButton);
    ControlStates states(cs_none);
    if (pushButton->isEnabled()) {
        states |= cs_pressed;
        states |= cs_hover;
        states |= cs_focus;

        if (pushButton->isCheckable()) {
            states |= cs_checked;
            connect(pushButton, &QPushButton::clicked, [this](bool checked) {
                d_ptr->mainStateDisplay()->setState(cs_checked, checked);
            });
        }
        d->addControlStateDisplay(w->metaObject()->className(), states, true);
    } else {
        d->addControlStateDisplay(w->metaObject()->className(), cs_disabled, true);
    }


    if (pushButton->menu()) {
        pushButton->menu()->installEventFilter(this);
        d->mMenuArrow = "QPushButton::menu-indicator";
        states = cs_pressed;
        states |= cs_hover;
        d->addControlStateDisplay(d->mMenuArrow, states, false);
    }

    if (!pushButton->menu()) {
        connect(pushButton, &QPushButton::pressed, [this] {
            d_ptr->mainStateDisplay()->setState(cs_pressed, true);
        });
        connect(pushButton, &QPushButton::released, [this] {
            d_ptr->mainStateDisplay()->setState(cs_pressed, false);
        });
    }

}
