//
// Created by jiang on 2021/4/18.
//

#ifndef HUMPTY_QSS_TRANSLATABLE_HPP
#define HUMPTY_QSS_TRANSLATABLE_HPP

#include "Translatable_Export.hpp"

namespace Com
{

/*!
 * @brief 翻译类
 * 只需要调用reTranslate即可让所有注册的类进行翻译, 实现软件运行中动态更改语言
 */
class COM_TRANSLATABLE_API Translatable
{
public:
    Translatable();
    virtual ~Translatable();
    Translatable(const Translatable &) = delete;
    Translatable &operator=(const Translatable &) = delete;
    static void reTranslate();

protected:
    virtual void translate();
};
}

#endif //HUMPTY_QSS_TRANSLATABLE_HPP
