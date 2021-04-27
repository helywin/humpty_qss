//
// Created by jiang on 2021/4/27.
//

#ifndef HUMPTY_QSS_WIDGETCONTAINER_HPP
#define HUMPTY_QSS_WIDGETCONTAINER_HPP

#include "Container.hpp"


class WidgetContainerPrivate;

class WidgetContainer : public Container
{
Q_OBJECT
public:
    explicit WidgetContainer(QWidget *parent = nullptr);
    ~WidgetContainer() override;
    void setListenWidget(QWidget *w) override;
protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;
private:
    Q_DECLARE_PRIVATE(WidgetContainer)
    Q_DISABLE_COPY(WidgetContainer)
    QScopedPointer<WidgetContainerPrivate> d_ptr;
};


#endif //HUMPTY_QSS_WIDGETCONTAINER_HPP
