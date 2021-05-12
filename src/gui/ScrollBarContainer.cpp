//
// Created by jiang on 2021/5/7.
//

#include "ScrollBarContainer.hpp"

#include "Container_p.hpp"
#include <QDebug>
#include <QScrollBar>
#include <QStyleOptionSlider>
#include <QMouseEvent>
#include <cassert>


class ScrollBarContainerPrivate : public ContainerPrivate
{
public:
    Q_DECLARE_PUBLIC(ScrollBarContainer)
    QString mOrientation;
    QString mName;
    QString mHandle;
    QString mAddLine;
    QString mSubLine;
    QString mAddPage;
    QString mSubPage;
    QString mLeftArrow;
    QString mRightArrow;
    QString mUpArrow;
    QString mDownArrow;
    bool mHandlePressed = false;

    explicit ScrollBarContainerPrivate(ScrollBarContainer *p);
};

ScrollBarContainerPrivate::ScrollBarContainerPrivate(ScrollBarContainer *p) :
        ContainerPrivate(p)
{
    Q_Q(ScrollBarContainer);
}

ScrollBarContainer::ScrollBarContainer(QWidget *parent) :
        Container(*new ScrollBarContainerPrivate(this), parent)
{
    Q_D(ScrollBarContainer);
}

ScrollBarContainer::~ScrollBarContainer()
{

}

void ScrollBarContainer::setListenWidget(QWidget *w)
{
    Q_D(ScrollBarContainer);
    Container::setListenWidget(w);
    auto scrollBar = dynamic_cast<QScrollBar *>(w);
    if (!scrollBar) {
        std::abort();
    }
    d->mOrientation = scrollBar->orientation() == Qt::Horizontal ? "horizontal" : "vertical";
    QString name = w->metaObject()->className();
    w->dumpObjectTree();
    ControlStates states;
    states |= cs_pressed;
    states |= cs_hover;
    d->mName = name + ":" + d->mOrientation;
    d->mHandle = name + "::handle:" + d->mOrientation;
    d->mAddLine = name + "::add-line:" + d->mOrientation;
    d->mSubLine = name + "::sub-line:" + d->mOrientation;
    d->mAddPage = name + "::add-page:" + d->mOrientation;
    d->mSubPage = name + "::sub-page:" + d->mOrientation;
    d->mLeftArrow = name + "::left-arrow";
    d->mRightArrow = name + "::right-arrow";
    d->mUpArrow = name + "::up-arrow";
    d->mDownArrow = name + "::down-arrow";


    d->addControlStateDisplay(d->mName, states, true);
    d->addControlStateDisplay(d->mHandle, states, false);
    d->addControlStateDisplay(d->mAddLine, states, false);
    d->addControlStateDisplay(d->mSubLine, states, false);
    d->addControlStateDisplay(d->mAddPage, states, false);
    d->addControlStateDisplay(d->mSubPage, states, false);
    if (scrollBar->orientation() == Qt::Horizontal) {
        d->addControlStateDisplay(d->mLeftArrow, states, false);
        d->addControlStateDisplay(d->mRightArrow, states, false);
    } else {
        d->addControlStateDisplay(d->mUpArrow, states, false);
        d->addControlStateDisplay(d->mDownArrow, states, false);
    }
    w->setMouseTracking(true);
    connect(scrollBar, &QScrollBar::sliderPressed, [this] {
        auto d = (ScrollBarContainerPrivate *) d_ptr.data();
        d_ptr->stateDisplay(d->mHandle)->setState(cs_pressed, true);
    });
    connect(scrollBar, &QScrollBar::sliderReleased, [this] {
        auto d = (ScrollBarContainerPrivate *) d_ptr.data();
        d_ptr->stateDisplay(d->mHandle)->setState(cs_pressed, false);
    });
}

