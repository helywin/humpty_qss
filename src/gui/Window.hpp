//
// Created by helywin on 2021/4/14.
//

#ifndef SOLAR_WINDOW_HPP
#define SOLAR_WINDOW_HPP

#include <QWidget>
#include "Translatable.hpp"

class WindowPrivate;

class Window : public QWidget, public Com::Translatable
{
Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() override;

protected:
    void translate() override;

private:
    Q_DECLARE_PRIVATE(Window)
    Q_DISABLE_COPY(Window)
    QScopedPointer<WindowPrivate> d_ptr;
};

#endif   //SOLAR_WINDOW_HPP
