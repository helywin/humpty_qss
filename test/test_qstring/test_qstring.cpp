//
// Created by jiang on 2021/4/22.
//

#include <QString>
#include <QDebug>

int main()
{
    auto str = QString(" ").repeated(6);
    qDebug() << std::all_of(str.begin(), str.end(), [](QChar c) { return c == ' '; });
    qDebug() << str.size();
    qDebug() << str.length();
}