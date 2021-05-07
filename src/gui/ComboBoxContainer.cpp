//
// Created by jiang on 2021/4/30.
//

#include "ComboBoxContainer.hpp"
#include "Container_p.hpp"
#include <QComboBox>
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QDebug>

class ComboBoxContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(ComboBoxContainer)
    QString mDropDown;
    QString mDownArrow;

    explicit ComboBoxContainerPrivate(ComboBoxContainer *p);
};

ComboBoxContainerPrivate::ComboBoxContainerPrivate(ComboBoxContainer *p) :
        ContainerPrivate(p)
{

}

ComboBoxContainer::ComboBoxContainer(QWidget *parent) :
        Container(*new ComboBoxContainerPrivate(this), parent)
{

}

ComboBoxContainer::~ComboBoxContainer()
{

}

void ComboBoxContainer::setListenWidget(QWidget *w)
{
    Q_D(ComboBoxContainer);
    Container::setListenWidget(w);
    auto comboBox = dynamic_cast<QComboBox *>(w);
    QString name = w->metaObject()->className();
    ControlStates states{cs_none};
    states |= cs_hover;
    states |= cs_focus;
    states |= cs_on;
    if (!comboBox->isEditable()) {
        name += "[read-only]";
        states ^= cs_focus;
    }
    if (!comboBox->isEnabled()) {
        states = cs_disabled;
    }
    d->mMainControlName = name;
    d->addControlStateDisplay(d->mMainControlName, states);
    d->mDropDown = QString(w->metaObject()->className()) + "::drop-down";
    d->mDownArrow = QString(w->metaObject()->className()) + "::down-arrow";
    if (comboBox->isEnabled()) {
        d->addControlStateDisplay(d->mDropDown, cs_pressed, false);
        d->addControlStateDisplay(d->mDownArrow, cs_pressed, false);
    } else {
        d->addControlStateDisplay(d->mDropDown, cs_disabled, false);
        d->addControlStateDisplay(d->mDownArrow, cs_disabled, false);
    }
#ifdef _WINDOWS
    setListenGlobalMouseEvent(true);
#endif
    comboBox->view()->installEventFilter(this);
}

void ComboBoxContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(ComboBoxContainer);
    if (watched->metaObject()->className() == QString("QComboBoxListView")) {
        if (event->type() == QEvent::FocusIn) {
            d->mainStateDisplay()->setState(cs_on, true);
        } else if (event->type() == QEvent::FocusOut) {
            d->mainStateDisplay()->setState(cs_on, false);
        }
        return;
    }
    switch (event->type()) {
        case QEvent::Enter:
//            qDebug() << "enter";
//            qDebug() << d->mMainControlName;
            d->mainStateDisplay()->setState(cs_hover, true);
            break;
        case QEvent::Leave:
            d->mainStateDisplay()->setState(cs_hover, false);
            break;
        case QEvent::MouseButtonPress: {
            auto comboBox = dynamic_cast<QComboBox *>(watched);
            if (event->type() == QEvent::MouseButtonPress &&
                dynamic_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) {
                auto mouseEvent = (QMouseEvent *) event;
                QStyleOptionComboBox opt;
                opt.initFrom(d->mListenWidget);
                opt.editable = comboBox->isEditable();
                opt.subControls = QStyle::SC_All;
                auto subControl = d->mListenWidget->style()
                        ->hitTestComplexControl(QStyle::CC_ComboBox, &opt,
                                                mouseEvent->pos(),
                                                d->mListenWidget);
                if (subControl == QStyle::SC_ComboBoxArrow) {
                    d->stateDisplay(d->mDropDown)->setState(cs_pressed, true);
                    d->stateDisplay(d->mDownArrow)->setState(cs_pressed, true);
                }
            }
            break;
        }
        case QEvent::FocusIn: {
            auto comboBox = dynamic_cast<QComboBox *>(watched);
            if (comboBox && comboBox->isEditable()) {
                d->mainStateDisplay()->setState(cs_focus, true);
            }
            break;
        }
        case QEvent::FocusOut: {
            auto comboBox = dynamic_cast<QComboBox *>(watched);
            if (comboBox && comboBox->isEditable()) {
                d->mainStateDisplay()->setState(cs_focus, false);
            }
            break;
        }
        default:
            break;
    }
}

void ComboBoxContainer::onGlobalMouseEvent(QEvent::Type type, Qt::MouseButton button)
{
    Q_D(ComboBoxContainer);
    switch (type) {
        case QEvent::MouseButtonRelease:
            d->stateDisplay(d->mDropDown)->setState(cs_pressed, false);
            d->stateDisplay(d->mDownArrow)->setState(cs_pressed, false);
            break;
        default:
            break;
    }
}
