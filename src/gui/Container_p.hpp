//
// Created by jiang on 2021/4/27.
//

#ifndef HUMPTY_QSS_CONTAINER_P_HPP
#define HUMPTY_QSS_CONTAINER_P_HPP

#include <QMap>
#include <QLabel>
#include <QFormLayout>
#include "StateDisplay.hpp"


class ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(Container)
    Container *q_ptr;
    QMap<QString, StateDisplay *> mStateDisplay;
    QVBoxLayout *mLayout;
    QLabel *mTitle;
    QWidget *mStatesContainer;
    QFormLayout *mStatesLayout;
    QMap<QString, StateDisplay *> mStateDisplayList;
    bool mListenGlobalMouseEvent = false;
    QWidget *mListenWidget = nullptr;

    explicit ContainerPrivate(Container *p);
};

#endif //HUMPTY_QSS_CONTAINER_P_HPP
