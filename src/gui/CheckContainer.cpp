//
// Created by jiang on 2021/5/2.
//

#include "CheckContainer.hpp"
#include "Container_p.hpp"
#include <QCheckBox>
#include <QRadioButton>
#include "Functions.hpp"

class CheckContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(CheckContainer)
    QString mIndicatorName;

    explicit CheckContainerPrivate(CheckContainer *p);
};

CheckContainerPrivate::CheckContainerPrivate(CheckContainer *p) :
        ContainerPrivate(p)
{

}

CheckContainer::CheckContainer(QWidget *parent) :
        Container(*new CheckContainerPrivate(this), parent)
{

}

CheckContainer::~CheckContainer()
{

}

void CheckContainer::setListenWidget(QWidget *w)
{
    Q_D(CheckContainer);
    Container::setListenWidget(w);
    QString name = w->metaObject()->className();
    ControlStates states;
    states |= cs_focus;
    states |= cs_pressed;
    states |= cs_focus;
    states |= cs_checked;
    states |= cs_hover;
    auto checkBox = dynamic_cast<QCheckBox *>(w);
    auto radioButton = dynamic_cast<QRadioButton *>(w);
    if (checkBox && checkBox->isTristate()) {
        states |= cs_indeterminate;
    }
    if (!w->isEnabled()) {
        states = cs_disabled;
    }
    d->addControlStateDisplay(name, states);
    d->mIndicatorName = name + "::indicator";
    auto indicator = d->mIndicatorName;
    d->addControlStateDisplay(d->mIndicatorName, states, false);
    if (!w->isEnabled()) {
        return;
    }
    connect(dynamic_cast<QAbstractButton *>(w), &QAbstractButton::pressed, [this, indicator] {
        d_ptr->mainStateDisplay()->setState(cs_pressed, true);
        d_ptr->stateDisplay(indicator)->setState(cs_pressed, true);
    });
    connect(dynamic_cast<QAbstractButton *>(w), &QAbstractButton::released, [this, indicator] {
        d_ptr->mainStateDisplay()->setState(cs_pressed, false);
        d_ptr->stateDisplay(indicator)->setState(cs_pressed, false);
    });

    if (checkBox) {
        connect(checkBox, &QCheckBox::stateChanged, [checkBox, this, indicator](int state) {
            bool tristate = checkBox->isTristate();
            if (state == Qt::Checked) {
                d_ptr->mainStateDisplay()->setState(cs_checked, true);
                d_ptr->stateDisplay(indicator)->setState(cs_checked, true);
                if (tristate) {
                    d_ptr->mainStateDisplay()->setState(cs_indeterminate, false);
                    d_ptr->stateDisplay(indicator)->setState(cs_indeterminate, false);
                }
            } else if (state == Qt::PartiallyChecked) {
                d_ptr->mainStateDisplay()->setState(cs_checked, false);
                d_ptr->stateDisplay(indicator)->setState(cs_checked, false);
                d_ptr->mainStateDisplay()->setState(cs_indeterminate, true);
                d_ptr->stateDisplay(indicator)->setState(cs_indeterminate, true);
            } else {
                d_ptr->mainStateDisplay()->setState(cs_checked, false);
                d_ptr->stateDisplay(indicator)->setState(cs_checked, false);
                if (tristate) {
                    d_ptr->mainStateDisplay()->setState(cs_indeterminate, false);
                    d_ptr->stateDisplay(indicator)->setState(cs_indeterminate, false);
                }
            }
        });
    } else if (radioButton) {
        connect(radioButton, &QRadioButton::clicked, [this, indicator](bool checked) {
            d_ptr->mainStateDisplay()->setState(cs_checked, checked);
            d_ptr->stateDisplay(indicator)->setState(cs_checked, checked);
        });
    } else {
        std::abort();
    }
}

void CheckContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(CheckContainer);
    switch (event->type()) {
        case QEvent::Enter:
            d->mainStateDisplay()->setState(cs_hover, true);
            d->stateDisplay(d->mIndicatorName)->setState(cs_hover, true);
            break;
        case QEvent::Leave:
            d->mainStateDisplay()->setState(cs_hover, false);
            d->stateDisplay(d->mIndicatorName)->setState(cs_hover, false);
            break;
        case QEvent::FocusIn:
            d->mainStateDisplay()->setState(cs_focus, true);
            d->stateDisplay(d->mIndicatorName)->setState(cs_focus, true);
            break;
        case QEvent::FocusOut:
            d->mainStateDisplay()->setState(cs_focus, false);
            d->stateDisplay(d->mIndicatorName)->setState(cs_focus, false);
            break;
        default:
            break;
    }
}
