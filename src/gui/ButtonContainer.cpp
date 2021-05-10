//
// Created by jiang on 2021/4/30.
//

#include "ButtonContainer.hpp"
#include "Container_p.hpp"
#include <QDebug>
#include <QAbstractButton>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include <cassert>


class ButtonContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(ButtonContainer)
    QString mMenuArrow;

    explicit ButtonContainerPrivate(ButtonContainer *p);
};

ButtonContainerPrivate::ButtonContainerPrivate(ButtonContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(ButtonContainer);
}

ButtonContainer::ButtonContainer(QWidget *parent) :
        Container(*new ButtonContainerPrivate(this), parent)
{
    Q_D(ButtonContainer);
}

ButtonContainer::~ButtonContainer()
{

}

void ButtonContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(ButtonContainer);
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

void ButtonContainer::setListenWidget(QWidget *w)
{
    Q_D(ButtonContainer);
    Container::setListenWidget(w);
    auto ab = dynamic_cast<QAbstractButton *>(w);
    assert(ab);
    if (ab->isEnabled()) {
        ControlStates states(cs_none);
        states |= cs_pressed;
        states |= cs_hover;
        if (!dynamic_cast<QToolButton *>(ab)) {
            states |= cs_focus;
        }
        if (ab->isCheckable()) {
            states |= cs_checked;
            connect(ab, &QAbstractButton::clicked, [this](bool checked) {
                d_ptr->mainStateDisplay()->setState(cs_checked, checked);
            });
        }
        d->addControlStateDisplay(w->metaObject()->className(), states, true);
    } else {
        d->addControlStateDisplay(w->metaObject()->className(), cs_disabled, true);
    }

    auto pushButton = dynamic_cast<QPushButton *>(w);
    if (pushButton) {
        if (pushButton->menu()) {
            pushButton->menu()->installEventFilter(this);
        }
        d->mMenuArrow = "QPushButton::menu-arrow";
        d->addControlStateDisplay(d->mMenuArrow, cs_none, false);
    }

    if (!pushButton || !pushButton->menu()) {
        connect(ab, &QAbstractButton::pressed, [this] {
            d_ptr->mainStateDisplay()->setState(cs_pressed, true);
        });
        connect(ab, &QAbstractButton::released, [this] {
            d_ptr->mainStateDisplay()->setState(cs_pressed, false);
        });
    }

}
