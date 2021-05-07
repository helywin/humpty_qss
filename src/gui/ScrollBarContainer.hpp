//
// Created by jiang on 2021/5/7.
//

#ifndef HUMPTY_QSS_SCROLLBARCONTAINER_HPP
#define HUMPTY_QSS_SCROLLBARCONTAINER_HPP


#include "Container.hpp"

class ScrollBarContainerPrivate;

class ScrollBarContainer : public Container
{
Q_OBJECT
public:
    explicit ScrollBarContainer(QWidget *parent = nullptr);
    ~ScrollBarContainer() override;
    void setListenWidget(QWidget *w) override;

protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(ScrollBarContainer)
    Q_DISABLE_COPY(ScrollBarContainer)
};


#endif //HUMPTY_QSS_SCROLLBARCONTAINER_HPP
