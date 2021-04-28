//
// Created by helywin on 2021/4/26.
//

#ifndef HUMPTY_QSS_CONTAINER_HPP
#define HUMPTY_QSS_CONTAINER_HPP


#include <QFrame>
#include <QEvent>
#include "GuiCom.hpp"

class QStandardItemModel;
class ContainerPrivate;
class StateDisplay;

class Container : public QFrame
{
Q_OBJECT
public:
    enum WidgetPosition
    {
        wp_north = 1,
        wp_south = 2,
    };
public:
    explicit Container(QWidget *parent = nullptr);
    ~Container() override;

#ifdef _WINDOWS
    void setListenGlobalMouseEvent(bool enable);
#endif
    QStandardItemModel *objectTree();
    bool eventFilter(QObject *watched, QEvent *event) final;

protected:
    explicit Container(ContainerPrivate &d, QWidget *parent = nullptr);
    virtual void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event);
    virtual void onGlobalMouseEvent(QEvent::Type type, Qt::MouseButton button);
    void setListenWidget(QWidget *w);
    void setWidget(QWidget *w, WidgetPosition wp = wp_north);
    void addControlStateDisplay(const QString &name, ControlStates states);
    StateDisplay *stateDisplay(const QString &name);


private:
    Q_DECLARE_PRIVATE(Container)
    Q_DISABLE_COPY(Container)
    QSharedPointer<ContainerPrivate> d_ptr;
};


#endif //HUMPTY_QSS_CONTAINER_HPP
