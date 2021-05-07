//
// Created by jiang on 2021/5/7.
//

#include "ScrollBarContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>
#include <QScrollBar>
#include <QStyleOptionSlider>
#include <QMouseEvent>


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
    w->setMouseTracking(true);
}

void ScrollBarContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Container::onListenedWidgetEventOccurred(watched, event);
    auto scrollBar = dynamic_cast<QScrollBar *>(watched);
    if (!scrollBar) {
        std::abort();
    }
    QStyleOptionSlider opt;
    auto mouseEvent = (QMouseEvent *) event;
    opt.initFrom(this);
    opt.subControls = QStyle::SC_None;
    opt.activeSubControls = QStyle::SC_None;
    opt.orientation = scrollBar->orientation();
    opt.minimum = scrollBar->minimum();
    opt.maximum = scrollBar->maximum();
    opt.sliderPosition = opt.orientation == Qt::Horizontal ?
                         scrollBar->pos().x() :
                         scrollBar->pos().y();
    opt.sliderValue = scrollBar->value();
    opt.singleStep = scrollBar->singleStep();
    opt.pageStep = scrollBar->pageStep();
    opt.upsideDown = scrollBar->invertedAppearance();
    if (opt.orientation == Qt::Horizontal)
        opt.state |= QStyle::State_Horizontal;
//    if ((d->flashed || !d->transient) &&
//        style()->styleHint(QStyle::SH_ScrollBar_Transient, 0, this))
//        option->state |= QStyle::State_On;
    auto sliderRect = scrollBar->style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                                         QStyle::SC_ScrollBarSlider, scrollBar);
//    qDebug() << sliderRect.width();
    switch (event->type()) {
        case QEvent::MouseMove: {
            if (sliderRect.contains(pos())) {
                qDebug() << "sliderRect;";
            }
            auto subControl = scrollBar->style()->hitTestComplexControl(QStyle::CC_ScrollBar,
                                                                        &opt,
                                                                        mouseEvent->pos(),
                                                                        scrollBar);
//            qDebug() << subControl;
        }
        default:
            break;
    }
}
