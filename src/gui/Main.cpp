//
// Created by jiang on 2021/4/8.
//
//
//                            _ooOoo_
//                           o8888888o
//                           88" . "88
//                           (| -_- |)
//                            O\ = /O
//                        ____/`---'\____
//                      .   ' \\| |// `.
//                       / \\||| : |||// \
//                     / _||||| -:- |||||- \
//                       | | \\\ - /// | |
//                     | \_| ''\---/'' | |
//                      \ .-\__ `-` ___/-. /
//                   ___`. .' /--.--\ `. . __
//                ."" '< `.___\_<|>_/___.' >'"".
//               | | : `- \`.;`\ _ /`;.`/ - ` : | |
//                 \ \ `-. \_ __\ /__ _/ .-` / /
//         ======`-.____`-.___\_____/___.-`____.-'======
//                            `=---='
//
//         .............................................
//                  佛祖保佑             永无BUG

#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QFontDatabase>
#include <QWidget>
#include <QFile>
#include <QDebug>
#ifdef _WINDOWS
#include "CrashHandler.hpp"
#endif
#include "Logger.hpp"
#include "version_info.h"
#include "Window.hpp"

using namespace Com;

int main(int argc, char *argv[])
{
#ifdef DEBUG
//    SetDllDirectory(TEXT("D:\\Qt\\6.0.3\\msvc2019_64\\bin"));
#endif
#ifdef _WINDOWS
    auto handler = CrashHandler::initHandler();
#endif
    Logger::installMessageHandler();
    Logger::setAppName(APP_NAME);
    Logger::setAppVersion(VERSION);
    Logger::setLogContent(Com::Logger::lc_thread, false);
    Logger::setLogContent(Com::Logger::lc_func, false);
    Logger::startRecord(true);
#ifdef _WINDOWS
    handler->setApplicationName(APP_NAME);
    handler->setApplicationVersion(VERSION);
    handler->setMiniDumpDir("crash");
#endif
#ifdef NO_CRASH_DIALOG
    handler->setExceptionFilterCallback(nullptr);
#endif
    if (!qEnvironmentVariableIsSet("QT_DEVICE_PIXEL_RATIO")
        && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
        && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
        && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication app(argc, argv);
    QFile file(":css/gui.css");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QFont font = QApplication::font();
#ifdef _WINDOWS
    font.setFamily("Microsoft YaHei");
#endif
    // let hidpi font scale
    font.setPointSize(12);
//    qDebug() << font.pixelSize();
    QApplication::setFont(font);
    Window window;
    window.setStyleSheet(QString(file.readAll()));
    window.show();
    int ret = QApplication::exec();
#ifdef _WINDOWS
    CrashHandler::freeHandler();
#endif
    Logger::endRecord();
    return ret;
}