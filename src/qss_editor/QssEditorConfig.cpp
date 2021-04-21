//
// Created by jiang on 2021/4/21.
//

#include "QssEditorConfig.hpp"
namespace Com
{

class QssEditorConfigPrivate
{
public:
    bool mTabReplace = false;
    int mTabReplaceSize = 4;
    QString mTab = "\t";
    bool mAutoParentheses = false;
    bool mAutoIndentation = false;

    QssEditorConfigPrivate &operator=(const QssEditorConfigPrivate &) = default;
    void updateTab();
};

void QssEditorConfigPrivate::updateTab()
{
    if (mTabReplace) {
        mTab = QString(mTabReplaceSize, QChar(' '));
    } else {
        mTab = "\t";
    }
}

QssEditorConfig::QssEditorConfig() :
        d(new QssEditorConfigPrivate)
{

}

QssEditorConfig::~QssEditorConfig()
{
    delete d;
}

void QssEditorConfig::setTabReplace(bool enabled)
{
    d->mTabReplace = enabled;
    d->updateTab();
}

bool QssEditorConfig::tabReplace() const
{
    return  d->mTabReplace;
}

void QssEditorConfig::setTabReplaceSize(int size)
{
    d->mTabReplaceSize = size;
    d->updateTab();
}

int QssEditorConfig::tabReplaceSize() const
{
    return d->mTabReplaceSize;
}

void QssEditorConfig::setAutoParentheses(bool enabled)
{
    d->mAutoParentheses = enabled;
}

bool QssEditorConfig::autoParentheses() const
{
    return d->mAutoParentheses;
}

void QssEditorConfig::setAutoIndentation(bool enable)
{
    d->mAutoIndentation = enable;
}

bool QssEditorConfig::autoIndentation() const
{
    return d->mAutoIndentation;
}

QString QssEditorConfig::tab() const
{
    return d->mTab;
}

QssEditorConfig::QssEditorConfig(const QssEditorConfig &other)
{
    *this = other;
}

QssEditorConfig &QssEditorConfig::operator=(const QssEditorConfig &other)
{
    if (this != &other) {
        *(this->d) = *other.d;
    }
    return *this;
}


}