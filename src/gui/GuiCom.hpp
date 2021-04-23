//
// Created by helywin on 2021/4/22.
//

#ifndef HUMPTY_QSS_GUICOM_HPP
#define HUMPTY_QSS_GUICOM_HPP

#include <QString>

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
    wt_widgetCount,
};

QString widgetName(WidgetType type);
WidgetType widgetType(const QString &name);


#endif //HUMPTY_QSS_GUICOM_HPP
