//
// Created by jiang on 2021/5/2.
//

#ifndef HUMPTY_QSS_MENUCONTAINER_HPP
#define HUMPTY_QSS_MENUCONTAINER_HPP

#include "Container.hpp"

class MenuContainerPrivate;

class MenuContainer : public Container
{
Q_OBJECT
public:
    explicit MenuContainer(QWidget *parent = nullptr);
    ~MenuContainer() override;
    void setListenWidget(QWidget *w) override;
protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(MenuContainer)
    Q_DISABLE_COPY(MenuContainer)
};

#endif //HUMPTY_QSS_MENUCONTAINER_HPP
