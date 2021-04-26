//
// Created by jiang on 2021/4/18.
//

#ifndef HUMPTY_QSS_SHOWCASE_HPP
#define HUMPTY_QSS_SHOWCASE_HPP

#include <QWidget>

class ShowcasePrivate;

enum ShowcaseWidgetPosition
{
    slp_north,
    slp_south,
};

class Showcase : public QWidget
{
Q_OBJECT
public:
    explicit Showcase(QWidget *content, QWidget *parent,
                      ShowcaseWidgetPosition pos = slp_north);
    explicit Showcase(QWidget *content, QWidget *container, QWidget *parent,
                      ShowcaseWidgetPosition pos = slp_north);
    ~Showcase() override;

private:
    Q_DECLARE_PRIVATE(Showcase)
    Q_DISABLE_COPY(Showcase)
    QScopedPointer<ShowcasePrivate> d_ptr;
};

class EventListener : public QObject
{
Q_OBJECT
public:
    explicit EventListener(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void eventOccurred(QObject *watched, QEvent *event);
};


#endif //HUMPTY_QSS_SHOWCASE_HPP
