//
// Created by helywin on 2021/4/26.
//

#include "StateDisplay.hpp"
#include <QMap>
#include <QLabel>
#include <QVariant>
#include <QBoxLayout>
#include <QStyle>
#include "Utils.hpp"

using namespace Com;

class StateDisplayPrivate
{
public:
    Q_DECLARE_PUBLIC(StateDisplay)
    StateDisplay *q_ptr;
    QHBoxLayout *mLayout;
    bool mIsAllStateInit = false;
    ControlStates mAllStates;
    ControlStates mCurrentStates;
    QMap<int, QLabel *> mLabels;

    explicit StateDisplayPrivate(StateDisplay *p);
};

StateDisplayPrivate::StateDisplayPrivate(StateDisplay *p) :
        q_ptr(p)
{
    Q_Q(StateDisplay);
    initWidget(mLayout, q);
}

StateDisplay::StateDisplay(QWidget *parent) :
        QWidget(parent),
        d_ptr(new StateDisplayPrivate(this))
{

}

StateDisplay::~StateDisplay()
{

}

void StateDisplay::setState(ControlState s, bool on)
{
    Q_D(StateDisplay);
    assert(d->mAllStates.testFlag(s));
    if (on) {
        d->mCurrentStates |= s;
    } else {
        d->mCurrentStates ^= s;
    }
    d->mLabels[s]->setProperty("active", on);
    d->mLabels[s]->style()->polish(d->mLabels[s]);
}

void StateDisplay::setAllStates(ControlStates states)
{
    Q_D(StateDisplay);
    assert(!d->mIsAllStateInit);
    d->mIsAllStateInit = true;
    d->mAllStates = states;
    for (int i = 0; i < cs_stateCount; ++i) {
        auto state = static_cast<ControlState>(i);
        if (d->mAllStates.testFlag(state)) {
            d->mLabels[i] = new QLabel(controlStateToString(state), this);
            d->mLabels[i]->setProperty("active", false);
            d->mLabels[i]->setObjectName("StateLabel");
            d->mLayout->addWidget(d->mLabels[i]);
        }
    }
    d->mLayout->addStretch(1);
}

