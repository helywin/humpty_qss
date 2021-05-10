//
// Created by jiang on 2021/5/2.
//

#include "TabBarContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>
#include <QHoverEvent>
#include <QTabBar>
#include <QAbstractButton>
#include <QMouseEvent>
#include <QToolButton>
#include <cassert>


class TabBarContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(TabBarContainer)
    QStringList mTabTypes;
    QMap<QString, int> mTabInfos;
    QString mCloseButton;
    QString mTear;
    QString mScroller;
    QString mLeftArrow;
    QString mRightArrow;
    QList<QToolButton *> mToolButtons;

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
    mTear = "QTabBar::tear";
    mScroller = "QTabBar::scroller";
    mLeftArrow = "QTabBar QToolButton::left-arrow";
    mRightArrow = "QTabBar QToolButton::right-arrow";
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
#ifdef __linux
    tabBar->setMouseTracking(true);
#endif
    QString className = w->metaObject()->className();
    d->mCloseButton = className + "::close-button";
    d->addControlStateDisplay(className, cs_none, true);
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
            d->addControlStateDisplay(d->mCloseButton, states, true);
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

    d->addControlStateDisplay(d->mTear, cs_none, false);
    d->addControlStateDisplay(d->mScroller, cs_none, false);
    states = cs_pressed;
    states |= cs_hover;
    states |= cs_disabled;
    d->addControlStateDisplay(d->mLeftArrow, states, false);
    d->addControlStateDisplay(d->mRightArrow, states, false);

    for (auto child : tabBar->children()) {
        auto b = dynamic_cast<QToolButton *>(child);
        if (b) {
            d->mToolButtons.append(b);
        }
    }
    assert(d->mToolButtons.size() == 2);
    //toolButtons[0] is left-arrow
    //toolButtons[1] is right-arrow
    d->mToolButtons[0]->setObjectName(d->mLeftArrow);
    d->mToolButtons[1]->setObjectName(d->mRightArrow);
    d->mToolButtons[0]->installEventFilter(this);
    d->mToolButtons[1]->installEventFilter(this);
    connect(d->mToolButtons[0], &QToolButton::pressed, [this] {
        auto d = (TabBarContainerPrivate *) d_ptr.data();
        d->stateDisplay(d->mLeftArrow)->setState(cs_pressed, true);
    });
    connect(d->mToolButtons[0], &QToolButton::released, [this] {
        auto d = (TabBarContainerPrivate *) d_ptr.data();
        d->stateDisplay(d->mLeftArrow)->setState(cs_pressed, false);
    });
    connect(d->mToolButtons[1], &QToolButton::pressed, [this] {
        auto d = (TabBarContainerPrivate *) d_ptr.data();
        d->stateDisplay(d->mRightArrow)->setState(cs_pressed, true);
    });
    connect(d->mToolButtons[1], &QToolButton::released, [this] {
        auto d = (TabBarContainerPrivate *) d_ptr.data();
        d->stateDisplay(d->mRightArrow)->setState(cs_pressed, false);
    });

    auto updateToolButtonEnabled = [this] {
        auto d = (TabBarContainerPrivate *) d_ptr.data();
        for (auto btn : d->mToolButtons) {
            d->stateDisplay(btn->objectName())->setState(cs_disabled, !btn->isEnabled());
            if (!btn->isEnabled()) {
                d->stateDisplay(btn->objectName())->setState(cs_hover, false);
                d->stateDisplay(btn->objectName())->setState(cs_pressed, false);
            }
        }
    };

    connect(d->mToolButtons[0], &QToolButton::clicked, updateToolButtonEnabled);
    connect(d->mToolButtons[1], &QToolButton::clicked, updateToolButtonEnabled);

    d->stateDisplay(d->mLeftArrow)->setState(cs_disabled, !d->mToolButtons[0]->isEnabled());
    d->stateDisplay(d->mRightArrow)->setState(cs_disabled, !d->mToolButtons[1]->isEnabled());
}

void TabBarContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(TabBarContainer);
    auto tabBar = dynamic_cast<QTabBar *>(d->mListenWidget);
#ifdef _WINDOWS
    auto hoverEvent = dynamic_cast<QHoverEvent *>(event);
    if (hoverEvent && tabBar) {
        qDebug() << "linux hover";
        for (const auto &tab : d->mTabInfos.keys()) {
            d->stateDisplay(tab)->setState(cs_hover, false);
            if (tabBar->tabRect(d->mTabInfos[tab]).contains(hoverEvent->pos())) {
                d->stateDisplay(tab)->setState(cs_hover, true);
            }
        }
        return;
    }
#elif __linux
    auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
    if (mouseEvent && event->type() == QEvent::MouseMove) {
        for (const auto &tab : d->mTabInfos.keys()) {
            d->stateDisplay(tab)->setState(cs_hover, false);
            if (tabBar->tabRect(d->mTabInfos[tab]).contains(mouseEvent->pos())) {
                d->stateDisplay(tab)->setState(cs_hover, true);
            }
        }
    }
#endif
    if (watched->metaObject()->className() == QString("CloseButton")) {
        if (event->type() == QEvent::Enter) {
            d->stateDisplay(d->mCloseButton)->setState(cs_hover, true);
        } else if (event->type() == QEvent::Leave) {
            d->stateDisplay(d->mCloseButton)->setState(cs_hover, false);
        }
    }
    if (watched == d->mListenWidget && event->type() == QEvent::Leave) {
        for (const auto &tab : d->mTabInfos.keys()) {
            d->stateDisplay(tab)->setState(cs_hover, false);
        }
    }
    if (watched->metaObject()->className() == QString("QToolButton")) {
        switch (event->type()) {
            case QEvent::Enter:
                if (watched->isEnabled()) {
                    d->stateDisplay(watched->objectName())->setState(cs_hover, true);
                }
                break;
            case QEvent::Leave:
                d->stateDisplay(watched->objectName())->setState(cs_hover, false);
                break;
            default:
                break;
        }
    }
}
