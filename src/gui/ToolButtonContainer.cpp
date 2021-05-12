//
// Created by jiang on 2021/4/30.
//

#include "ToolButtonContainer.hpp"
#include "Container_p.hpp"
#include <QDebug>
#include <QToolButton>
#include <QMenu>
#include <cassert>


class ToolButtonContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(ToolButtonContainer)
    QString mMenuArrow;

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
        default:
            break;
    }
}

void ToolButtonContainer::setListenWidget(QWidget *w)
{
    Q_D(ToolButtonContainer);
    Container::setListenWidget(w);
    auto toolButton = dynamic_cast<QToolButton *>(w);
    assert(toolButton);
    ControlStates states(cs_none);
    if (toolButton->isEnabled()) {
        states |= cs_pressed;
        states |= cs_hover;
        if (toolButton->isCheckable()) {
            states |= cs_checked;
            connect(toolButton, &QToolButton::clicked, [this](bool checked) {
                d_ptr->mainStateDisplay()->setState(cs_checked, checked);
            });
        }
        d->addControlStateDisplay(w->metaObject()->className(), states, true);
    } else {
        d->addControlStateDisplay(w->metaObject()->className(), cs_disabled, true);
    }


    if (toolButton->menu()) {
        toolButton->menu()->installEventFilter(this);
        d->mMenuArrow = "QToolButton::menu-indicator";
        states = cs_pressed;
        states |= cs_hover;
        d->addControlStateDisplay(d->mMenuArrow, states, false);
    }

    connect(toolButton, &QToolButton::pressed, [this] {
        d_ptr->mainStateDisplay()->setState(cs_pressed, true);
    });
    connect(toolButton, &QToolButton::released, [this] {
        d_ptr->mainStateDisplay()->setState(cs_pressed, false);
    });

}
