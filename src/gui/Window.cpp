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
    QVBoxLayout *mLayout;
    QTabWidget *mWidgetGroup;
    QWidget *mPages[wt_widgetCount];
    QTextEdit *mEditor;
    QPushButton *mUpdate;

    explicit WindowPrivate(Window *p);
};

WindowPrivate::WindowPrivate(Window *p) :
        q_ptr(p)
{
    Q_Q(Window);
    initWidget(mLayout, q);
    initWidget(mWidgetGroup, q);
    initWidgets(mPages, mWidgetGroup);
    initWidget(mEditor, q);
    initWidget(mUpdate, q);

    auto page = mPages[wt_widget];
    page->setWindowTitle("QWidget");
    auto grid = new QGridLayout(page);
    grid->setSpacing(20);
    grid->setContentsMargins(20, 20, 20, 20);

    auto e = new QWidget;
    e->setWindowTitle("QWidget");
    auto s = new Showcase(e, page);
    e->setProperty("test", true);
    grid->addWidget(s, 0, 0);

    e = new QWidget;
    e->setDisabled(true);
    e->setWindowTitle("QWidget:disabled");
    s = new Showcase(e, page);
    e->setProperty("test", true);
    grid->addWidget(s, 0, 1);


    mWidgetGroup->addTab(page, page->windowTitle());

    q->setMinimumSize(800, 600);
    auto hbox = new QHBoxLayout;
    hbox->addWidget(mEditor);
    hbox->addWidget(mUpdate);
    mLayout->addWidget(mWidgetGroup);
    mLayout->addLayout(hbox);
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

