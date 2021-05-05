//
// Created by jiang on 2021/5/2.
//

#include "TabBarContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>


class TabBarContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(TabBarContainer)

    explicit TabBarContainerPrivate(TabBarContainer *p);
};

TabBarContainerPrivate::TabBarContainerPrivate(TabBarContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(TabBarContainer);
}

TabBarContainer::TabBarContainer(QWidget *parent) :
        Container(*new TabBarContainerPrivate(this), parent)
{
    Q_D(TabBarContainer);
}

TabBarContainer::~TabBarContainer()
{

}

void TabBarContainer::setListenWidget(QWidget *w)
{
    Container::setListenWidget(w);
}

void TabBarContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Container::onListenedWidgetEventOccurred(watched, event);
}
