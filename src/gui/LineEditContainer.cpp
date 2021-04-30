//
// Created by jiang on 2021/4/30.
//

#include "LineEditContainer.hpp"
#include <QLineEdit>
#include "Container_p.hpp"

class LineEditContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(LineEditContainer)

    explicit LineEditContainerPrivate(LineEditContainer *p);
};

LineEditContainerPrivate::LineEditContainerPrivate(LineEditContainer *p) :
        ContainerPrivate(p)
{

}

LineEditContainer::LineEditContainer(QWidget *parent) :
        Container(*new LineEditContainerPrivate(this), parent)
{

}

LineEditContainer::~LineEditContainer()
{

}

void LineEditContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(LineEditContainer);
    switch (event->type()) {
        case QEvent::FocusIn:
            d->mainStateDisplay()->setState(cs_focus, true);
            break;
        case QEvent::FocusOut:
            d->mainStateDisplay()->setState(cs_focus, false);
            break;
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

void LineEditContainer::setListenWidget(QWidget *w)
{
    Q_D(LineEditContainer);
    Container::setListenWidget(w);
    auto lineEdit = dynamic_cast<QLineEdit *>(w);
    QString name = w->metaObject()->className();
    ControlStates states;
    states |= cs_hover;
    states |= cs_focus;
    if (lineEdit->isReadOnly()) {
        name += "[read-only]";
    } else if (lineEdit->echoMode() == QLineEdit::Password) {
        name += "[echoMode=\"2\"]";
    }
    if (!lineEdit->isEnabled()) {
        states = cs_disabled;
    }
    d->addControlStateDisplay(name, states);
    if (!lineEdit->isEnabled()) {
        d->mainStateDisplay()->setState(cs_disabled);
    }
}
