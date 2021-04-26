//
// Created by helywin on 2021/4/26.
//

#ifndef HUMPTY_QSS_FUNCTIONS_HPP
#define HUMPTY_QSS_FUNCTIONS_HPP

#include <QStringList>
class QWidget;
class QObject;

void printChildrenWidgetInfo(QWidget *w);

QString widgetSubtree(const QWidget *w, int currentLevel = 0,
                      const char *sep = "\n", const char *space = " ");

QString objectSubtree(const QObject *w, int currentLevel = 0,
                      const char *sep = "\n", const char *space = " ");

QList<QObject *> getChildrenByClassNames(const QObject *parent, QStringList names);

#endif //HUMPTY_QSS_FUNCTIONS_HPP
