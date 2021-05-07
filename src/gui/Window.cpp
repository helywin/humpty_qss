//
// Created by helywin on 2021/4/14.
//

#include "Window.hpp"
#include <QtWidgets>
#include <QSizePolicy>
#include <cassert>

#include "QssEditor.hpp"
#include "Utils.hpp"
#include "Showcase.hpp"
#include "QssEditorConfig.hpp"
#include "GuiCom.hpp"
#include "WidgetContainer.hpp"
#include "ButtonContainer.hpp"
#include "LineEditContainer.hpp"
#include "ComboBoxContainer.hpp"
#include "CheckContainer.hpp"
#include "MenuContainer.hpp"
#include "TabBarContainer.hpp"

QList<QWidget *> gNoParentWidgets;

using namespace Com;


/*
 QCheckBox
 QComboBox
QCommandLinkButton
QDateEdit
QDateTimeEdit
QDial
QDoubleSpinBox
QFocusFrame
QFontComboBox
QLCDNumber
 QLabel
 QLineEdit
 QMenu
QProgressBar
 QPushButton
 QRadioButton
QScrollArea
QScrollBar
QSizeGrip
QSlider
QSpinBox
QTabBar
QTabWidget
QTimeEdit
QToolBox
 QToolButton
 QWidget

*/

class GridLayoutHelper
{
public:
    explicit GridLayoutHelper(QGridLayout *layout, int columnCount = 1) :
            mLayout(layout),
            mColumnCount(columnCount)
    {
        assert(mColumnCount > 0);
        assert(mLayout);
    }

    void addWidget(QWidget *widget)
    {
        mLayout->addWidget(widget, mCurrentRow, mCurrentColumn, Qt::AlignCenter);
        mCurrentColumn += 1;
        if (mCurrentColumn == mColumnCount) {
            mCurrentRow += 1;
            mCurrentColumn = 0;
        }
    }

private:
    QGridLayout *mLayout;
    const int mColumnCount;
    int mCurrentRow = 0;
    int mCurrentColumn = 0;
};

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
    QWidget *mWidgetPage;
    QHBoxLayout *mWidgetPageLayout;
    QButtonGroup *mIndexButtonGroup;
    QToolButton *mIndexButtons[wt_widgetCount];
    QStackedWidget *mStackedWidget;
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
    void addQCheckBoxPage();
    void addQRadioButtonPage();
    void addQMenuPage();
    void addQTabBarPage();
    void addQTabWidgetPage();
};

WindowPrivate::WindowPrivate(Window *p) :
        q_ptr(p)
{
    Q_Q(Window);
    initWidget(mLayout, q);
    initWidget(mSplitter, q);
    initWidget(mWidgetPage, q);
    initWidget(mWidgetPageLayout, mWidgetPage);
    initWidget(mIndexButtonGroup, mWidgetPage);
    initWidgets(mIndexButtons, mWidgetPage);
    initWidget(mStackedWidget, mWidgetPage);
    initWidgets(mPages, mStackedWidget);
    initWidget(mEditPage, q);
    initWidget(mEditor, q);
    initWidget(mUpdate, q);
    q->setMinimumSize(1000, 600);

    mStackedWidget->setMinimumWidth(500);
    auto vbox = new QVBoxLayout;
    for (int i = 0; i < wt_widgetCount; ++i) {
        mIndexButtons[i]->setText(widgetName((WidgetType) i));
        mIndexButtons[i]->setCheckable(true);
        mIndexButtons[i]->setFixedWidth(150);
        vbox->addWidget(mIndexButtons[i], 0);
        mIndexButtonGroup->addButton(mIndexButtons[i], i);
        mStackedWidget->addWidget(mPages[i]);
    }
    mIndexButtons[0]->setChecked(true);
    mIndexButtonGroup->setExclusive(true);
    vbox->addStretch(1);
    mWidgetPageLayout->addLayout(vbox, 0);
    mWidgetPageLayout->addWidget(mStackedWidget, 1);

    addQWidgetPage();
    addQFramePage();
    addQLabelPage();
    addQPushButton();
    addQToolButton();
    addQLineEditPage();
    addQComboBoxPage();
    addQCheckBoxPage();
    addQRadioButtonPage();
    addQMenuPage();
    addQTabBarPage();
    addQTabWidgetPage();

    QssEditorConfig config;
    config.setTabReplace(true);
    config.setAutoIndentation(true);
    config.setAutoParentheses(true);
//    mEditor->setMinimumWidth(320);
    mEditor->setEditorConfig(config);
    mEditor->setFontFamily("consolas");
    mEditor->setText("QWidget {\n    background:#eeeeee;\n}\n\n"
                     "QWidget:disabled {\n    background:#cccccc;\n}\n"
                     ".QLabel {\n"
                     "    qproperty-indent: 10;\n"
                     "}");
    mEditPage->setSizePolicy(QSizePolicy::Expanding,
                             QSizePolicy::Preferred);
    vbox = new QVBoxLayout(mEditPage);
    vbox->addWidget(mEditor);
    vbox->addWidget(mUpdate);
    mLayout->addWidget(mSplitter);
    mSplitter->addWidget(mWidgetPage);
    mSplitter->addWidget(mEditPage);
    mSplitter->setStretchFactor(0, 1);
    mSplitter->setStretchFactor(1, 2);
    mSplitter->setCollapsible(0, false);
    mSplitter->setCollapsible(1, false);
}

