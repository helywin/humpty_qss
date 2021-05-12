//
// Created by jiang on 2021/5/12.
//

#include "SliderContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>
#include <QSlider>


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
    QString suffix = slider->orientation() == Qt::Horizontal ? "horizontal" : "vertical";
    d->mGroove = prefix + "::groove" + suffix;
    d->mHandle = prefix + "::handle" + suffix;
    d->mAddPage = prefix + "::add-page" + suffix;
    d->mSubPage = prefix + "::sub-page" + suffix;
    ControlStates states;
    states |= cs_hover;
    states |= cs_pressed;
    d->addControlStateDisplay(prefix, states, true);
    d->addControlStateDisplay(d->mGroove, states, false);
    d->addControlStateDisplay(d->mHandle, states, false);
    d->addControlStateDisplay(d->mAddPage, states, false);
    d->addControlStateDisplay(d->mSubPage, states, false);


}

void SliderContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    switch (event->type()) {
        case QEvent::MouseButtonPress:

            break;
    }
}
