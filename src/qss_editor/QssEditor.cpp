//
// Created by jiang on 2021/4/20.
//

#include "QssEditor.hpp"
#include "QssEditorConfig.hpp"
#include <QTextBlock>
#include <QDebug>
#include <QMimeData>

namespace Com
{
class QssEditorPrivate
{
public:
    Q_DECLARE_PUBLIC(QssEditor)
    QssEditor *q_ptr;
    QssEditorConfig mConfig;

    explicit QssEditorPrivate(QssEditor *p);
};

QssEditorPrivate::QssEditorPrivate(QssEditor *p) :
        q_ptr(p)
{

}

QssEditor::QssEditor(QWidget *parent) :
        QTextEdit(parent),
        d_ptr(new QssEditorPrivate(this))
{

}

QssEditor::~QssEditor()
{

}

void QssEditor::setEditorConfig(const QssEditorConfig &config)
{
    Q_D(QssEditor);
    d->mConfig = config;
}

QssEditorConfig QssEditor::editorConfig() const
{
    Q_D(const QssEditor);
    return d->mConfig;
}

void QssEditor::keyPressEvent(QKeyEvent *e)
{
    Q_D(QssEditor);
//    qDebug() << d->mConfig.autoIndentation() << (e->key() == Qt::Key_Return);
    if (d->mConfig.autoIndentation() && (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        && matchString(-1, 1, "{}")) {
//        qDebug() << "|" << d->mConfig.tab() << "|";
        insertPlainText("\n" + d->mConfig.tab() + "\n");
        moveCursor(QTextCursor::Left, QTextCursor::MoveAnchor);
        return;
    }
    if (d->mConfig.tabReplace() && (e->modifiers() == Qt::NoModifier && e->key() == Qt::Key_Tab)) {

    }
    QTextEdit::keyPressEvent(e);
}

QChar QssEditor::findChar(int offset) const
{
    auto block = textCursor().blockNumber();
    auto index = textCursor().positionInBlock();
    auto text = document()->findBlockByNumber(block).text();

    index += offset;

    if (index < 0 || index >= text.size()) {
        return {};
    }

    return text[index];
}

bool QssEditor::matchString(int start, int end, const QString &str)
{
    assert(end > start);
    auto index = textCursor().positionInBlock();
    auto text = textCursor().block().text();

    index += start;
    if (index < 0 || index >= text.size()) {
        return false;
    }
//    qDebug() << text.mid(index, end - start);
    return text.mid(index, end - start) == str;
}

void QssEditor::insertFromMimeData(const QMimeData *source)
{
    QTextEdit::insertPlainText(source->text());
}

QString QssEditor::cursorLeftText() const
{
    return QString();
//    std::all_of(str.begin(), str.end(), [](QChar c) { return c == ' '; });
}

QString QssEditor::cursorRightText() const
{
    return QString();
}

bool QssEditor::surroundWithBrace(QPoint &start, QPoint &end)
{
    return false;
}

int QssEditor::widthBeyondIndent() const
{
    return 0;
}

int QssEditor::widthUnderIndent() const
{
    return 0;
}

}