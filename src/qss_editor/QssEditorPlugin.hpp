//
// Created by jiang on 2021/4/20.
//

#ifndef HUMPTY_QSS_QSSEDITORPLUGIN_HPP
#define HUMPTY_QSS_QSSEDITORPLUGIN_HPP

#include <QDesignerCustomWidgetInterface>
#include "QssEditor_Export.hpp"

class QssEditorPlugin : public QObject, public QDesignerCustomWidgetInterface
{
Q_OBJECT
    Q_PLUGIN_METADATA(IID "Com.QssEditor")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit QssEditorPlugin(QObject *parent = nullptr);

    bool isContainer() const override;
    bool isInitialized() const override;
    QIcon icon() const override;
    QString domXml() const override;
    QString group() const override;
    QString includeFile() const override;
    QString name() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QWidget *createWidget(QWidget *parent) override;
    void initialize(QDesignerFormEditorInterface *core) override;

private:
    bool initialized = false;
};


#endif //HUMPTY_QSS_QSSEDITORPLUGIN_HPP
