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
    QString mName;
    QString mHandle;
    QString mAddLine;
    QString mSubLine;

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
    d->mName = name + ":" + d->mOrientation;
    d->mHandle = name + "::handle:" + d->mOrientation;
    d->mAddLine = name + "::add-line:" + d->mOrientation;
    d->mSubLine = name + "::sub-line:" + d->mOrientation;

    d->addControlStateDisplay(d->mName, states);
    d->addControlStateDisplay(d->mHandle, states);
    d->addControlStateDisplay(d->mAddLine, states);
    d->addControlStateDisplay(d->mSubLine, states);
    w->setMouseTracking(true);
    connect(scrollBar, &QScrollBar::sliderPressed, [this] {
        auto d = (ScrollBarContainerPrivate *) d_ptr.data();
        d_ptr->stateDisplay(d->mHandle)->setState(cs_pressed, true);
    });
    connect(scrollBar, &QScrollBar::sliderReleased, [this] {
        auto d = (ScrollBarContainerPrivate *) d_ptr.data();
        d_ptr->stateDisplay(d->mHandle)->setState(cs_pressed, false);
    });
}

void ScrollBarContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(ScrollBarContainer);
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
    opt.sliderPosition = scrollBar->sliderPosition();
    opt.sliderValue = scrollBar->value();
    opt.singleStep = scrollBar->singleStep();
    opt.pageStep = scrollBar->pageStep();
    opt.upsideDown = scrollBar->invertedAppearance();
    if (opt.orientation == Qt::Horizontal)
        opt.state |= QStyle::State_Horizontal;

    auto sliderRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider, scrollBar);
    auto subLineRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSubLine, scrollBar);

    // recalculate position of slider because there is a bug
    int maxlen = ((scrollBar->orientation() == Qt::Horizontal) ?
                  scrollBar->rect().width() : scrollBar->rect().height()) -
                 (subLineRect.width() * 2);
    auto sliderPos = QStyle::sliderPositionFromValue(scrollBar->minimum(),
                                                     scrollBar->maximum(),
                                                     scrollBar->sliderPosition(),
                                                     maxlen - sliderRect.width(),
                                                     scrollBar->invertedAppearance());
    sliderRect.moveLeft(sliderPos + sliderRect.width() / 2);

    auto addLineRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarAddLine, scrollBar);
    addLineRect.moveLeft(scrollBar->width() - addLineRect.width());
    switch (event->type()) {
        case QEvent::MouseMove: {
            d->stateDisplay(d->mHandle)
                    ->setState(cs_hover, sliderRect.contains(mouseEvent->pos()));
            d->stateDisplay(d->mSubLine)
                    ->setState(cs_hover, subLineRect.contains(mouseEvent->pos()));
            d->stateDisplay(d->mAddLine)
                    ->setState(cs_hover, addLineRect.contains(mouseEvent->pos()));
            break;
        }
        case QEvent::Leave:
            d->stateDisplay(d->mHandle)->setState(cs_hover, false);
            d->stateDisplay(d->mSubLine)->setState(cs_hover, false);
            d->stateDisplay(d->mAddLine)->setState(cs_hover, false);
            break;
        case QEvent::MouseButtonPress:
            if (mouseEvent->button() == Qt::LeftButton) {
                d->stateDisplay(d->mHandle)
                        ->setState(cs_pressed, sliderRect.contains(mouseEvent->pos()));
                d->stateDisplay(d->mSubLine)
                        ->setState(cs_pressed, subLineRect.contains(mouseEvent->pos()));
                d->stateDisplay(d->mAddLine)
                        ->setState(cs_pressed, addLineRect.contains(mouseEvent->pos()));
            }

            break;
        case QEvent::MouseButtonRelease:
            d->stateDisplay(d->mHandle)->setState(cs_pressed, false);
            d->stateDisplay(d->mSubLine)->setState(cs_pressed, false);
            d->stateDisplay(d->mAddLine)->setState(cs_pressed, false);
            break;
        default:
            break;
    }
}
