//
// Created by jiang on 2021/4/21.
//

#ifndef HUMPTY_QSS_QSSEDITORCONFIG_HPP
#define HUMPTY_QSS_QSSEDITORCONFIG_HPP


class QssEditorConfig
{
public:
    void setTabReplace(bool replace);
    bool tabReplace() const;
    void setTabReplaceSize(int size);
    int tabReplaceSize() const;
};


#endif //HUMPTY_QSS_QSSEDITORCONFIG_HPP
