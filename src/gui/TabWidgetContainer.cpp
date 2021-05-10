//
// Created by jiang on 2021/5/7.
//

#include "TabWidgetContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>


class TabWidgetContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(TabWidgetContainer)

    explicit TabWidgetContainerPrivate(TabWidgetContainer *p);
};

TabWidgetContainerPrivate::TabWidgetContainerPrivate(TabWidgetContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(TabWidgetContainer);
}

TabWidgetContainer::TabWidgetContainer(QWidget *parent) :
        Container(*new TabWidgetContainerPrivate(this), parent)
{
    Q_D(TabWidgetContainer);
}

TabWidgetContainer::~TabWidgetContainer()
{

}

void TabWidgetContainer::setListenWidget(QWidget *w)
{
    Q_D(TabWidgetContainer);
    Container::setListenWidget(w);
    QString name = w->metaObject()->className();
    d->addControlStateDisplay(name, cs_none, true);
    d->addControlStateDisplay(name + "::pane", cs_none, false);
    d->addControlStateDisplay(name + "::tab-bar", cs_none, false);
}
