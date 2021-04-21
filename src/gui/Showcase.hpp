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
    explicit Showcase(QWidget *content, QWidget *parent = nullptr,
                      ShowcaseWidgetPosition pos = slp_north);
    ~Showcase() override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(Showcase)
    Q_DISABLE_COPY(Showcase)
    QScopedPointer<ShowcasePrivate> d_ptr;
};


#endif //HUMPTY_QSS_SHOWCASE_HPP
