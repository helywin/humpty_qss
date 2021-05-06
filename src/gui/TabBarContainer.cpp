//
// Created by jiang on 2021/5/2.
//

#include "TabBarContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>
#include <QHoverEvent>
#include <QTabBar>
#include <QAbstractButton>


class TabBarContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(TabBarContainer)
    QStringList mTabTypes;
    QMap<QString, int> mTabInfos;
    QString mCloseButton;

    explicit TabBarContainerPrivate(TabBarContainer *p);
};

TabBarContainerPrivate::TabBarContainerPrivate(TabBarContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(TabBarContainer);
    mTabTypes << "QTabBar::tab:first"
              << "QTabBar::tab"
              << "QTabBar::tab:last"
              << "QTabBar::tab:only-one";
}

TabBarContainer::TabBarContainer(QWidget *parent) :
        Container(*new TabBarContainerPrivate(this), parent)
{
    Q_D(TabBarContainer);
}

TabBarContainer::~TabBarContainer()
{

}

void TabBarContainer::setListenWidget(QWidget *w)
{
    Q_D(TabBarContainer);
    Container::setListenWidget(w);
    auto tabBar = dynamic_cast<QTabBar *>(w);
    if (!tabBar) {
        std::abort();
    }
    QString className = w->metaObject()->className();
    d->mCloseButton = className + "::close-button";
    d->addControlStateDisplay(className, cs_none);
    ControlStates states;
    states |= cs_hover;
    states |= cs_selected;

    for (const auto &tab: d->mTabTypes) {
        int i;
        for (i = 0; i < tabBar->count(); ++i) {
            if (tabBar->tabText(i) == tab) {
                break;
            }
        }
        if (i != tabBar->count()) {
            d->mTabInfos[tab] = i;
        }
    }

    for (const auto &tab: d->mTabInfos.keys()) {
        d->addControlStateDisplay(tab, states, false);
        // detect default selected tab
        if (tabBar->tabText(tabBar->currentIndex()) == tab) {
            d->stateDisplay(tab)->setState(cs_selected, true);
        }
        connect(tabBar, &QTabBar::currentChanged, [tabBar, this, tab](int index) {
            auto d = (TabBarContainerPrivate *) d_ptr.data();
            d->stateDisplay(tab)->setState(cs_selected, false);
            if (tabBar->tabText(index) == tab) {
                d->stateDisplay(tab)->setState(cs_selected, true);
            }
        });
    }


    for (int i = 0; i < tabBar->count(); ++i) {
        auto *button = dynamic_cast<QAbstractButton *>(tabBar->tabButton(i, QTabBar::RightSide));
        if (tabBar->tabText(i) == d->mCloseButton && button) {
            states = cs_hover;
            states |= cs_pressed;
            d->addControlStateDisplay(d->mCloseButton, states);
            connect(button, &QAbstractButton::pressed, [this] {
                auto d = (TabBarContainerPrivate *) d_ptr.data();
                d->stateDisplay(d->mCloseButton)->setState(cs_pressed, true);
            });

            connect(button, &QAbstractButton::released, [this] {
                auto d = (TabBarContainerPrivate *) d_ptr.data();
                d->stateDisplay(d->mCloseButton)->setState(cs_pressed, false);
            });
            button->installEventFilter(this);
        }
    }

}

void TabBarContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(TabBarContainer);
    auto hoverEvent = dynamic_cast<QHoverEvent *>(event);
    auto tabBar = dynamic_cast<QTabBar *>(watched);
    if (hoverEvent && tabBar) {
        for (const auto &tab : d->mTabInfos.keys()) {
            d->stateDisplay(tab)->setState(cs_hover, false);
            if (tabBar->tabRect(d->mTabInfos[tab]).contains(hoverEvent->pos())) {
                d->stateDisplay(tab)->setState(cs_hover, true);
            }
        }
        return;
    }
    if (watched->metaObject()->className() == QString("CloseButton")) {
        if (event->type() == QEvent::Enter) {
            d->stateDisplay(d->mCloseButton)->setState(cs_hover, true);
        } else if (event->type() == QEvent::Leave) {
            d->stateDisplay(d->mCloseButton)->setState(cs_hover, false);
        }
    }
}
