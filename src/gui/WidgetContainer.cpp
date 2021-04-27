//
// Created by jiang on 2021/4/27.
//

#include "WidgetContainer.hpp"
#include "Container_p.hpp"


class WidgetContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(WidgetContainer)

    explicit WidgetContainerPrivate(WidgetContainer *p);
};

WidgetContainerPrivate::WidgetContainerPrivate(WidgetContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(WidgetContainer);
}

WidgetContainer::WidgetContainer(QWidget *parent) :
        Container(parent),
        d_ptr(new WidgetContainerPrivate(this))
{
    Q_D(WidgetContainer);
}

WidgetContainer::~WidgetContainer()
{

}

void WidgetContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(WidgetContainer);
    switch (event->type()) {
        case QEvent::Enter:
            d->mainStateDisplay()->setState(cs_hover, false);
            break;
        case QEvent::Leave:
            d->mainStateDisplay()->setState(cs_hover, true);
            break;
        default:
            break;
    }
}

void WidgetContainer::setListenWidget(QWidget *w)
{
    Q_D(WidgetContainer);
    Container::setListenWidget(w);
    d->addControlStateDisplay(w->metaObject()->className(), cs_hover);
}
