//
// Created by helywin on 2021/4/22.
//

#include "GuiCom.hpp"

const char *cWidgetNames[] = {
        "QWidget",
        "QFrame",
        "QLabel",
        "QPushButton",
        "QToolButton",
        "QLineEdit",
        "QComboBox",
        "QCheckBox",
        "QRadioButton",
        "QMenu",
        "QTabBar",
        "QTabWidget",
};

QString widgetName(WidgetType type)
{
    assert(type >= 0);
    return cWidgetNames[type];
}

WidgetType widgetType(const QString &name)
{
    for (int i = 0; i < wt_widgetCount; ++i) {
        if (cWidgetNames[i] == name) {
            return (WidgetType)i;
        }
    }
    return wt_none;
}
