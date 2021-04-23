//
// Created by helywin on 2021/4/14.
//

#include "Window.hpp"
#include <QtWidgets>

#include "QssEditor.hpp"
#include "Utils.hpp"
#include "Showcase.hpp"
#include "QssEditorConfig.hpp"
#include "GuiCom.hpp"

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

    mStackedWidget->setMinimumWidth(600);
    auto vbox = new QVBoxLayout;
    for (int i = 0; i < wt_widgetCount; ++i) {
        mIndexButtons[i]->setText(widgetName((WidgetType) i));
        mIndexButtons[i]->setCheckable(true);
        vbox->addWidget(mIndexButtons[i]);
        mIndexButtonGroup->addButton(mIndexButtons[i], i);
        mStackedWidget->addWidget(mPages[i]);
    }
    mIndexButtons[0]->setChecked(true);
    mIndexButtonGroup->setExclusive(true);
    vbox->addStretch(1);
    mWidgetPageLayout->addLayout(vbox);
    mWidgetPageLayout->addWidget(mStackedWidget);

    addQWidgetPage();
    addQFramePage();
    addQLabelPage();
    addQPushButton();
    addQToolButton();
    addQLineEditPage();
    addQComboBoxPage();
    addQCheckBoxPage();
    addQRadioButtonPage();

    QssEditorConfig config;
    config.setTabReplace(true);
    config.setAutoIndentation(true);
    mEditor->setEditorConfig(config);
    mEditor->setFontFamily("consolas");
    mEditor->setText("QWidget {\n    background:#eeeeee;\n}\n\n"
                     "QWidget:disabled {\n    background:#cccccc;\n}\n"
                     ".QLabel {\n"
                     "    qproperty-indent: 10;\n"
                     "}");

    vbox = new QVBoxLayout(mEditPage);
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
    setSize(e);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page));
}

void WindowPrivate::addQFramePage()
{
    QGridLayout *grid;
    auto page = initPage(wt_frame, grid);
    GridLayoutHelper helper(grid);
    using Type = QFrame;

    auto e = new Type;
    setSize(e);
    helper.addWidget(new Showcase(e, page));

    e = new Type;;
    setSize(e);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page));
}

void WindowPrivate::addQLabelPage()
{
    const QString text = "标签/Label";
    QGridLayout *grid;
    auto page = initPage(wt_label, grid);
    GridLayoutHelper helper(grid);
    using Type = QLabel;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page));
}

void WindowPrivate::addQPushButton()
{
    const QString text = "按钮/PushButton";
    QGridLayout *grid;
    auto page = initPage(wt_pushButton, grid);
    GridLayoutHelper helper(grid);
    using Type = QPushButton;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setCheckable(true);
    helper.addWidget(new Showcase(e, page));
}

void WindowPrivate::addQToolButton()
{
    const QString text = "按钮/ToolButton";
    QGridLayout *grid;
    auto page = initPage(wt_toolButton, grid);
    GridLayoutHelper helper(grid);
    using Type = QToolButton;

    auto e = new Type;
    setSize(e);
    e->setText(text);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setCheckable(true);
    helper.addWidget(new Showcase(e, page));
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
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setReadOnly(true);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setEchoMode(QLineEdit::Password);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text + "+ clear");
    e->setClearButtonEnabled(true);
    helper.addWidget(new Showcase(e, page));
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
    helper.addWidget(new Showcase(e, page, slp_south));

    e = new Type;
    setSize(e);
    e->addItems(textList);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page, slp_south));

    e = new Type;
    setSize(e);
    e->addItems(textList);
    helper.addWidget(new Showcase(e, page, slp_south));
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
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setTristate(true);
    helper.addWidget(new Showcase(e, page));
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
    helper.addWidget(new Showcase(e, page));

    e = new Type;
    setSize(e);
    e->setText(text);
    e->setDisabled(true);
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
    });
    connect(d->mIndexButtonGroup,
            &QButtonGroup::idClicked,
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
{}