void ScrollBarContainer::onListenedWidgetEventOccurred(QWidget *watched, QEvent *event)
{
    Q_D(ScrollBarContainer);
    Container::onListenedWidgetEventOccurred(watched, event);
    auto scrollBar = dynamic_cast<QScrollBar *>(watched);
    if (!scrollBar) {
        std::abort();
    }
    QStyleOptionSlider opt;
    auto mouseEvent = (QMouseEvent *) event;
    opt.initFrom(this);
    opt.subControls = QStyle::SC_None;
    opt.activeSubControls = QStyle::SC_None;
    opt.orientation = scrollBar->orientation();
    opt.minimum = scrollBar->minimum();
    opt.maximum = scrollBar->maximum();
    opt.sliderPosition = scrollBar->sliderPosition();
    opt.sliderValue = scrollBar->value();
    opt.singleStep = scrollBar->singleStep();
    opt.pageStep = scrollBar->pageStep();
    opt.upsideDown = scrollBar->invertedAppearance();
    if (opt.orientation == Qt::Horizontal)
        opt.state |= QStyle::State_Horizontal;

    auto sliderRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider, scrollBar);
    auto subLineRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSubLine, scrollBar);
    auto addLineRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarAddLine, scrollBar);
    auto subPageRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSubPage, scrollBar);
    auto addPageRect = scrollBar->style()
            ->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarAddPage, scrollBar);

    /*****************************************************************************************
     * recalculate position of slider because there is a bug!!!
     * default style slider region start from right of sub-line and end with left of add-line
     * BUT when custom style sheet set, slider region start from left of scrollbar and end
     * with right of scrollbar, see (asset/scrollbar_handle_rect.gif)
    ******************************************************************************************/

    int scrollAreaLen = scrollBar->orientation() == Qt::Horizontal ?
                        scrollBar->rect().width()
                        //                        - subLineRect.width() * 2
                                                                   :
                        scrollBar->rect().height()
//                        - subLineRect.height() * 2
    ;
#if 0
    int sliderPos = QStyle::sliderPositionFromValue(scrollBar->minimum(),
                                                     scrollBar->maximum(),
                                                     scrollBar->sliderPosition(),
                                                     scrollAreaLen - sliderRect.width(),
                                                     scrollBar->invertedAppearance());
#else
    int sliderPos = scrollAreaLen * scrollBar->sliderPosition() /
                    (scrollBar->pageStep() + scrollBar->maximum() - scrollBar->minimum());
