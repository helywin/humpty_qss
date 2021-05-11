//
// Created by jiang on 2021/5/11.
//

#ifndef HUMPTY_QSS_PUSHBUTTONCONTAINER_HPP
#define HUMPTY_QSS_PUSHBUTTONCONTAINER_HPP


#include "Container.hpp"

class PushButtonContainerPrivate;

class PushButtonContainer : public Container
{
Q_OBJECT
public:
    explicit PushButtonContainer(QWidget *parent = nullptr);
    ~PushButtonContainer() override;
    void setListenWidget(QWidget *w) override;
protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

protected:
    Q_DECLARE_PRIVATE(PushButtonContainer)
    Q_DISABLE_COPY(PushButtonContainer)
};


#endif //HUMPTY_QSS_PUSHBUTTONCONTAINER_HPP
