//
// Created by jiang on 2021/4/30.
//

#ifndef HUMPTY_QSS_LINEEDITCONTAINER_HPP
#define HUMPTY_QSS_LINEEDITCONTAINER_HPP


#include "Container.hpp"

class LineEditContainerPrivate;

class LineEditContainer : public Container
{
Q_OBJECT
public:
    explicit LineEditContainer(QWidget *parent = nullptr);
    ~LineEditContainer() override;
    void setListenWidget(QWidget *w) override;

protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(LineEditContainer)
    Q_DISABLE_COPY(LineEditContainer)
};


#endif //HUMPTY_QSS_LINEEDITCONTAINER_HPP
