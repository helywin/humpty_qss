//
// Created by jiang on 2021/5/2.
//

#ifndef HUMPTY_QSS_CHECKCONTAINER_HPP
#define HUMPTY_QSS_CHECKCONTAINER_HPP


#include "Container.hpp"

class CheckContainerPrivate;

class CheckContainer : public Container
{
Q_OBJECT
public:
    explicit CheckContainer(QWidget *parent = nullptr);
    ~CheckContainer() override;
    void setListenWidget(QWidget *w) override;

protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(CheckContainer)
    Q_DISABLE_COPY(CheckContainer)
};


#endif //HUMPTY_QSS_CHECKCONTAINER_HPP
