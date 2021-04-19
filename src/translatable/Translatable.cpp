//
// Created by jiang on 2021/4/18.
//

#include "Translatable.hpp"
#include <list>

namespace Com
{

//! \brief 翻译类存放数组
std::list<Translatable *> gPointers;

/*!
 * @brief 构造
 */
Translatable::Translatable()
{
    gPointers.push_back(this);
}

/*!
 * @brief 析构
 */
Translatable::~Translatable()
{
    gPointers.remove(this);
}

/*!
 * @brief 翻译
 */
void Translatable::translate()
{}

/*!
 * @brief 重新翻译
 */
void Translatable::reTranslate()
{
    for (auto p : gPointers) {
        p->translate();
    }
}

}