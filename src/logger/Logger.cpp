//
// Created by jiang on 2020/3/19.
//

#include "Logger.hpp"
#include <QtGlobal>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#ifdef QT_NO_SYSTEMTRAYICON
#include <QProcess>
#endif
#include <QDir>
#include <QMutex>
#include <QThread>
#include <QProcess>
#include <QCoreApplication>
#include <QSysInfo>
#include <QSettings>
#include <QThread>
#include <QRegularExpression>

#ifdef _WINDOWS
#include <windows.h>
#include <WinNls.h>
#endif

namespace Com
{
QString appLogPath = "log/";   ///< 日志目录
QString logFile;   ///< 日志文件名
QString gAppName = "App";   ///< 应用名称
QString gAppVersion = "1.0";   ///< 应用版本号
std::atomic<bool> gWriteLog = false;    ///< 是否写文件
QMutex gLogMutex;   ///< 日志多线程锁
bool gSaveWithDate = false;   ///< 是否按照日期存储日志
Logger::LogContents gContents =
        Logger::lc_date | Logger::lc_time | Logger::lc_thread | Logger::lc_level   ///< 日志内容
        | Logger::lc_func | Logger::lc_file | Logger::lc_line;
#define DATE_FORMAT_STR "yyyy-MM-dd"   ///< 日期格式
#define TIME_FORMAT_STR "hh:mm:ss.zzz"   ///< 时间格式
QString gDateTimeFormat = DATE_FORMAT_STR " " TIME_FORMAT_STR;   ///< 日期时间格式字符串

#ifdef _WINDOWS

/*!
 * @brief 获取处理器信息
 * @return 处理器信息
 */
QString cpuType()
{
    auto *cpu = new QSettings(
            R"(HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\CentralProcessor\0)",
            QSettings::NativeFormat);
    auto cpuStr = cpu->value("ProcessorNameString").toString();
    delete cpu;
    return cpuStr;
}

#endif

void updateLogFileName()
{
    if (gSaveWithDate) {
        logFile = appLogPath + QString("%1-%2.log")
                .arg(gAppName)
                .arg(QDate::currentDate().toString("yyyy_MM_dd"));
    } else {
        logFile = appLogPath + QString("%1.log").arg(gAppName);
    }
}

/*!
 * @brief QTextStream到标准输出
 * @return QTextStream
 */
QTextStream &stdOut()
{
    static QTextStream ts(stdout);
    return ts;
}

/*!
 * @brief 记录日志函数
 * @param level 日志等级
 * @param msg 信息
 * @param func 函数名称
 * @param file 文件名
 * @param line 行号
 */
void Logger::sendLog(LogLevel level, const QString &func,
                     const QString &file, int line, const QString &msg)
{
    QString text;
    text += QString("[%1]").arg(QDateTime::currentDateTime().toString(gDateTimeFormat)) +
            QChar('\t');
    if (gContents.testFlag(Logger::lc_thread)) {
        text +=
                QString("Thread:0x%1").arg((int64_t) QThread::currentThreadId(), 8, 16,
                                           QChar('0')) + QChar('\t');
    }
    if (gContents.testFlag(Logger::lc_level)) {
        text += levelStr(level) + QChar('\t');
    }
    QString replacedMsg = msg;
    replacedMsg = replacedMsg.replace(QChar('\n'), QString("\\n"));
    replacedMsg = replacedMsg.replace(QChar('\r'), QString("\\r"));
    text += replacedMsg + QChar('\t');
    if (gContents.testFlag(Logger::lc_func)) {
        //     file.split('/').last() + QChar('\t');
        text += func + QChar('\t');
    }
    if (gContents.testFlag(Logger::lc_file)) {
        text += file.section(QRegularExpression(R"([\\\/])"), -1, -1) + QChar(':');
    }
    if (gContents.testFlag(Logger::lc_line)) {
        text += QString("%1").arg(line);
    }
    if (gSaveWithDate) {
        logFile = QString("log/%2-%1.log")
                .arg(gAppName, QDate::currentDate().toString("yyyy_MM_dd"));
    }

    gLogMutex.lock();
    if (gWriteLog) {
        QFile log(logFile);
        if (log.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream(&log);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            stream.setEncoding(QStringConverter::Utf8);
#else
            stream.setCodec("utf-8");
#endif
            stream << text << "\n";
            log.close();
        }
    }
    text.replace(QChar('\t'), "  ");
    stdOut() << text << "\n";
    stdOut().flush();
    gLogMutex.unlock();
}

/*!
 * @brief 处理Qt QDebug信息
 * @param type QDebug信息类型
 * @param context 内容
 * @param msg 信息
 */
void qDebugHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Logger::LogLevel logLevel;
    switch (type) {
        case QtDebugMsg:
            logLevel = Logger::llDebug;
            break;
        case QtInfoMsg:
            logLevel = Logger::llInfo;
            break;
        case QtWarningMsg:
            logLevel = Logger::llWarning;
            break;
        case QtCriticalMsg:
            logLevel = Logger::llCritical;
            break;
        case QtFatalMsg:
            logLevel = Logger::llFatal;
            break;
    }
    Logger::sendLog(logLevel, context.function, context.file, context.line, "(Qt) " + msg);
}


