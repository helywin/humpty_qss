//
// Created by jiang on 2021/5/12.
//

#ifndef HUMPTY_QSS_SLIDERCONTAINER_HPP
#define HUMPTY_QSS_SLIDERCONTAINER_HPP


#include "Container.hpp"

class SliderContainerPrivate;

class SliderContainer : public Container
{
Q_OBJECT
public:
    explicit SliderContainer(QWidget *parent = nullptr);
    ~SliderContainer() override;
    void setListenWidget(QWidget *w) override;

protected:
    void onListenedWidgetEventOccurred(QWidget *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(SliderContainer)
    Q_DISABLE_COPY(SliderContainer)
};


#endif //HUMPTY_QSS_SLIDERCONTAINER_HPP
