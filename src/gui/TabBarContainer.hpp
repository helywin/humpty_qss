//
// Created by jiang on 2021/5/2.
//

#ifndef HUMPTY_QSS_TABBARCONTAINER_HPP
#define HUMPTY_QSS_TABBARCONTAINER_HPP


#include "Container.hpp"

class TabBarContainerPrivate;

class TabBarContainer : public Container
{
Q_OBJECT
public:
    explicit TabBarContainer(QWidget *parent = nullptr);
    ~TabBarContainer() override;
    void setListenWidget(QWidget *w) override;

protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(TabBarContainer)
    Q_DISABLE_COPY(TabBarContainer)
};


#endif //HUMPTY_QSS_TABBARCONTAINER_HPP
