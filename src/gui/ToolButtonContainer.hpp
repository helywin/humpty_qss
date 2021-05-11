//
// Created by jiang on 2021/4/30.
//

#ifndef HUMPTY_QSS_TOOLBUTTONCONTAINER_HPP
#define HUMPTY_QSS_TOOLBUTTONCONTAINER_HPP

#include "Container.hpp"

class ToolButtonContainerPrivate;

class ToolButtonContainer : public Container
{
    Q_OBJECT
public:
    explicit ToolButtonContainer(QWidget *parent = nullptr);
    ~ToolButtonContainer() override;
    void setListenWidget(QWidget *w) override;
protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

protected:
    Q_DECLARE_PRIVATE(ToolButtonContainer)
    Q_DISABLE_COPY(ToolButtonContainer)
};


#endif //HUMPTY_QSS_TOOLBUTTONCONTAINER_HPP
