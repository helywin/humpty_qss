//
// Created by helywin on 2020/9/5.
//

#include "Utils.hpp"
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QSettings>
#include <QEvent>

namespace Com
{
void drawPrimitive(QWidget *w)
{
    QStyleOption opt;
    opt.initFrom(w);
    QPainter p(w);
    w->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, w);
}

void setIfNotExists(QSettings *settings, const QString &key, const QVariant &variant)
{
    if (settings->allKeys().contains(key)) {
        return;
    }
    settings->setValue(key, variant);
}

class EventBlocker : public QObject
{
Q_OBJECT
public:
    explicit EventBlocker(EventFlags flags, QObject *parent = nullptr) :
            QObject(parent), mFlags(flags)
    {}

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (mFlags.testFlag(event->type())) {
            return true;
        }
        return false;
    }

private:
    EventFlags mFlags;
};


void blockEvent(QWidget *widget, EventFlags flags, QObject *parent)
{
    auto blocker = new EventBlocker(flags, parent);
    widget->installEventFilter(blocker);
}

void blockEvent(QWidget *widget, EventBlockFlags flags, QObject *parent)
{
    EventFlags events;
    if (flags.testFlag(eb_mouse)) {
        events.operator|=(QEvent::MouseButtonPress);
        events.operator|=(QEvent::MouseButtonRelease);
        events.operator|=(QEvent::MouseButtonDblClick);
        events.operator|=(QEvent::MouseMove);
        events.operator|=(QEvent::Enter);
        events.operator|=(QEvent::Leave);
        events.operator|=(QEvent::Wheel);
    }

    if (flags.testFlag(eb_key)) {
        events.operator|=(QEvent::KeyPress);
        events.operator|=(QEvent::KeyRelease);
    }
    blockEvent(widget, events, parent);
}

void changeFonts(QWidget *widget, const QString &family)
{
    auto currentFont = widget->font();
    currentFont.setFamily(family);
    widget->setFont(currentFont);
    for (auto child : widget->children()) {
        auto w = dynamic_cast<QWidget *>(child);
        if (w) {
            changeFonts(w, family);
        }
    }

}

}   // namespace Com

#include "Utils.moc"