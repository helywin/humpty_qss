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

}

WidgetContainer::WidgetContainer(QWidget *parent) :
        Container(parent),
        d_ptr(new WidgetContainerPrivate(this))
{

}

WidgetContainer::~WidgetContainer()
{

}