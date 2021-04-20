//
// Created by jiang on 2021/4/20.
//

#include "QssEditorPlugin.hpp"

QssEditorPlugin::QssEditorPlugin(QObject *parent) : QObject(parent)
{

}

bool QssEditorPlugin::isContainer() const
{
    return false;
}

bool QssEditorPlugin::isInitialized() const
{
    return QDesignerCustomWidgetInterface::isInitialized();
}

QIcon QssEditorPlugin::icon() const
{
    return QIcon();
}

QString QssEditorPlugin::domXml() const
{
    return QDesignerCustomWidgetInterface::domXml();
}

QString QssEditorPlugin::group() const
{
    return QString();
}

QString QssEditorPlugin::includeFile() const
{
    return QString();
}

QString QssEditorPlugin::name() const
{
    return QString();
}

QString QssEditorPlugin::toolTip() const
{
    return QString();
}

QString QssEditorPlugin::whatsThis() const
{
    return QString();
}

QWidget *QssEditorPlugin::createWidget(QWidget *parent)
{
    return nullptr;
}

void QssEditorPlugin::initialize(QDesignerFormEditorInterface *core)
{
    QDesignerCustomWidgetInterface::initialize(core);
}
