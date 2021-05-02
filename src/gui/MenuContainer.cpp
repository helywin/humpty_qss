//
// Created by jiang on 2021/5/2.
//

#include "MenuContainer.hpp"
#include "Container_p.hpp"
#include <QMenu>
#include <QDebug>

class MenuContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(MenuContainer)

    explicit MenuContainerPrivate(MenuContainer *p);
};

MenuContainerPrivate::MenuContainerPrivate(MenuContainer *p) :
        ContainerPrivate(p)
{

}

MenuContainer::MenuContainer(QWidget *parent) :
        Container(*new MenuContainerPrivate(this), parent)
{

}

MenuContainer::~MenuContainer()
{

}

void MenuContainer::setListenWidget(QWidget *w)
{
    Q_D(MenuContainer);
    Container::setListenWidget(w);
    auto menu = dynamic_cast<QMenu *>(w);
    ControlStates states;
    states |= cs_selected;
    d->addControlStateDisplay(w->metaObject()->className(), cs_none);
    for (auto action : menu->actions()) {
        qDebug() << action->text() << "  ===";
        if (action->data().toBool()) {
            qDebug() << action->text() << "  on";
            d->addControlStateDisplay(action->text(),
                                      states | (action->isCheckable() ? cs_checked : cs_none),
                                      false);
        }
        QObject::connect(action, &QAction::hovered, [action, this] {
            for (const auto &cName : d_ptr->mStateDisplayList.keys()) {
                d_ptr->stateDisplay(cName)->setState(cs_selected, false);
            }
            if (action->data().toBool()) {
                d_ptr->stateDisplay(action->text())->setState(cs_selected, true);
            }
        });
        // listen specified QAction check state
        if (action->data().toBool() && action->isCheckable()) {
            QObject::connect(action, &QAction::triggered, [action, this](bool checked) {
                d_ptr->stateDisplay(action->text())->setState(cs_checked, checked);
            });
        }
    }
}

void MenuContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(MenuContainer);
    switch (event->type()) {
        case QEvent::Leave:
            for (const auto &cName : d->mStateDisplayList.keys()) {
                d->stateDisplay(cName)->setState(cs_selected, false);
            }
            break;
        default:
            break;
    }
}