QWidget *WindowPrivate::initPage(WidgetType type, QGridLayout *&grid)
{
    auto page = mPages[type];
    assert(page->windowTitle().isEmpty());
    page->setWindowTitle(widgetName(type));
    auto pageLayout = new QVBoxLayout(page);
    page->setLayout(pageLayout);
    grid = new QGridLayout;
    grid->setSpacing(20);
    grid->setContentsMargins(20, 20, 20, 20);

    pageLayout->addLayout(grid);
    pageLayout->addStretch(1);
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
    GridLayoutHelper helper(grid);
    using Type = QWidget;

    auto e = new Type;
    auto c = new WidgetContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);

    e = new Type;
    c = new WidgetContainer(page);
    e->setDisabled(true);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);
}

void WindowPrivate::addQFramePage()
{
    QGridLayout *grid;
    auto page = initPage(wt_frame, grid);
    GridLayoutHelper helper(grid);
    using Type = QFrame;

    auto e = new Type;
    auto c = new WidgetContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);

    e = new Type;
    c = new WidgetContainer(page);
    e->setDisabled(true);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);
}

void WindowPrivate::addQLabelPage()
{
    const QString text = "标签/Label";
    QGridLayout *grid;
    auto page = initPage(wt_label, grid);
    GridLayoutHelper helper(grid);
    using Type = QLabel;

    auto e = new Type;
    e->setText(text);
    auto c = new WidgetContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);

    e = new Type;
    e->setText(text);
    c = new WidgetContainer(page);
    e->setDisabled(true);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);
}

void WindowPrivate::addQPushButton()
{
    const QString text = "按钮/PushButton";
    QGridLayout *grid;
    auto page = initPage(wt_pushButton, grid);
    GridLayoutHelper helper(grid);
    using Type = QPushButton;

    auto e = new Type;
    e->setText(text);
    auto c = new ButtonContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);

    e = new Type;
    e->setText(text);
    c = new ButtonContainer(page);
    e->setDisabled(true);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);

    e = new Type;
    e->setText(text);
    e->setCheckable(true);
    c = new ButtonContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);
}

void WindowPrivate::addQToolButton()
{
    const QString text = "按钮/ToolButton";
    QGridLayout *grid;
    auto page = initPage(wt_toolButton, grid);
    GridLayoutHelper helper(grid);
    using Type = QToolButton;

    auto e = new Type;
    e->setText(text);
    auto c = new ButtonContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);

    e = new Type;
    e->setText(text);
    c = new ButtonContainer(page);
    e->setDisabled(true);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);

    e = new Type;
    e->setText(text);
    e->setCheckable(true);
    c = new ButtonContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    setSize(e);
    helper.addWidget(c);
}

