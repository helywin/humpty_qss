//
// Created by jiang on 2021/4/18.
//

#include "Showcase.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QVariant>
#include "Utils.hpp"

using namespace Com;

class ShowcasePrivate
{
public:
    Q_DECLARE_PUBLIC(Showcase)
    Showcase *q_ptr;
    QVBoxLayout *mLayout;
    QWidget *mContent;
    QLabel *mLabel;

    explicit ShowcasePrivate(Showcase *p, QWidget *content);
};

ShowcasePrivate::ShowcasePrivate(Showcase *p, QWidget *content) :
        q_ptr(p)
{
    Q_Q(Showcase);
    initWidget(mLayout, q);
    initWidget(mLabel, q);

    mContent = content;
    mContent->setProperty("test", true);
    mContent->setParent(q);
    mLayout->addWidget(mContent, 1, Qt::AlignHCenter);
    mLayout->addWidget(mLabel, 0, Qt::AlignHCenter);
    mLabel->setText(content->windowTitle());
}

Showcase::Showcase(QWidget *content, QWidget *parent) :
        QWidget(parent),
        d_ptr(new ShowcasePrivate(this, content))
{
    connect(content, &QWidget::windowTitleChanged, [this](const QString &title) {
        d_ptr->mLabel->setText(title);
    });
}

Showcase::~Showcase()
{

}

void Showcase::setWidget(QWidget *w)
{
    Q_D(Showcase);
    d->mLayout->removeWidget(d->mContent);
    d->mContent = w;
    d->mContent->setParent(this);
    d->mLayout->insertWidget(0, w, 1);
}
