//
// Created by jiang on 2021/5/7.
//

#ifndef HUMPTY_QSS_TEMPLATE_HPP
#define HUMPTY_QSS_TEMPLATE_HPP


#include "Container.hpp"

class XXXContainerPrivate;

class XXXContainer : public Container
{
Q_OBJECT
public:
    explicit XXXContainer(QWidget *parent = nullptr);
    ~XXXContainer() override;
    void setListenWidget(QWidget *w) override;

protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(XXXContainer)
    Q_DISABLE_COPY(XXXContainer)
};

//cpp

#include "XXXContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>


class XXXContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(XXXContainer)

    explicit XXXContainerPrivate(XXXContainer *p);
};

XXXContainerPrivate::XXXContainerPrivate(XXXContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(XXXContainer);
}

XXXContainer::XXXContainer(QWidget *parent) :
        Container(*new XXXContainerPrivate(this), parent)
{
    Q_D(XXXContainer);
}

XXXContainer::~XXXContainer()
{

}


#endif //HUMPTY_QSS_TEMPLATE_HPP