void WindowPrivate::addQLineEditPage()
{
    const QString text = "行编辑/LineEdit";
    QGridLayout *grid;
    auto page = initPage(wt_lineEdit, grid);
    GridLayoutHelper helper(grid);
    using Type = QLineEdit;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    auto c = new LineEditContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    c = new LineEditContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setReadOnly(true);
    c = new LineEditContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setEchoMode(QLineEdit::Password);
    c = new LineEditContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    setSize(e);
    e->setText(text + "+ clear");
    e->setClearButtonEnabled(true);
    c = new LineEditContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);
}

/* *********************************************************************************
 * NOTE
 * QComboBoxPrivateContainer is lazy load, we should use setEditable(true) to let
 * it to be initialized, so that we can get QComboBoxListView object
 * *********************************************************************************/

void WindowPrivate::addQComboBoxPage()
{
    const QStringList textList = {"红楼梦", "三国演义", "西游记", "水浒传"};
    QGridLayout *grid;
    auto page = initPage(wt_comboBox, grid);
    GridLayoutHelper helper(grid);
    using Type = QComboBox;

    auto e = new Type;
    setSize(e);
    e->setEditable(true);
    e->addItems(textList);
    auto c = new ComboBoxContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    setSize(e);
    e->setDisabled(true);
    e->addItems(textList);
    c = new ComboBoxContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    setSize(e);
    e->addItems(textList);
    c = new ComboBoxContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);
}

void WindowPrivate::addQCheckBoxPage()
{
    const QString text = "复选/CheckBox";
    QGridLayout *grid;
    auto page = initPage(wt_checkBox, grid);
    GridLayoutHelper helper(grid);
    using Type = QCheckBox;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    auto c = new CheckContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    e->setEnabled(false);
    setSize(e);
    e->setText(text);
    c = new CheckContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    e->setTristate(true);
    setSize(e);
    e->setText(text);
    c = new CheckContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);
}

void WindowPrivate::addQRadioButtonPage()
{
    const QString text = "单选/RadioButton";
    QGridLayout *grid;
    auto page = initPage(wt_radioButton, grid);
    GridLayoutHelper helper(grid);
    using Type = QRadioButton;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    auto c = new CheckContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);

    e = new Type;
    e->setEnabled(false);
    setSize(e);
    e->setText(text);
    c = new CheckContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);
}

void WindowPrivate::addQMenuPage()
{
    Q_Q(Window);
    const QString text = "菜单/Menu";
    QGridLayout *grid;
    auto page = initPage(wt_menu, grid);
    GridLayoutHelper helper(grid);
    using Type = QMenu;

    auto e = new Type;
    e->setTitle(text);
    auto menu = new QMenu("expand", e);
    menu->addAction(qApp->style()->standardIcon(QStyle::StandardPixmap::SP_ComputerIcon),
                    "computer");
    e->addMenu(menu);
    e->addSection(qApp->style()->standardIcon(QStyle::StandardPixmap::SP_DriveDVDIcon),
                  "-- exclusive --");
    auto ag = new QActionGroup(q);
    QAction *a;
    ag->setExclusive(true);
    ag->addAction((a = new QAction("none", e),
            a->setCheckable(true),
            a->setChecked(true),
            a));
    ag->addAction((a = new QAction("dir", e),
            a->setIcon(qApp->style()->standardIcon(QStyle::StandardPixmap::SP_DirIcon)),
            a->setCheckable(true),
            a));
    ag->addAction((a = new QAction("QMenu::indicator:exclusive", e),
            a->setCheckable(true),
            a->setData(true),
            a));
    e->addActions(ag->actions());
    e->addSection("-- non-exclusive --");
    ag = new QActionGroup(q);
    ag->setExclusive(false);
    ag->addAction((a = new QAction("none", e),
            a->setCheckable(true),
            a->setChecked(true),
            a));
    ag->addAction((a = new QAction("trash", e),
            a->setIcon(qApp->style()->standardIcon(QStyle::StandardPixmap::SP_TrashIcon)),
            a->setCheckable(true),
            a->setChecked(true),
            a));
    ag->addAction((a = new QAction("QMenu::indicator:non-exclusive", e),
            a->setCheckable(true),
            a->setData(true),
            a));
    e->addActions(ag->actions());
    e->addSection("-- item --");
    e->addAction((a = new QAction("QMenu::item", e),
            a->setCheckable(true),
            a->setData(true),
            a));
    e->addAction((a = new QAction("QMenu::icon", e),
            a->setCheckable(true),
            a->setData(true),
            a->setIcon(qApp->style()->standardIcon(QStyle::StandardPixmap::SP_FileIcon)),
            a));
    e->addAction((a = new QAction("QMenu:item:disabled", e),
            a->setDisabled(true),
            a));
    auto button = new QPushButton("click to show menu");
    QObject::connect(button, &QPushButton::clicked, [e] {
        e->exec(QCursor::pos());
    });
    gNoParentWidgets.append(e);
    auto c = new MenuContainer(page);
    c->setWidget(button, Container::wp_south);
    c->setListenWidget(e);
    helper.addWidget(c);
}

