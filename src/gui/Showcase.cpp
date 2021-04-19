//
// Created by jiang on 2021/4/18.
//

#include "Showcase.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QEvent>
#include <QMouseEvent>
#include <QSet>
#include <QDebug>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
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
    QSet<QString> mContentStates;
    QString mType;

    explicit ShowcasePrivate(Showcase *p, QWidget *content);
    void updateTitle();
};

ShowcasePrivate::ShowcasePrivate(Showcase *p, QWidget *content) :
        q_ptr(p)
{
    Q_Q(Showcase);
    initWidget(mLayout, q);
    initWidget(mLabel, q);

    mLayout->addWidget(mLabel, 0, Qt::AlignHCenter);
    mLabel->setText(content->windowTitle());
}

void ShowcasePrivate::updateTitle()
{
    QString text = mContent->metaObject()->className();
    if (!mType.isEmpty()) {
        text += "[" + mType + "]";
    }
    for (const auto &state : mContentStates) {
        text += ":" + state;
    }
    mLabel->setText(text);
}

Showcase::Showcase(QWidget *content, QWidget *parent) :
        QWidget(parent),
        d_ptr(new ShowcasePrivate(this, content))
{
    connect(content, &QWidget::windowTitleChanged, [this](const QString &title) {
        d_ptr->mLabel->setText(title);
    });

    setWidget(content);

}

Showcase::~Showcase()
{

}

void Showcase::setWidget(QWidget *w)
{
    Q_D(Showcase);
    if (d->mContent) {
        d->mContent->removeEventFilter(this);
        d->mLayout->removeWidget(d->mContent);
    }
    d->mContent = w;
    d->mContent->setProperty("test", true);
    d->mContent->setParent(this);
    d->mLayout->insertWidget(0, w, 1);

    if (!d->mContent->isEnabled()) {
        d->mContentStates.insert("disabled");
        d->updateTitle();
        return;
    } else {
        d->mContent->installEventFilter(this);

    }

    auto dPushButton = dynamic_cast<QPushButton *>(d->mContent);
    if (dPushButton) {
        connect(dPushButton, &QPushButton::pressed, [this] {
            d_ptr->mContentStates.insert("pressed");
            d_ptr->updateTitle();
        });
        connect(dPushButton, &QPushButton::released, [this] {
            d_ptr->mContentStates.remove("pressed");
            d_ptr->updateTitle();
        });

        if (dPushButton->isCheckable()) {
            if (dPushButton->isChecked()) {
                d_ptr->mContentStates.insert("checked");
            }
            connect(dPushButton, &QPushButton::clicked, [this](bool checked) {
                if (checked) {
                    d_ptr->mContentStates.insert("checked");
                } else {
                    d_ptr->mContentStates.remove("checked");
                }
                d_ptr->updateTitle();
            });
        }
    }

    auto dToolButton = dynamic_cast<QToolButton *>(d->mContent);
    if (dToolButton) {
        connect(dToolButton, &QToolButton::pressed, [this] {
            d_ptr->mContentStates.insert("pressed");
            d_ptr->updateTitle();
        });
        connect(dToolButton, &QToolButton::released, [this] {
            d_ptr->mContentStates.remove("pressed");
            d_ptr->updateTitle();
        });

        if (dToolButton->isCheckable()) {
            if (dToolButton->isChecked()) {
                d_ptr->mContentStates.insert("checked");
            }
            connect(dToolButton, &QToolButton::clicked, [this](bool checked) {
                if (checked) {
                    d_ptr->mContentStates.insert("checked");
                } else {
                    d_ptr->mContentStates.remove("checked");
                }
                d_ptr->updateTitle();
            });
        }
    }

    auto dLineEdit = dynamic_cast<QLineEdit *>(d->mContent);
    if (dLineEdit) {
        if (dLineEdit->isReadOnly()) {
            d_ptr->mType = "read-only";
        }
        if (dLineEdit->echoMode() == QLineEdit::Password) {
            d_ptr->mType = "echoMode=\"2\"";
        }
    }
    d->updateTitle();
}

bool Showcase::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(Showcase);
    if (!watched) {
        return false;
    }
    if (event->type() == QEvent::Enter) {
        d->mContentStates.insert("hover");
    } else if (event->type() == QEvent::Leave) {
        d->mContentStates.remove("hover");
    }
    if (dynamic_cast<QLineEdit *>(watched)) {
        if (event->type() == QEvent::FocusIn) {
            qDebug() << "focus in";
            d->mContentStates.insert("focus");
        } else if (event->type() == QEvent::FocusOut) {
            d->mContentStates.remove("focus");
        }
    }

    d->updateTitle();
    return QObject::eventFilter(watched, event);
}
