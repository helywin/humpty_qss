//
// Created by helywin on 2021/4/22.
//

#ifndef HUMPTY_QSS_GUICOM_HPP
#define HUMPTY_QSS_GUICOM_HPP

#include <QString>

class QDebug;

enum WidgetType
{
    wt_none = -1,
    wt_widget,
    wt_frame,
    wt_label,
    wt_pushButton,
    wt_toolButton,
    wt_lineEdit,
    wt_comboBox,
    wt_checkBox,
    wt_radioButton,
    wt_menu,
    wt_tabBar,
    wt_tabWidget,
    wt_widgetCount,
};

QString widgetName(WidgetType type);
WidgetType widgetType(const QString &name);

enum ControlState {
    cs_none = 0x0000,
    cs_stateFirst = 1 << 0,
    cs_disabled = 1 << 0,
    cs_pressed = 1 << 1,
    cs_hover = 1 << 2,
    cs_focus = 1 << 3,
    cs_unchecked = 1 << 4,
    cs_checked = 1 << 5,
    cs_indeterminate = 1 << 6,
    cs_selected = 1 << 7,
    cs_on = 1 << 8,
    cs_stateLast = cs_on,
};

QString controlStateToString(ControlState s);
QDebug &operator<<(QDebug &debug, ControlState s);

Q_DECLARE_FLAGS(ControlStates, ControlState)


#endif //HUMPTY_QSS_GUICOM_HPP
