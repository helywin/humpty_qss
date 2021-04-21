//
// Created by jiang on 2021/4/20.
//

#ifndef HUMPTY_QSS_QSSEDITOR_HPP
#define HUMPTY_QSS_QSSEDITOR_HPP

#include <QTextEdit>
#include "QssEditor_Export.hpp"

namespace Com
{

class QssEditorConfig;
class QssEditorPrivate;

class COM_QSSEDITOR_API QssEditor : public QTextEdit
{
Q_OBJECT
public:
    explicit QssEditor(QWidget *parent = nullptr);
    ~QssEditor() override;
    void setEditorConfig(const QssEditorConfig &config);;
    QssEditorConfig editorConfig() const;

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    QChar findChar(int offset) const;
    bool matchString(int start, int end, const QString &str);

private:
    Q_DECLARE_PRIVATE(QssEditor)
    Q_DISABLE_COPY(QssEditor)
    QScopedPointer<QssEditorPrivate> d_ptr;
};

}
#endif //HUMPTY_QSS_QSSEDITOR_HPP
