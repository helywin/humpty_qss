//
// Created by jiang on 2021/5/12.
//

#include "SliderContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>


class SliderContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(SliderContainer)

    explicit SliderContainerPrivate(SliderContainer *p);
};

SliderContainerPrivate::SliderContainerPrivate(SliderContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(SliderContainer);
}

SliderContainer::SliderContainer(QWidget *parent) :
        Container(*new SliderContainerPrivate(this), parent)
{
    Q_D(SliderContainer);
}

SliderContainer::~SliderContainer()
{

}

void SliderContainer::setListenWidget(QWidget *w)
{
    Container::setListenWidget(w);
}

void SliderContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Container::onListenedWidgetEventOccurred(watched, event);
}
