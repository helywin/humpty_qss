//
// Created by jiang on 2021/4/30.
//

#ifndef HUMPTY_QSS_BUTTONCONTAINER_HPP
#define HUMPTY_QSS_BUTTONCONTAINER_HPP

#include "Container.hpp"

class ButtonContainerPrivate;

class ButtonContainer : public Container
{
    Q_OBJECT
public:
    explicit ButtonContainer(QWidget *parent = nullptr);
    ~ButtonContainer() override;
    void setListenWidget(QWidget *w) override;
protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

protected:
    Q_DECLARE_PRIVATE(ButtonContainer)
    Q_DISABLE_COPY(ButtonContainer)
};


#endif //HUMPTY_QSS_BUTTONCONTAINER_HPP
