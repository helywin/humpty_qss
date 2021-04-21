//
// Created by jiang on 2021/4/21.
//

#ifndef HUMPTY_QSS_QSSEDITORCONFIG_HPP
#define HUMPTY_QSS_QSSEDITORCONFIG_HPP

#include "QssEditor_Export.hpp"
#include <QString>

namespace Com
{

class QssEditorConfigPrivate;
class COM_QSSEDITOR_API QssEditorConfig
{
public:
    QssEditorConfig();
    QssEditorConfig(const QssEditorConfig &);
    QssEditorConfig &operator=(const QssEditorConfig &);
    ~QssEditorConfig();

    void setTabReplace(bool enabled);
    bool tabReplace() const;
    void setTabReplaceSize(int size);
    int tabReplaceSize() const;
    QString tab() const;
    void setAutoParentheses(bool enabled);
    bool autoParentheses() const;
    void setAutoIndentation(bool enable);
    bool autoIndentation() const;

private:
    QssEditorConfigPrivate *d;
};

}

#endif //HUMPTY_QSS_QSSEDITORCONFIG_HPP
