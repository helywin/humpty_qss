//
// Created by helywin on 2021/4/26.
//

#include "Functions.hpp"

#include <QWidget>
#include <iostream>

void printChildrenWidgetInfo(QWidget *w)
{
    bool hasChildWidget = false;
    QString text = w->metaObject()->className() + QString(":\n");
    for (auto child : w->children()) {
        if (child->inherits("QWidget")) {
            text += child->metaObject()->className() + QString(" ");
            if (!child->dynamicPropertyNames().isEmpty()) {
                text += QString("properties: ") +
                        child->dynamicPropertyNames().join("  ");
            }
            hasChildWidget = true;
        }
    }
    if (hasChildWidget) {
        std::cout << text.toStdString() << std::endl;
    }
}

QString widgetSubtree(const QWidget *w, int currentLevel,
                      const char *sep, const char *space)
{
    QString text;
    if (currentLevel != 0) {
        text += sep;
    }
    QString indent = QString(space).repeated(currentLevel * 4);
    text += indent + w->metaObject()->className();
    if (!w->objectName().isEmpty()) {
        text += QString(":") + space + w->objectName();
    }
    for (const auto child : w->children()) {
        if (!child->inherits("QWidget")) {
            continue;
        }
        text += widgetSubtree((QWidget *) child, currentLevel + 1, sep, space);
    }
    return text;
}

QString objectSubtree(const QObject *w, int currentLevel,
                      const char *sep, const char *space)
{
    QString text;
    if (currentLevel != 0) {
        text += sep;
    }
    QString indent = QString(space).repeated(currentLevel * 4);
    text += indent + w->metaObject()->className();
    if (!w->objectName().isEmpty()) {
        text += QString(": ") + w->objectName();
    }
    for (const auto child : w->children()) {
        if (!child->inherits("QObject")) {
            continue;
        }
        text += objectSubtree(child, currentLevel + 1, sep, space);
    }
    return text;
}

QList<QObject *> getChildrenByClassNames(const QObject *parent, QStringList names)
{
    QList<QObject *> list;
    if (parent->children().isEmpty() || names.isEmpty()) {
        return {};
    }
    if (names.size() == 1) {
        for (const auto child : parent->children()) {
            if (child->metaObject()->className() == names.first()) {
                list.append(child);
            }
        }
    } else {
        for (const auto child : parent->children()) {
            if (child->metaObject()->className() == names.first()) {
                names.pop_front();
                list.append(getChildrenByClassNames(child, names));
            }
        }
    }
    return list;
}