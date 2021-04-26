//
// Created by helywin on 2021/4/26.
//

#ifndef HUMPTY_QSS_CONTAINER_HPP
#define HUMPTY_QSS_CONTAINER_HPP


#include <QFrame>
#include <QEvent>

class QStandardItemModel;
class ContainerPrivate;

class Container : public QFrame
{
Q_OBJECT
public:
    explicit Container(QWidget *parent = nullptr);
    ~Container() override;
    void setListenGlobalMouseEvent(bool enable);
    QStandardItemModel *objectTree();
    bool eventFilter(QObject *watched, QEvent *event) final;

protected:
    virtual void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event);
    virtual void onGlobalMouseEvent(QEvent::Type type, Qt::MouseButton button);
    void setListenWidget(QWidget *w);


private:
    Q_DECLARE_PRIVATE(Container)
    Q_DISABLE_COPY(Container)
    QScopedPointer<ContainerPrivate> d_ptr;
};


#endif //HUMPTY_QSS_CONTAINER_HPP