#define SEND_LOG_INFO(text) Logger::sendLog(llInfo,  __FUNC__, __FILE__, __LINE__, text)

/*!
 * @brief 开始记录
 */
void Logger::startRecord(bool writeFile)
{
    gWriteLog = writeFile;
    if (gWriteLog) {
        QFile log(logFile);
        QFileInfo info(logFile);
        if (!info.dir().exists()) {
            QDir::current().mkpath(info.dir().dirName());
        }
        if (log.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream(&log);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            stream.setEncoding(QStringConverter::Utf8);
#else
            stream.setCodec("utf-8");
#endif
            stream << "***************************************************************\n";
        }
        SEND_LOG_INFO(gAppName + QString("(%1) - Qt(%2)")
                .arg(gAppVersion)
                .arg(QT_VERSION_STR));
        SEND_LOG_INFO(QString("OS:%1 [%2]")
                              .arg(QSysInfo::prettyProductName())
                              .arg(QSysInfo::kernelVersion()));
        SEND_LOG_INFO(QString("CPU:%1 x%2").arg(cpuType(), QThread::idealThreadCount()));
        SEND_LOG_INFO(
                QString("PID:0x%1").arg(QCoreApplication::applicationPid(), 8, 16, QChar('0')));
        SEND_LOG_INFO("DIR:" + QDir::currentPath());
        SEND_LOG_INFO("ENV:PATH:" + qEnvironmentVariable("path"));
#ifdef _WINDOWS
        SEND_LOG_INFO("Code Page:" + QString::number(GetACP()));
#endif
        log.close();
    }
}

/*!
 * @brief 停止记录
 */
void Logger::endRecord()
{
    if (gWriteLog) {
        QFile log(logFile);
        if (log.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream(&log);
            stream << "\n"
                   << "\n";
        }
        log.close();
    }
}

/*!
 * @brief 显示通知
 * @param info 通知内容
 */
void Logger::info(const QString &info)
{
#ifndef QT_NO_SYSTEMTRAYICON
    // SysInfo(info);
#else
    QString icon = QDir::current().absolutePath() + "/SOLAR.png";
    QProcess::startDetached("notify-send " + QString("-i %1 ").arg(icon) + QString("\"%1\"").arg(info));
#endif
}

/*!
 * @brief 日志等级转字符串
 * @param level 日志等级
 * @return 字符串
 */
QString Logger::levelStr(LogLevel level)
{
    switch (level) {
        case llOther:
            return ("[O]");      // OTHER
        case llInfo:
            return ("[I]");       // INFO
        case llWarning:
            return ("[W]");    // WARN
        case llCritical:
            return ("[C]");   // CRITICAL
        case llFatal:
            return ("[F]");      // FATAL
        case llDebug:
            return ("[D]");      // DEBUG
        default:
            return QString();
    }
}

/*!
 * @brief 设置是否按照日期保存日志
 * @param flag 是/否
 */
void Logger::setSaveWithDate(bool flag)
{
    gSaveWithDate = flag;
}

/*!
 * @brief 设置日志包含内容字段
 * @param contents 字段枚举
 */
void Logger::setLogContents(Logger::LogContents contents)
{
    gContents = contents;
    if (contents.testFlag(Logger::lc_date)) {
        gDateTimeFormat += DATE_FORMAT_STR;
    }
    if (contents.testFlag(Logger::lc_time)) {
        if (!gDateTimeFormat.isEmpty()) {
            gDateTimeFormat += " ";
        }
        gDateTimeFormat += TIME_FORMAT_STR;
    }
}

/*!
 * @brief 单个设置日志包含内容字段
 * @param content 单个字段
 * @param on 开/关
 */
void Logger::setLogContent(Logger::LogContent content, bool on)
{
    if (on) {
        gContents |= content;
    } else {
        gContents ^= content;
    }
}

/*!
 * @brief 设置应用名称
 * @param name 名称
 */
void Logger::setAppName(const QString &name)
{
    gAppName = name;
    updateLogFileName();
}

/*!
 * @brief 设置应用版本
 * @param version 版本号
 */
void Logger::setAppVersion(const QString &version)
{
    gAppVersion = version;
}

/*!
 * @brief 设置qt qdebug监听
 */
void Logger::installMessageHandler()
{
    qInstallMessageHandler(qDebugHandler);
}

/*!
 * @brief 设置应用路径
 * @param path 路径
 */
void Logger::setAppDataPath(const QString &path)
{
    appLogPath = path;
    if (!appLogPath.endsWith('/')) {
        appLogPath = path + '/';
    }
    updateLogFileName();
}

}   // namespace SOLAR
