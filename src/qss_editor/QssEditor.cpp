//
// Created by jiang on 2021/4/20.
//

#include "QssEditor.hpp"

namespace Com
{
class QssEditorPrivate
{
public:
    Q_DECLARE_PUBLIC(QssEditor)
    QssEditor *q_ptr;

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
}