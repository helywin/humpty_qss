//
// Created by jiang on 2021/5/7.
//

#ifndef HUMPTY_QSS_TABWIDGETCONTAINER_HPP
#define HUMPTY_QSS_TABWIDGETCONTAINER_HPP


#include "Container.hpp"

class TabWidgetContainerPrivate;

class TabWidgetContainer : public Container
{
Q_OBJECT
public:
    explicit TabWidgetContainer(QWidget *parent = nullptr);
    ~TabWidgetContainer() override;
    void setListenWidget(QWidget *w) override;

private:
    Q_DECLARE_PRIVATE(TabWidgetContainer)
    Q_DISABLE_COPY(TabWidgetContainer)
};

#endif //HUMPTY_QSS_TABWIDGETCONTAINER_HPP
