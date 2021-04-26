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
#include <QTimer>
#include <iostream>
#include <QtWidgets>
#include "Utils.hpp"
#include "GuiCom.hpp"
#include "GlobalMouseListener.hpp"
#include "Functions.hpp"

#define QCHECKBOX_INDICATOR "QCheckBox::indicator"
#define QRADIOBUTTON_INDICATOR "QRadioButton::indicator"
#define QCOMBOBOX_DROPDOWN "QComboBox::drop-down"
#define QCOMBOBOX_DOWNARROW "QComboBox::down-arrow"
#define QMENU_ITEM "QMenu::item"
#define QMENU_ICON "QMenu::icon"
#define QMENU_INDICATOR_EXCLUSIVE "QMenu::indicator:exclusive"
#define QMENU_INDICATOR_NONEEXCLUSIVE "QMenu::indicator:non-exclusive"
#define QMENU_RIGHTARROW "QMenu::right-arrow"
#define QTABBAR_TAB "QTabBar::tab"
#define QTABBAR_FIRST "QTabBar:first"
#define QTABBAR_LAST "QTabBar:last"
#define QTABBAR_CLOSEBUTTON "QTabBar:close-button"


using namespace Com;

struct ControlDetail
{
    QLabel *display = nullptr;
    QWidget *control = nullptr;
    QSet<QString> states;
    QString type;
    QString objectTree;

    ControlDetail() = default;

    explicit ControlDetail(QWidget *parent) :
            display(new QLabel(parent))
    {}

    void setControl(QWidget *c)
    {
        control = c;
        display->setToolTip("Object&nbsp;Tree<br>" + objectSubtree(c, 0, "<br>", "&nbsp;"));
    }

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
    EventListener *mEventListener;

    explicit ShowcasePrivate(Showcase *p, QWidget *content, ShowcaseWidgetPosition pos,
                             QWidget *container);
    void updateTitle(const QString &name);
    void updateAllControlTitle();
    void updateMainControlTitle();
    void setWidget(QWidget *w, ShowcaseWidgetPosition pos, QWidget *container);
    ControlDetail &mainControlDetail();
    void onEventOccurred(QObject *watched, QEvent *event);
};

