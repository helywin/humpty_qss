//
// Created by jiang on 2021/4/30.
//

#ifndef HUMPTY_QSS_COMBOBOXCONTAINER_HPP
#define HUMPTY_QSS_COMBOBOXCONTAINER_HPP


#include "Container.hpp"

class ComboBoxContainerPrivate;

class ComboBoxContainer : public Container
{
Q_OBJECT
public:
    explicit ComboBoxContainer(QWidget *parent = nullptr);
    ~ComboBoxContainer() override;
    void setListenWidget(QWidget *w) override;

protected:
//    void onGlobalMouseEvent(QEvent::Type type, Qt::MouseButton button) override;
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(ComboBoxContainer)
    Q_DISABLE_COPY(ComboBoxContainer)
};

#endif //HUMPTY_QSS_COMBOBOXCONTAINER_HPP
