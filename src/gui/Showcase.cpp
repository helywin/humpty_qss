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
#include <QtWidgets>
#include "Utils.hpp"

using namespace Com;

struct ControlDetail
{
    QLabel *display = nullptr;
    QSet<QString> states;
    QString type;

    ControlDetail() = default;

    explicit ControlDetail(QLabel *label) : display(label)
    {}

    ControlDetail(const ControlDetail &detail) = default;
    ControlDetail(ControlDetail &&detail) = default;
    ControlDetail &operator=(const ControlDetail &detail) = default;

};

class ShowcasePrivate
{
public:
    Q_DECLARE_PUBLIC(Showcase)
    Showcase *q_ptr;
    QVBoxLayout *mLayout;
    QWidget *mContent;
    QMap<QString, ControlDetail> mControlDetails;
    QString mControlName;

    explicit ShowcasePrivate(Showcase *p, QWidget *content, ShowcaseWidgetPosition pos);
    void updateTitle(const QString &name);
    void updateAllControlTitle();
    void updateMainControlTitle();
    void setWidget(QWidget *w, ShowcaseWidgetPosition pos);
    ControlDetail &mainControlDetail();
};

ShowcasePrivate::ShowcasePrivate(Showcase *p, QWidget *content, ShowcaseWidgetPosition pos) :
        q_ptr(p)
{
    Q_Q(Showcase);
    initWidget(mLayout, q);
    setWidget(content, pos);
}

void ShowcasePrivate::updateTitle(const QString &name)
{
    assert(mControlDetails.contains(name));
    const auto &detail = mControlDetails[name];
    QString text = name;
    if (!detail.type.isEmpty()) {
        text += "[" + detail.type + "]";
    }
    for (const auto &state : detail.states) {
        text += ":" + state;
    }
    detail.display->setText(text);
}

void ShowcasePrivate::updateMainControlTitle()
{
    updateTitle(mControlName);
}

void ShowcasePrivate::updateAllControlTitle()
{
    for (const auto &key : mControlDetails.keys()) {
        updateTitle(key);
    }
}

ControlDetail &ShowcasePrivate::mainControlDetail()
{
    return mControlDetails[mControlName];
}

void ShowcasePrivate::setWidget(QWidget *w, ShowcaseWidgetPosition pos)
{
    Q_Q(Showcase);
    if (mContent) {
        mContent->removeEventFilter(q);
        mLayout->removeWidget(mContent);
    }
    mContent = w;
    mContent->setProperty("test", true);
    mContent->setParent(q);

    mControlName = w->metaObject()->className();
    mainControlDetail() = ControlDetail{new QLabel(q)};

    if (!mContent->isEnabled()) {
        mainControlDetail().states.insert("disabled");
        goto set_layout;
    } else {
        mContent->installEventFilter(q);
    }

    auto dPushButton = dynamic_cast<QPushButton *>(mContent);
    if (dPushButton) {
        QObject::connect(dPushButton, &QPushButton::pressed, [this] {
            mainControlDetail().states.insert("pressed");
            updateMainControlTitle();
        });
        QObject::connect(dPushButton, &QPushButton::released, [this] {
            mainControlDetail().states.remove("pressed");
            updateMainControlTitle();
        });

        if (dPushButton->isCheckable()) {
            if (dPushButton->isChecked()) {
                mainControlDetail().states.insert("checked");
            }
            QObject::connect(dPushButton, &QPushButton::clicked, [this](bool checked) {
                if (checked) {
                    mainControlDetail().states.insert("checked");
                } else {
                    mainControlDetail().states.remove("checked");
                }
                updateMainControlTitle();
            });
        }
    }

    auto dToolButton = dynamic_cast<QToolButton *>(mContent);
    if (dToolButton) {
        QObject::connect(dToolButton, &QToolButton::pressed, [this] {
            mainControlDetail().states.insert("pressed");
            updateMainControlTitle();
        });
        QObject::connect(dToolButton, &QToolButton::released, [this] {
            mainControlDetail().states.remove("pressed");
            updateMainControlTitle();
        });

        if (dToolButton->isCheckable()) {
            if (dToolButton->isChecked()) {
                mainControlDetail().states.insert("checked");
            }
            QObject::connect(dToolButton, &QToolButton::clicked, [this](bool checked) {
                if (checked) {
                    mainControlDetail().states.insert("checked");
                } else {
                    mainControlDetail().states.remove("checked");
                }
                updateMainControlTitle();
            });
        }
    }

    auto dLineEdit = dynamic_cast<QLineEdit *>(mContent);
    if (dLineEdit) {
        if (dLineEdit->isReadOnly()) {
            mainControlDetail().type = "read-only";
        }
        if (dLineEdit->echoMode() == QLineEdit::Password) {
            mainControlDetail().type = "echoMode=\"2\"";
        }
    }

    auto dComboBox = dynamic_cast<QComboBox *>(mContent);
    if (dComboBox) {
        if (!dComboBox->isEditable()) {
            mainControlDetail().type = "read-only";
        }

        qDebug() << dComboBox->children();
    }

set_layout:
    for (const auto &v  : mControlDetails) {
        mLayout->addWidget(v.display, 0, Qt::AlignCenter);
    }
    if (pos == slp_south) {
        mLayout->addWidget(mContent, 0, Qt::AlignCenter);
    } else {
        mLayout->insertWidget(0, mContent, 0, Qt::AlignCenter);
    }

    updateAllControlTitle();
}

Showcase::Showcase(QWidget *content, QWidget *parent, ShowcaseWidgetPosition pos) :
        QWidget(parent),
        d_ptr(new ShowcasePrivate(this, content, pos))
{}

Showcase::~Showcase()
{

}

bool Showcase::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(Showcase);
    if (!watched) {
        return false;
    }
    if (event->type() == QEvent::Enter) {
        d->mainControlDetail().states.insert("hover");
    } else if (event->type() == QEvent::Leave) {
        d->mainControlDetail().states.remove("hover");
    }
    if (dynamic_cast<QLineEdit *>(watched)) {
        if (event->type() == QEvent::FocusIn) {
            qDebug() << "focus in";
            d->mainControlDetail().states.insert("focus");
        } else if (event->type() == QEvent::FocusOut) {
            d->mainControlDetail().states.remove("focus");
        }
    }

    d->updateMainControlTitle();
    return QObject::eventFilter(watched, event);
}
