//
// Created by jiang on 2021/4/18.
//

#ifndef HUMPTY_QSS_SHOWCASE_HPP
#define HUMPTY_QSS_SHOWCASE_HPP

#include <QWidget>

class ShowcasePrivate;

class Showcase : public QWidget
{
    Q_OBJECT
public:
    explicit Showcase(QWidget *content, QWidget *parent = nullptr);
    ~Showcase() override;
    void setWidget(QWidget *w);

private:
    Q_DECLARE_PRIVATE(Showcase)
    Q_DISABLE_COPY(Showcase)
    QScopedPointer <ShowcasePrivate> d_ptr;
};


#endif //HUMPTY_QSS_SHOWCASE_HPP