void WindowPrivate::addQTabBarPage()
{
    QGridLayout *grid;
    auto page = initPage(wt_tabBar, grid);
    GridLayoutHelper helper(grid);
    using Type = QTabBar;

    auto e = new Type;
    e->setMinimumWidth(500);
    e->addTab("QTabBar::tab:first");
    e->addTab("QTabBar::tab");
    e->addTab("QTabBar::close-button");
    e->addTab("QTabBar::tab:disabled");
    e->addTab(qApp->style()->standardIcon(QStyle::StandardPixmap::SP_FileIcon),
              "icon");
    e->addTab("QTabBar::tab:last");
    e->setTabsClosable(true);
    e->setTabButton(0, QTabBar::RightSide, nullptr);
    e->setTabButton(1, QTabBar::RightSide, nullptr);
//    e->setTabButton(2, QTabBar::RightSide, nullptr);
    e->setTabButton(3, QTabBar::RightSide, nullptr);
    e->setTabButton(4, QTabBar::RightSide, nullptr);
    e->setTabButton(5, QTabBar::RightSide, nullptr);
    e->setTabEnabled(3, false);
    auto c = new TabBarContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);


    e = new Type;
    e->setMinimumWidth(100);
    e->addTab("QTabBar::tab:only-one");
    c = new TabBarContainer(page);
    c->setWidget(e);
    c->setListenWidget(e);
    helper.addWidget(c);
}

void WindowPrivate::addQTabWidgetPage()
{
    QGridLayout *grid;
    auto page = initPage(wt_tabWidget, grid);
    GridLayoutHelper helper(grid);
    using Type = QTabWidget;

    auto e = new Type;
    e->setMinimumWidth(400);
    QWidget *w;
    e->addTab((w = new QWidget(e),
                      w->setMinimumHeight(50),
                      w),
              "tab");
    e->addTab(new QWidget(e),
              "tab disabled");
    e->addTab(new QWidget(e),
              qApp->style()->standardIcon(QStyle::StandardPixmap::SP_FileIcon),
              "tab_icon");
    e->setTabEnabled(1, false);
    helper.addWidget(new Showcase(e, page));
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
        for (auto w : gNoParentWidgets) {
            updateStyleSheet(w, qss);
        }
    });
    connect(d->mIndexButtonGroup,
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
            &QButtonGroup::idClicked,
#else
            static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
#endif
            d->mStackedWidget,
            &QStackedWidget::setCurrentIndex
    );
}

void Window::translate()
{
    Q_D(Window);
    d->mUpdate->setText(tr("Update"));
}

Window::~Window()
{
    for (auto w : gNoParentWidgets) {
        delete w;
    }
}

