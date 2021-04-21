//
// Created by jiang on 2021/4/20.
//

#ifndef HUMPTY_QSS_QSSEDITOR_HPP
#define HUMPTY_QSS_QSSEDITOR_HPP

#include <QTextEdit>
#include "QssEditor_Export.hpp"

namespace Com
{

class QssEditorPrivate;

class COM_QSSEDITOR_API QssEditor : public QTextEdit
{
Q_OBJECT
public:
    explicit QssEditor(QWidget *parent = nullptr);
    ~QssEditor() override;

private:
    Q_DECLARE_PRIVATE(QssEditor)
    Q_DISABLE_COPY(QssEditor)
    QScopedPointer<QssEditorPrivate> d_ptr;
};

}
#endif //HUMPTY_QSS_QSSEDITOR_HPP
