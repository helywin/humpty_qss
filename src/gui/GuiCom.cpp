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


QString controlStateToString(ControlState s)
{
    switch (s) {
        case cs_disabled:
            return "disabled";
        case cs_pressed:
            return "pressed";
        case cs_hover:
            return "hover";
        case cs_focus:
            return "focus";
        case cs_unchecked:
            return "unchecked";
        case cs_checked:
            return "checked";
        case cs_indeterminate:
            return "indeterminate";
        case cs_selected:
            return "selected";
        case cs_on:
            return "on";
        default:
            return "";
    }
}