#endif
    if (scrollBar->orientation() == Qt::Horizontal) {
        sliderRect.moveLeft(sliderPos);
        addLineRect.moveLeft(scrollBar->width() - addLineRect.width());
        subPageRect.setLeft(subLineRect.right() + 1);
        subPageRect.setRight(sliderRect.left() - 1);
        addPageRect.setRight(addLineRect.left() - 1);
        addPageRect.setLeft(sliderRect.right() + 1);
    } else {
        sliderRect.moveTop(sliderPos);
        addLineRect.moveTop(scrollBar->height() - addLineRect.height());
        subPageRect.moveTop(subLineRect.bottom() + 1);
        subPageRect.moveBottom(sliderRect.top() - 1);
        addPageRect.moveBottom(addLineRect.top() - 1);
        addPageRect.moveTop(sliderRect.bottom() + 1);
    }

    switch (event->type()) {
        case QEvent::MouseMove: {
            d->stateDisplay(d->mName)->setState(cs_hover, true);
            d->stateDisplay(d->mHandle)
                    ->setState(cs_hover, sliderRect.contains(mouseEvent->pos()));
            // only when handle not be dragged should hover event be passed to other sub-controls
            if (!d->mHandlePressed) {
                bool subHover = subLineRect.contains(mouseEvent->pos());
                bool addHover = addLineRect.contains(mouseEvent->pos());
                d->stateDisplay(d->mSubLine)
                        ->setState(cs_hover, subHover);
                d->stateDisplay(d->mAddLine)
                        ->setState(cs_hover, addHover);
                d->stateDisplay(d->mSubPage)
                        ->setState(cs_hover, subPageRect.contains(mouseEvent->pos()));
                d->stateDisplay(d->mAddPage)
                        ->setState(cs_hover, addPageRect.contains(mouseEvent->pos()));
                if (scrollBar->orientation() == Qt::Horizontal) {
                    d->stateDisplay(d->mLeftArrow)->setState(cs_hover, subHover);
                    d->stateDisplay(d->mRightArrow)->setState(cs_hover, addHover);
                } else {
                    d->stateDisplay(d->mUpArrow)->setState(cs_hover, subHover);
                    d->stateDisplay(d->mDownArrow)->setState(cs_hover, addHover);
                }
            }
            break;
        }
        case QEvent::Leave:
            d->stateDisplay(d->mName)->setState(cs_hover, false);
            d->stateDisplay(d->mHandle)->setState(cs_hover, false);
            d->stateDisplay(d->mSubLine)->setState(cs_hover, false);
            d->stateDisplay(d->mAddLine)->setState(cs_hover, false);
            d->stateDisplay(d->mSubPage)->setState(cs_hover, false);
            d->stateDisplay(d->mAddPage)->setState(cs_hover, false);
            if (scrollBar->orientation() == Qt::Horizontal) {
                d->stateDisplay(d->mLeftArrow)->setState(cs_hover, false);
                d->stateDisplay(d->mRightArrow)->setState(cs_hover, false);
            } else {
                d->stateDisplay(d->mUpArrow)->setState(cs_hover, false);
                d->stateDisplay(d->mDownArrow)->setState(cs_hover, false);
            }
            break;
        case QEvent::MouseButtonPress:
            if (mouseEvent->button() == Qt::LeftButton) {
                d->stateDisplay(d->mName)->setState(cs_pressed, true);
                d->mHandlePressed = sliderRect.contains(mouseEvent->pos());
                bool subPressed = subLineRect.contains(mouseEvent->pos());
                bool addPressed = addLineRect.contains(mouseEvent->pos());
                d->stateDisplay(d->mHandle)
                        ->setState(cs_pressed, d->mHandlePressed);
                d->stateDisplay(d->mSubLine)
                        ->setState(cs_pressed, subPressed);
                d->stateDisplay(d->mAddLine)
                        ->setState(cs_pressed, addPressed);
                d->stateDisplay(d->mSubPage)
                        ->setState(cs_pressed, subPageRect.contains(mouseEvent->pos()));
                d->stateDisplay(d->mAddPage)
                        ->setState(cs_pressed, addPageRect.contains(mouseEvent->pos()));
                if (scrollBar->orientation() == Qt::Horizontal) {
                    d->stateDisplay(d->mLeftArrow)->setState(cs_pressed, subPressed);
                    d->stateDisplay(d->mRightArrow)->setState(cs_pressed, addPressed);
                } else {
                    d->stateDisplay(d->mUpArrow)->setState(cs_pressed, subPressed);
                    d->stateDisplay(d->mDownArrow)->setState(cs_pressed, addPressed);
                }
            }

            break;
        case QEvent::MouseButtonRelease:
            d->mHandlePressed = false;
            d->stateDisplay(d->mName)->setState(cs_pressed, false);
            d->stateDisplay(d->mHandle)->setState(cs_pressed, false);
            d->stateDisplay(d->mSubLine)->setState(cs_pressed, false);
            d->stateDisplay(d->mAddLine)->setState(cs_pressed, false);
            d->stateDisplay(d->mSubPage)->setState(cs_pressed, false);
            d->stateDisplay(d->mAddPage)->setState(cs_pressed, false);
            if (scrollBar->orientation() == Qt::Horizontal) {
                d->stateDisplay(d->mLeftArrow)->setState(cs_pressed, false);
                d->stateDisplay(d->mRightArrow)->setState(cs_pressed, false);
            } else {
                d->stateDisplay(d->mUpArrow)->setState(cs_pressed, false);
                d->stateDisplay(d->mDownArrow)->setState(cs_pressed, false);
            }
            break;
        default:
            break;
    }
}
