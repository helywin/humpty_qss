//
// Created by jiang on 2021/5/7.
//

#include "ScrollBarContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>
#include <QScrollBar>


class ScrollBarContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(ScrollBarContainer)
    QString mOrientation;

    explicit ScrollBarContainerPrivate(ScrollBarContainer *p);
};

ScrollBarContainerPrivate::ScrollBarContainerPrivate(ScrollBarContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(ScrollBarContainer);
}

ScrollBarContainer::ScrollBarContainer(QWidget *parent) :
        Container(*new ScrollBarContainerPrivate(this), parent)
{
    Q_D(ScrollBarContainer);
}

ScrollBarContainer::~ScrollBarContainer()
{

}

void ScrollBarContainer::setListenWidget(QWidget *w)
{
    Q_D(ScrollBarContainer);
    Container::setListenWidget(w);
    auto scrollBar = dynamic_cast<QScrollBar *>(w);
    if (!scrollBar) {
        std::abort();
    }
    d->mOrientation = scrollBar->orientation() == Qt::Horizontal ? "horizontal" : "vertical";
    QString name = w->metaObject()->className();
    w->dumpObjectTree();
    ControlStates states;
    states |= cs_pressed;
    states |= cs_hover;
    d->addControlStateDisplay(name + ":" + d->mOrientation, states);
    d->addControlStateDisplay(name + "::handle:" + d->mOrientation, states);
    d->addControlStateDisplay(name + "::add-line:" + d->mOrientation, states);
    d->addControlStateDisplay(name + "::sub-line:" + d->mOrientation, states);
}

void ScrollBarContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Container::onListenedWidgetEventOccurred(watched, event);
}