ShowcasePrivate::ShowcasePrivate(Showcase *p, QWidget *content, ShowcaseWidgetPosition pos,
                                 QWidget *container) :
        q_ptr(p)
{
    Q_Q(Showcase);
    initWidget(mLayout, q);
    initWidget(mEventListener, q);
    setWidget(content, pos, container);
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

void ShowcasePrivate::setWidget(QWidget *w, ShowcaseWidgetPosition pos, QWidget *container)
{
    Q_Q(Showcase);
    if (mContent) {
        mContent->removeEventFilter(q);
        mLayout->removeWidget(mContent);
    }
    mContent = w;
    mContent->setProperty("test", true);

    mControlName = w->metaObject()->className();
    mContent->setProperty("controlName", mControlName);
    mainControlDetail() = ControlDetail(q);
    mainControlDetail().setControl(mContent);

//    printChildrenWidgetInfo(mContent);
    std::cout << objectSubtree(mContent).toStdString() << std::endl;
    std::cout << "========================" << std::endl;


    if (!mContent->isEnabled()) {
        mainControlDetail().states.insert("disabled");
        goto set_layout;
    } else {
        mContent->installEventFilter(mEventListener);
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
        mControlDetails[QCOMBOBOX_DROPDOWN] = ControlDetail(q);
        mControlDetails[QCOMBOBOX_DOWNARROW] = ControlDetail(q);
        if (!dComboBox->isEditable()) {
            mainControlDetail().type = "read-only";
        }

//        auto childList = getChildrenByClassNames(dComboBox, {"QComboBoxPrivateContainer",
//                                                            "QComboBoxListView"});
//        if (!childList.isEmpty()) {
        dComboBox->view()->installEventFilter(mEventListener);
        // listen to system event through win api
        QObject::connect(GlobalMouseListener::instance(), &GlobalMouseListener::mouseEvent,
                         [this](QEvent::Type type, Qt::MouseButton button) {
                             //qDebug() << "get response";
                             if (type == QEvent::MouseButtonRelease && button == Qt::LeftButton) {
                                 mControlDetails[QCOMBOBOX_DROPDOWN].states.remove("pressed");
                                 mControlDetails[QCOMBOBOX_DOWNARROW].states.remove("pressed");
                             }
                         });
//        }
    }
    //indeterminate
    auto dCheckBox = dynamic_cast<QCheckBox *>(mContent);
    if (dCheckBox) {
        mControlDetails[QCHECKBOX_INDICATOR] = ControlDetail(q);
        QObject::connect(dCheckBox, &QCheckBox::stateChanged, [this](int state) {
            mainControlDetail().states.remove("checked");
            mainControlDetail().states.remove("indeterminate");
            mControlDetails[QCHECKBOX_INDICATOR].states.remove("checked");
            mControlDetails[QCHECKBOX_INDICATOR].states.remove("indeterminate");
            if (state == Qt::Checked) {
                mainControlDetail().states.insert("checked");
                mControlDetails[QCHECKBOX_INDICATOR].states.insert("checked");
            } else if (state == Qt::PartiallyChecked) {
                mainControlDetail().states.insert("indeterminate");
                mControlDetails[QCHECKBOX_INDICATOR].states.insert("indeterminate");
            }
            updateAllControlTitle();
        });
        QObject::connect(dCheckBox, &QCheckBox::pressed, [this] {
            mainControlDetail().states.insert("pressed");
            mControlDetails[QCHECKBOX_INDICATOR].states.insert("pressed");
            updateAllControlTitle();
        });
        QObject::connect(dCheckBox, &QCheckBox::released, [this] {
            mainControlDetail().states.remove("pressed");
            mControlDetails[QCHECKBOX_INDICATOR].states.remove("pressed");
            updateAllControlTitle();
        });
    }

    auto dRadioButton = dynamic_cast<QRadioButton *>(mContent);
    if (dRadioButton) {
        mControlDetails[QRADIOBUTTON_INDICATOR] = ControlDetail(q);
        QObject::connect(dRadioButton, &QRadioButton::clicked, [this](bool checked) {
            if (checked) {
                mControlDetails[QRADIOBUTTON_INDICATOR].states.insert("checked");
                mainControlDetail().states.insert("checked");
            } else {
                mControlDetails[QRADIOBUTTON_INDICATOR].states.remove("checked");
                mainControlDetail().states.remove("checked");
            }
            updateAllControlTitle();
        });
        QObject::connect(dRadioButton, &QRadioButton::pressed, [this] {
            mainControlDetail().states.insert("pressed");
            mControlDetails[QRADIOBUTTON_INDICATOR].states.insert("pressed");
            updateAllControlTitle();
        });
        QObject::connect(dRadioButton, &QRadioButton::released, [this] {
            mainControlDetail().states.remove("pressed");
            mControlDetails[QRADIOBUTTON_INDICATOR].states.remove("pressed");
            updateAllControlTitle();
        });
    }

    auto dMenu = dynamic_cast<QMenu *>(mContent);
    if (dMenu) {
        mControlDetails[QMENU_ITEM] = ControlDetail(q);
        mControlDetails[QMENU_ICON] = ControlDetail(q);
        mControlDetails[QMENU_INDICATOR_EXCLUSIVE] = ControlDetail(q);
        mControlDetails[QMENU_INDICATOR_NONEEXCLUSIVE] = ControlDetail(q);
        mControlDetails[QMENU_RIGHTARROW] = ControlDetail(q);

        for (auto action : dMenu->actions()) {
            QObject::connect(action, &QAction::hovered, [action, this] {
                mControlDetails[QMENU_ITEM].states.remove("selected");
                mControlDetails[QMENU_ICON].states.remove("selected");
                mControlDetails[QMENU_INDICATOR_EXCLUSIVE].states.remove("selected");
                mControlDetails[QMENU_INDICATOR_NONEEXCLUSIVE].states.remove("selected");
                if (action->data().toBool()) {
                    mControlDetails[action->text()].states.insert("selected");
                }
                updateAllControlTitle();
            });
            // listen specified QAction check state
            if (action->data().toBool()) {
                QObject::connect(action, &QAction::triggered, [action, this](bool checked) {
                    if (checked) {
                        mControlDetails[action->text()].states.insert("checked");
                    } else {
                        mControlDetails[action->text()].states.remove("checked");
                    }
                    updateTitle(action->text());
                });
            }
        }
    }

    auto dTabBar = dynamic_cast<QTabBar *>(mContent);
    if (dTabBar) {
        mControlDetails[QTABBAR_TAB] = ControlDetail(q);
        mControlDetails[QTABBAR_FIRST] = ControlDetail(q);
        mControlDetails[QTABBAR_LAST] = ControlDetail(q);
        mControlDetails[QTABBAR_CLOSEBUTTON] = ControlDetail(q);
        // initial state
        mControlDetails[QTABBAR_FIRST].states.insert("selected");

//        auto closeButton = dTabBar->tabButton(2, QTabBar::RightSide);
//        closeButton->setProperty("controlName", QTABBAR_CLOSEBUTTON);
//        closeButton->installEventFilter(mEventListener);

        QObject::connect(dTabBar, &QTabBar::currentChanged, [dTabBar, this](int index) {
            mControlDetails[QTABBAR_TAB].states.remove("selected");
            mControlDetails[QTABBAR_FIRST].states.remove("selected");
            mControlDetails[QTABBAR_LAST].states.remove("selected");

            if (dTabBar->tabText(index) == QTABBAR_TAB) {
                mControlDetails[QTABBAR_TAB].states.insert("selected");
            }
            if (dTabBar->tabText(index) == QTABBAR_FIRST) {
                mControlDetails[QTABBAR_FIRST].states.insert("selected");
            }
            if (dTabBar->tabText(index) == QTABBAR_LAST) {
                mControlDetails[QTABBAR_LAST].states.insert("selected");
            }
            updateAllControlTitle();
        });


    }

    set_layout:
    for (const auto &v  : mControlDetails) {
        mLayout->addWidget(v.display, 0, Qt::AlignCenter);
    }
    QWidget *added = mContent;
    if (container) {
        added = container;
        container->setParent(q);
    } else {
        mContent->setParent(q);
    }
    if (pos == slp_south) {
        mLayout->addWidget(added, 0, Qt::AlignCenter);
    } else {
        mLayout->insertWidget(0, added, 0, Qt::AlignCenter);
    }

    updateAllControlTitle();
}

void ShowcasePrivate::onEventOccurred(QObject *watched, QEvent *event)
{
    if (!watched) {
        return;
    }
    bool changed = false;
    auto wt = widgetType(mControlName);

    if (dynamic_cast<QMenu *>(watched) && (event->type() == QEvent::Close ||
                                           event->type() == QEvent::Hide)) {
        mControlDetails[QMENU_ITEM].states.remove("selected");
        mControlDetails[QMENU_ICON].states.remove("selected");
        mControlDetails[QMENU_INDICATOR_EXCLUSIVE].states.remove("selected");
        mControlDetails[QMENU_INDICATOR_NONEEXCLUSIVE].states.remove("selected");
        changed = true;
    }
    if (watched->metaObject()->className() == QString("QComboBoxListView")) {
        if (event->type() == QEvent::FocusIn) {
            mControlDetails[mControlName].states.insert("on");
            changed = true;
        } else if (event->type() == QEvent::FocusOut) {
            mControlDetails[mControlName].states.remove("on");
            changed = true;
        }
            // doesn't worked!!!!
        else if (event->type() == QEvent::MouseButtonRelease) {
            mControlDetails[QCOMBOBOX_DROPDOWN].states.remove("pressed");
            mControlDetails[QCOMBOBOX_DOWNARROW].states.remove("pressed");
            changed = true;
        }
//        QMetaEnum metaEnum = QMetaEnum::fromType<QEvent::Type>();
//        qDebug() << metaEnum.valueToKey(event->type());
    }
    if (!watched->dynamicPropertyNames().contains("controlName")) {
        return;
    }
    QString controlName = watched->property("controlName").toString();
    if (controlName.isEmpty()) {
        return;
    }
    auto &detail = mControlDetails[controlName];

    if (controlName != "QTabBar") {
        if (event->type() == QEvent::Enter) {
            detail.states.insert("hover");
            if (controlName == "QCheckBox") {
                mControlDetails[QCHECKBOX_INDICATOR].states.insert("hover");
            }
            if (controlName == "QRadioButton") {
                mControlDetails[QRADIOBUTTON_INDICATOR].states.insert("hover");
            }
            changed = true;
        } else if (event->type() == QEvent::Leave) {
            detail.states.remove("hover");
            if (controlName == "QCheckBox") {
                mControlDetails[QCHECKBOX_INDICATOR].states.remove("hover");
            }
            if (controlName == "QRadioButton") {
                mControlDetails[QRADIOBUTTON_INDICATOR].states.remove("hover");
            }
            changed = true;
        }
    }

        if (event->type() == QEvent::FocusIn) {
            detail.states.insert("focus");
            if (controlName == "QCheckBox") {
                mControlDetails[QCHECKBOX_INDICATOR].states.insert("focus");
            }
            if (controlName == "QRadioButton") {
                mControlDetails[QRADIOBUTTON_INDICATOR].states.insert("focus");
            }
            changed = true;
        } else if (event->type() == QEvent::FocusOut) {
            detail.states.remove("focus");
            if (controlName == "QCheckBox") {
                mControlDetails[QCHECKBOX_INDICATOR].states.remove("focus");
            }
            if (controlName == "QRadioButton") {
                mControlDetails[QRADIOBUTTON_INDICATOR].states.remove("focus");
            }
            changed = true;
        }

    if (dynamic_cast<QComboBox *>(watched)) {
        auto comboBox = dynamic_cast<QComboBox *>(watched);
        if (event->type() == QEvent::MouseButtonPress &&
            dynamic_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) {
            auto mouseEvent = (QMouseEvent *) event;
            QStyleOptionComboBox opt;
            opt.initFrom(mContent);
            opt.editable = comboBox->isEditable();
            opt.subControls = QStyle::SC_All;
            auto subControl = mContent->style()
                    ->hitTestComplexControl(QStyle::CC_ComboBox, &opt,
                                            mouseEvent->pos(),
                                            mContent);
            if (subControl == QStyle::SC_ComboBoxArrow) {
                mControlDetails[QCOMBOBOX_DROPDOWN].states.insert("pressed");
                mControlDetails[QCOMBOBOX_DOWNARROW].states.insert("pressed");
            }
            changed = true;
        }
            // doesn't worked!!!!
        else if (event->type() == QEvent::MouseButtonRelease) {
            mControlDetails[QCOMBOBOX_DROPDOWN].states.remove("pressed");
            mControlDetails[QCOMBOBOX_DOWNARROW].states.remove("pressed");
            changed = true;
        }
    }
    if (dynamic_cast<QTabBar *>(watched)) {
        auto tabBar = dynamic_cast<QTabBar *>(watched);
        auto hoverEvent = dynamic_cast<QHoverEvent *>(event);
        if (hoverEvent) {
            mControlDetails[QTABBAR_TAB].states.remove("hover");
            mControlDetails[QTABBAR_FIRST].states.remove("hover");
            mControlDetails[QTABBAR_LAST].states.remove("hover");
            if (tabBar->tabRect(0).contains(hoverEvent->pos())) {
                mControlDetails[QTABBAR_FIRST].states.insert("hover");
            }
            if (tabBar->tabRect(1).contains(hoverEvent->pos())) {
                mControlDetails[QTABBAR_TAB].states.insert("hover");
            }
            if (tabBar->tabRect(tabBar->count() - 1).contains(hoverEvent->pos())) {
                mControlDetails[QTABBAR_LAST].states.insert("hover");
            }
            changed = true;
        }
    }
    if (changed) {
        updateAllControlTitle();
    }

}

Showcase::Showcase(QWidget *content, QWidget *parent, ShowcaseWidgetPosition pos) :
        QWidget(parent),
        d_ptr(new ShowcasePrivate(this, content, pos, nullptr))
{
    Q_D(Showcase);
    connect(d->mEventListener, &EventListener::eventOccurred,
            [this](QObject *watched, QEvent *event) {
                d_ptr->onEventOccurred(watched, event);
            });
}

Showcase::Showcase(QWidget *content, QWidget *container, QWidget *parent,
                   ShowcaseWidgetPosition pos) :
        d_ptr(new ShowcasePrivate(this, content, pos, container))
{
    Q_D(Showcase);
    connect(d->mEventListener, &EventListener::eventOccurred,
            [this](QObject *watched, QEvent *event) {
                d_ptr->onEventOccurred(watched, event);
            });
}

Showcase::~Showcase()
{

}

EventListener::EventListener(QObject *parent) : QObject(parent)
{

}

bool EventListener::eventFilter(QObject *watched, QEvent *event)
{
    emit eventOccurred(watched, event);
    return false;
}

#include "Showcase.moc"