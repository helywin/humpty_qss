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
#include "CrashHandler.hpp"
#include "Logger.hpp"
#include "version_info.h"
#include "Window.hpp"

using namespace Com;

int main(int argc, char *argv[])
{
#ifdef DEBUG
//    SetDllDirectory(TEXT("D:\\Qt\\6.0.3\\msvc2019_64\\bin"));
#endif
    auto handler = CrashHandler::initHandler();
    Logger::installMessageHandler();
    Logger::setAppName(APP_NAME);
    Logger::setAppVersion(VERSION);
    Logger::setLogContent(Com::Logger::lc_thread, false);
    Logger::setLogContent(Com::Logger::lc_func, false);
    Logger::startRecord(true);
    handler->setApplicationName(APP_NAME);
    handler->setApplicationVersion(VERSION);
    handler->setMiniDumpDir("crash");
#ifdef NO_CRASH_DIALOG
    handler->setExceptionFilterCallback(nullptr);
#endif
    QApplication app(argc, argv);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 1)
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QFile file(":css/gui.css");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QFont font = QApplication::font();
    font.setFamily("Microsoft YaHei");
    QApplication::setFont(font);
    Window window;
    window.setStyleSheet(QString(file.readAll()));
    window.show();
    int ret = QApplication::exec();
    CrashHandler::freeHandler();
    Logger::endRecord();
    return ret;
}