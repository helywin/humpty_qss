//
// Created by helywin on 2021/4/14.
//

#include "Window.hpp"
#include <QtWidgets>
#include <QMouseEvent>

#include "QssEditor.hpp"
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
    wt_lineEdit,
    wt_comboBox,
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
        case wt_lineEdit:
            return "QLineEdit";
        case wt_comboBox:
            return "QComboBox";
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
QGroupBox
QKeySequenceEdit
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
    QssEditor *mEditor;
    QPushButton *mUpdate;

    explicit WindowPrivate(Window *p);
    QWidget *initPage(WidgetType type, QGridLayout *&grid);
    void addQWidgetPage();
    void addQFramePage();
    void addQLabelPage();
    void addQPushButton();
    void addQToolButton();
    void addQLineEditPage();
    void addQComboBoxPage();
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
    addQLineEditPage();
    addQComboBoxPage();

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

void setSize(QWidget *widget)
{
    widget->setFixedSize(160, 40);
}

void WindowPrivate::addQWidgetPage()
{
    QGridLayout *grid;
    auto page = initPage(wt_widget, grid);
    using Type = QWidget;

    auto e = new Type;
    setSize(e);
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setDisabled(true);
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);
}

void WindowPrivate::addQFramePage()
{
    QGridLayout *grid;
    auto page = initPage(wt_frame, grid);

    using Type = QFrame;
    auto e = new Type;
    setSize(e);
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;;
    setSize(e);
    e->setDisabled(true);
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);
}

void WindowPrivate::addQLabelPage()
{
    const QString text = "标签/Label";
    QGridLayout *grid;
    auto page = initPage(wt_label, grid);
    using Type = QLabel;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);
}

void WindowPrivate::addQPushButton()
{
    const QString text = "按钮/PushButton";
    QGridLayout *grid;
    auto page = initPage(wt_pushButton, grid);
    using Type = QPushButton;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setCheckable(true);
    grid->addWidget(new Showcase(e, page), 1, 0, Qt::AlignHCenter);
}

void WindowPrivate::addQToolButton()
{
    const QString text = "按钮/ToolButton";
    QGridLayout *grid;
    auto page = initPage(wt_toolButton, grid);
    using Type = QToolButton;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setCheckable(true);
    grid->addWidget(new Showcase(e, page), 1, 0, Qt::AlignHCenter);
}

void WindowPrivate::addQLineEditPage()
{
    const QString text = "行编辑/LineEdit";
    QGridLayout *grid;
    auto page = initPage(wt_lineEdit, grid);
    using Type = QLineEdit;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    grid->addWidget(new Showcase(e, page), 0, 0, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    grid->addWidget(new Showcase(e, page), 0, 1, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setReadOnly(true);
    grid->addWidget(new Showcase(e, page), 1, 0, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setEchoMode(QLineEdit::Password);
    grid->addWidget(new Showcase(e, page), 1, 1, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->setText(text + "+ clear");
    e->setClearButtonEnabled(true);
    grid->addWidget(new Showcase(e, page), 2, 0, Qt::AlignHCenter);
}

void WindowPrivate::addQComboBoxPage()
{
    const QStringList textList = {"红楼梦", "三国演义", "西游记", "水浒传"};
    QGridLayout *grid;
    auto page = initPage(wt_comboBox, grid);
    using Type = QComboBox;

    auto e = new Type;
    setSize(e);
    e->setEditable(true);
    e->addItems(textList);
    grid->addWidget(new Showcase(e, page, slp_south), 0, 0, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->addItems(textList);
    e->setDisabled(true);
    grid->addWidget(new Showcase(e, page, slp_south), 0, 1, Qt::AlignHCenter);

    e = new Type;
    setSize(e);
    e->addItems(textList);
    grid->addWidget(new Showcase(e, page, slp_south), 1, 0, Qt::AlignHCenter);
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

