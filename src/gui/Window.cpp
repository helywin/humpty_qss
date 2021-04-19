//
// Created by helywin on 2021/4/14.
//

#include "Window.hpp"
#include <QTabWidget>
#include <QTextEdit>
#include <QBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QApplication>
#include <QSplitter>
#include <QLabel>
#include <QToolButton>
#include "Utils.hpp"
#include "Showcase.hpp"

using namespace Com;

enum WidgetType
{
    wt_widget,
    wt_frame,
    wt_label,
    wt_pushButton,
    wt_toolButton,
    wt_widgetCount,
};

QString widgetName(WidgetType type)
{
    switch (type) {
        case wt_widget:
            return "QWidget";
        case wt_frame:
            return "QFrame";
        case wt_label:
            return "QLabel";
        case wt_pushButton:
            return "QPushButton";
        case wt_toolButton:
            return "QToolButton";
        default:
            return {};
    }
}

/*
QAbstractButton
QAbstractSlider
QAbstractSpinBox
QCalendarWidget
QComboBox
QDesignerActionEditorInterface
QDesignerFormWindowInterface
QDesignerObjectInspectorInterface
QDesignerPropertyEditorInterface
QDesignerWidgetBoxInterface
QDesktopWidget
QDialog
QDialogButtonBox
QDockWidget
QFocusFrame
QFrame
QGroupBox
QKeySequenceEdit
QLineEdit
QMacCocoaViewContainer
QMacNativeWidget
QMainWindow
QMdiSubWindow
QMenu
QMenuBar
QOpenGLWidget
QProgressBar
QQuickWidget
QRubberBand
QSizeGrip
QSplashScreen
QSplitterHandle
QStatusBar
QSvgWidget
QTabBar
QTabWidget
QToolBar
QWizardPage
*/

void updateStyleSheet(QWidget *parent, const QString &qss)
{
    if (parent->property("test").toBool()) {
        parent->setStyleSheet(qss);
        parent->style()->polish(parent);
    }
    for (auto child : parent->children()) {
        auto w = dynamic_cast<QWidget *>(child);
        if (w) {
            updateStyleSheet(w, qss);
        }
    }
}

class WindowPrivate
{
public:
    Q_DECLARE_PUBLIC(Window)

    Window *q_ptr;
    QHBoxLayout *mLayout;
    QSplitter *mSplitter;
    QTabWidget *mWidgetPage;
    QWidget *mPages[wt_widgetCount];
    QWidget *mEditPage;
    QTextEdit *mEditor;
    QPushButton *mUpdate;

    explicit WindowPrivate(Window *p);
    QWidget *initPage(WidgetType type, QGridLayout *&grid);
    void addQWidgetPage();
    void addQFramePage();
    void addQLabelPage();
    void addQPushButton();
    void addQToolButton();
};

WindowPrivate::WindowPrivate(Window *p) :
        q_ptr(p)
{
    Q_Q(Window);
    initWidget(mLayout, q);
    initWidget(mSplitter, q);
    initWidget(mWidgetPage, q);
    initWidgets(mPages, mWidgetPage);
    initWidget(mEditPage, q);
    initWidget(mEditor, q);
    initWidget(mUpdate, q);
    q->setMinimumSize(800, 600);

    mWidgetPage->setTabPosition(QTabWidget::West);

    addQWidgetPage();
    addQFramePage();
    addQLabelPage();
    addQPushButton();
    addQToolButton();

    mEditor->setFontFamily("consolas");
    mEditor->setText("QWidget {\n    background:#eeeeee;\n}\n\n"
                     "QWidget:disabled {\n    background:#cccccc;\n}");
    auto vbox = new QVBoxLayout(mEditPage);
    vbox->addWidget(mEditor);
    vbox->addWidget(mUpdate);
    mLayout->addWidget(mSplitter);
    mSplitter->addWidget(mWidgetPage);
    mSplitter->addWidget(mEditPage);
}

QWidget *WindowPrivate::initPage(WidgetType type, QGridLayout *&grid)
{
    auto page = mPages[type];
    page->setWindowTitle(widgetName(type));
    auto pageLayout = new QVBoxLayout(page);
    page->setLayout(pageLayout);
    grid = new QGridLayout;
    grid->setSpacing(20);
    grid->setContentsMargins(20, 20, 20, 20);

    pageLayout->addLayout(grid);
    pageLayout->addStretch(1);
    mWidgetPage->addTab(page, page->windowTitle());
    return page;
}

void WindowPrivate::addQWidgetPage()
{
    QGridLayout *grid;
    auto page = initPage(wt_widget, grid);
    using Type = QWidget;

    auto e = new Type;
    e->setMinimumHeight(40);
    e->setWindowTitle(QString(Type::staticMetaObject.className()));
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;
    e->setMinimumHeight(40);
    e->setDisabled(true);
    e->setWindowTitle(QString(Type::staticMetaObject.className()) + ":disabled");
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);
}

