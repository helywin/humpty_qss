//
// Created by helywin on 2021/4/26.
//

#ifndef HUMPTY_QSS_STATEDISPLAY_HPP
#define HUMPTY_QSS_STATEDISPLAY_HPP

#include <QWidget>
#include "GuiCom.hpp"

class StateDisplayPrivate;

class StateDisplay : public QWidget
{
Q_OBJECT
public:
    explicit StateDisplay(QWidget *parent = nullptr);
    ~StateDisplay() override;

public:
    void setAllStates(ControlStates states);
    void setState(ControlState s, bool on = true);

private:
    Q_DECLARE_PRIVATE(StateDisplay)
    Q_DISABLE_COPY(StateDisplay)
    QScopedPointer<StateDisplayPrivate> d_ptr;
};


#endif //HUMPTY_QSS_STATEDISPLAY_HPP
