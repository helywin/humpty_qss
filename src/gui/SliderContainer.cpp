//
// Created by jiang on 2021/5/12.
//

#include "SliderContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>
#include <QSlider>
#include <QStyleOptionSlider>
#include <QMouseEvent>


class SliderContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(SliderContainer)
    QString mGroove;
    QString mHandle;
    QString mAddPage;
    QString mSubPage;

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
    Q_D(SliderContainer);
    Container::setListenWidget(w);
    auto slider = dynamic_cast<QSlider *>(w);
    if (!slider) {
        std::abort();
    }
    QString prefix = w->metaObject()->className();
    QString suffix = slider->orientation() == Qt::Horizontal ? ":horizontal" : ":vertical";
    d->mGroove = prefix + "::groove" + suffix;
    d->mHandle = prefix + "::handle" + suffix;
    d->mAddPage = prefix + "::add-page" + suffix;
    d->mSubPage = prefix + "::sub-page" + suffix;
    ControlStates states;
    states |= cs_hover;
    states |= cs_pressed;
    d->addControlStateDisplay(prefix + suffix, states, true);
    d->addControlStateDisplay(d->mGroove, states, false);
    d->addControlStateDisplay(d->mHandle, states, false);
    d->addControlStateDisplay(d->mAddPage, states, false);
    d->addControlStateDisplay(d->mSubPage, states, false);
    w->setMouseTracking(true);
}

void SliderContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(SliderContainer);
    QStyleOptionSlider opt;
    auto slider = dynamic_cast<QSlider *>(watched);
    if (!slider) {
        std::abort();
    }
    opt.initFrom(slider);
    opt.subControls = QStyle::SC_None;
    opt.activeSubControls = QStyle::SC_None;
    opt.orientation = slider->orientation();
    opt.maximum = slider->maximum();
    opt.minimum = slider->minimum();
    opt.tickPosition = (QSlider::TickPosition) slider->tickPosition();
    opt.tickInterval = slider->tickInterval();
    opt.upsideDown = (slider->orientation() == Qt::Horizontal) ?
                     (slider->invertedAppearance() != (opt.direction == Qt::RightToLeft))
                                                               : (!slider->invertedAppearance());
    opt.direction = Qt::LeftToRight; // we use the upsideDown option instead
    opt.sliderPosition = slider->sliderPosition();
    opt.sliderValue = slider->value();
    opt.singleStep = slider->singleStep();
    opt.pageStep = slider->pageStep();
    if (slider->orientation() == Qt::Horizontal)
        opt.state |= QStyle::State_Horizontal;

    auto sliderHandleRect = slider->style()->subControlRect(QStyle::CC_Slider, &opt,
                                                            QStyle::SC_SliderHandle, slider);
    auto sliderGrooveRect = slider->style()->subControlRect(QStyle::CC_Slider, &opt,
                                                            QStyle::SC_SliderGroove, slider);
    auto sliderSubPage = sliderGrooveRect;
    auto sliderAddPage = sliderGrooveRect;
    sliderSubPage.setRight(sliderHandleRect.left() - 1);
    sliderAddPage.setLeft(sliderHandleRect.right() + 1);
    auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            bool onHandle = sliderHandleRect.contains(mouseEvent->pos());
            d->mainStateDisplay()->setState(cs_pressed, true);
            d->stateDisplay(d->mHandle)->setState(cs_pressed, onHandle);
            d->stateDisplay(d->mSubPage)->setState(cs_pressed,
                                                   sliderSubPage.contains(mouseEvent->pos()));
            d->stateDisplay(d->mAddPage)->setState(cs_pressed,
                                                   sliderAddPage.contains(mouseEvent->pos()));
            if (onHandle) {
                d->stateDisplay(d->mGroove)->setState(cs_hover, false);
            } else {
                d->stateDisplay(d->mGroove)->setState(cs_pressed,
                                                      sliderGrooveRect.contains(mouseEvent->pos()));
            }
            break;
        }
        case QEvent::MouseButtonRelease:
            d->mainStateDisplay()->setState(cs_pressed, false);
            d->stateDisplay(d->mHandle)->setState(cs_pressed, false);
            d->stateDisplay(d->mGroove)->setState(cs_pressed, false);
            d->stateDisplay(d->mSubPage)->setState(cs_pressed, false);
            d->stateDisplay(d->mAddPage)->setState(cs_pressed, false);
            break;
        case QEvent::HoverEnter:
            d->mainStateDisplay()->setState(cs_hover, true);
            break;
        case QEvent::MouseMove: {
            if (!slider->rect().contains(mouseEvent->pos())) {
                break;
            }
            bool onHandle = sliderHandleRect.contains(mouseEvent->pos());
            d->stateDisplay(d->mHandle)->setState(cs_hover, onHandle);
            d->stateDisplay(d->mSubPage)->setState(cs_hover,
                                                   sliderSubPage.contains(mouseEvent->pos()));
            d->stateDisplay(d->mAddPage)->setState(cs_hover,
                                                   sliderAddPage.contains(mouseEvent->pos()));
            if (onHandle) {
                d->stateDisplay(d->mGroove)->setState(cs_hover, false);
            } else {
                d->stateDisplay(d->mGroove)->setState(cs_hover,
                                                      sliderGrooveRect.contains(mouseEvent->pos()));
            }
            break;
        }
        case QEvent::HoverLeave:
            d->mainStateDisplay()->setState(cs_hover, false);
            d->stateDisplay(d->mHandle)->setState(cs_hover, false);
            d->stateDisplay(d->mGroove)->setState(cs_hover, false);
            d->stateDisplay(d->mSubPage)->setState(cs_hover, false);
            d->stateDisplay(d->mAddPage)->setState(cs_hover, false);
            break;
    }
}