void WindowPrivate::addQFramePage()
{
    QGridLayout *grid;
    auto page = initPage(wt_frame, grid);

    using Type = QFrame;
    auto e = new Type;
    e->setMinimumHeight(40);
    e->setWindowTitle(QString(Type::staticMetaObject.className()));
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;;
    e->setMinimumHeight(40);
    e->setDisabled(true);
    e->setWindowTitle(QString(Type::staticMetaObject.className()) + ":disabled");
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);
}

void WindowPrivate::addQLabelPage()
{
    const QString text = "标签/Label";
    QGridLayout *grid;
    auto page = initPage(wt_label, grid);
    using Type = QLabel;

    auto e = new Type;
    e->setMinimumHeight(40);
    e->setText(text);
    e->setWindowTitle(QString(Type::staticMetaObject.className()));
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;
    e->setMinimumHeight(40);
    e->setText(text);
    e->setDisabled(true);
    e->setWindowTitle(QString(Type::staticMetaObject.className()) + ":disabled");
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);
}

void WindowPrivate::addQPushButton()
{
    const QString text = "按钮/PushButton";
    QGridLayout *grid;
    auto page = initPage(wt_pushButton, grid);
    using Type = QPushButton;

    auto e = new Type;
    e->setMinimumHeight(40);
    e->setMinimumWidth(160);
    e->setText(text);
    e->setWindowTitle(QString(Type::staticMetaObject.className()));
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);
    QObject::connect(e, &Type::pressed, [e] {
        e->setWindowTitle(Type::staticMetaObject.className() + QString(":pressed"));
    });

    QObject::connect(e, &Type::released, [e] {
        e->setWindowTitle(Type::staticMetaObject.className());
    });

    e = new Type;
    e->setMinimumHeight(40);
    e->setMinimumWidth(160);
    e->setText(text);
    e->setDisabled(true);
    e->setWindowTitle(QString(Type::staticMetaObject.className()) + ":disabled");
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);

    e = new Type;
    e->setMinimumHeight(40);
    e->setMinimumWidth(160);
    e->setText(text);
    e->setCheckable(true);
    e->setWindowTitle(QString(Type::staticMetaObject.className()) + ":checked");
    QObject::connect(e, &Type::clicked, [e](bool checked) {
        if (checked) {
            e->setWindowTitle(Type::staticMetaObject.className() + QString(":checked"));
        } else {
            e->setWindowTitle(Type::staticMetaObject.className());
        }
    });

    grid->addWidget(new Showcase(e, page), 1, 0, Qt::AlignHCenter);
}

void WindowPrivate::addQToolButton()
{
    const QString text = "按钮/ToolButton";
    QGridLayout *grid;
    auto page = initPage(wt_toolButton, grid);
    using Type = QToolButton;

    auto e = new Type;
    e->setMinimumHeight(40);
    e->setMinimumWidth(160);
    e->setText(text);
    e->setWindowTitle(QString(Type::staticMetaObject.className()));
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);
    QObject::connect(e, &Type::pressed, [e] {
        e->setWindowTitle(Type::staticMetaObject.className() + QString(":pressed"));
    });

    QObject::connect(e, &Type::released, [e] {
        e->setWindowTitle(Type::staticMetaObject.className());
    });

    e = new Type;
    e->setMinimumHeight(40);
    e->setMinimumWidth(160);
    e->setText(text);
    e->setDisabled(true);
    e->setWindowTitle(QString(Type::staticMetaObject.className()) + ":disabled");
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);

    e = new Type;
    e->setMinimumHeight(40);
    e->setMinimumWidth(160);
    e->setText(text);
    e->setCheckable(true);
    e->setWindowTitle(QString(Type::staticMetaObject.className()) + ":checked");
    QObject::connect(e, &Type::clicked, [e](bool checked) {
        if (checked) {
            e->setWindowTitle(Type::staticMetaObject.className() + QString(":checked"));
        } else {
            e->setWindowTitle(Type::staticMetaObject.className());
        }
    });

    grid->addWidget(new Showcase(e, page), 1, 0, Qt::AlignHCenter);
}

Window::Window(QWidget *parent) :
        QWidget(parent),
        d_ptr(new WindowPrivate(this))
{
    Q_D(Window);
    Window::translate();
    connect(d->mUpdate, &QPushButton::clicked, [this] {
        QString qss = d_ptr->mEditor->toPlainText();
        updateStyleSheet(this, qss);
    });
}

void Window::translate()
{
    Q_D(Window);
    d->mUpdate->setText(tr("Update"));
}

Window::~Window()
{}

