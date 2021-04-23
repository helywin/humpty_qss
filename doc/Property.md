https://programming.vip/docs/set-qt-widget-property-in-qss.html

# Set Qt Widget property in QSS

- [Set Qt Widget property in QSS](https://programming.vip/docs/set-qt-widget-property-in-qss.html#在-qss-中设置-qt-widget-属性)
- [Default style](https://programming.vip/docs/set-qt-widget-property-in-qss.html#默认样式)
- QSS custom attribute corresponds to Qt type
  - [Use enumeration](https://programming.vip/docs/set-qt-widget-property-in-qss.html#使用枚举)
- Using the QSS property selector
  - [Code instance](https://programming.vip/docs/set-qt-widget-property-in-qss.html#代码实例)

# Set Qt Widget property in QSS

```c++
Q_OBJECT

// Add custom attributes to the Qt dynamic attribute system (usually written under q'u object)
Q_PROPERTY(QColor colSelectionColor READ colSelectionColor WRITE SetcolSelectionColor)

// Set in QSS
qproperty-colSelectionColor : rgba(247, 106, 91, 64);
```

# Default style

```c++
QStyleOption opt;
opt.init(this);
QPainter p(this);
style()->drawPrimitive(QStyle::PE_PanelButtonTool, &opt, &p, this);
```

Add the following code to make the toolButton support: hover: pressed state

```c++
QStylePainter p(this);
/* Make the component support: hover: pressed attribute */
QStyleOptionToolButton opt;
opt.init(this);
if (isDown())
    opt.state |= QStyle::State_Sunken;
if (isChecked())
    opt.state |= QStyle::State_On;
if (autoRaise())
    opt.state |= QStyle::State_AutoRaise;
if (!isChecked() && !isDown())
    opt.state |= QStyle::State_Raised;
p.drawComplexControl(QStyle::CC_ToolButton, opt);
```

# QSS custom attribute corresponds to Qt type

```c++
myWgtClass {
    qproperty-num: 1;        // Q_PROPERTY(int num ...)
    qproperty-str: "test";    // Q_PROPERTY(QString str ...)
    qproperty-mySize: 10px 15px;    // Q_PROPERTY(QSize mySize ...)
    qproperty-myColor: white;    //Q_PROPERTY(QColor myClolr ...)
    qproperty-myBool: true;        //Q_PROPERTY(bool myBool ...)
    qproperty-pixmap: url(:/test/);    //Q_PROPERTY(QPixmap pixmap ...)
}
```

- Remarks: 
  - Try not to use QSize. When the stylesheet value is decimal, the corresponding value of QSize in the program is (0, 0).  (try to divide w, h into two attributes)

## Use enumeration

- Define enumeration

```c++
class myWgtClass {

Q_OBJECT

Q_ENUMS(MyEnum)
Q_PROPERTY(MyEnum type READ type WRITE setType)

public:
    enum MyEnum {
        E_A, E_B
    };

    MyEnum type(void) const { return m_type; }
    void setType(MyEnum otherType) { m_type = otherType; }
private:
    MyEnum m_type;
};
```

- Settings in CSS

```c++
myWgtClass {
    qproperty-myEnum: E_A;        //Enumeration type must be used, not its value
}
```

# Using the QSS property selector

- Note: when an attribute value changes, the referenced style does not update automatically. Instead, the update must be triggered manually  for it to take effect. 
  - unpolish(): used to clean up previous styles
  - polish(): used to add new styles

## Code instance

Application example: maximize / restore button of window

- C++ logic

```c++
void Init(void)
{
    SetMaxOrRestoreBtnStatus(true);
}
void MainWindow::on_maxOrRestoreBtn_clicked()
{
    if (isMaximized()) {
        showNormal();
        SetMaxOrRestoreBtnStatus(true);
    } else {
        showMaximized();
        SetMaxOrRestoreBtnStatus(false);
    }

//There are two ways to manually update a style

#if 0     // Method 1

    ui->maxOrRestoreBtn->style()->unpolish(ui->maxOrRestoreBtn);
    ui->maxOrRestoreBtn->style()->polish(ui->maxOrRestoreBtn);
    update();

#else    // Method 2

    ui->maxOrRestoreBtn->setStyle(QApplication::style());

#endif

}

void MainWindow::SetMaxOrRestoreBtnStatus(bool isMaxBtnStatus)
{
    if (isMaxBtnStatus) {   //Maximize button
        ui->maxOrRestoreBtn->setToolTip(tr("Maximize"));
        ui->maxOrRestoreBtn->setProperty("maxOrRestore", "maximize");
    } else {
        ui->maxOrRestoreBtn->setToolTip(tr("Restore"));
        ui->maxOrRestoreBtn->setProperty("maxOrRestore", "restore");
    }
}
```

- QSS style sheet

```css
#maxOrRestoreBtn[maxOrRestore="maximize"] {
    background: red;
}

#maxOrRestoreBtn[maxOrRestore="maximize"]:hover {
    background: yellow;
}

#maxOrRestoreBtn[maxOrRestore="restore"] {
    background: blue;
}
```

