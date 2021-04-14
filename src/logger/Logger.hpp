//
// Created by jiang on 2020/3/19.
//

#ifndef SOLAR_PRO_LOGGER_HPP
#define SOLAR_PRO_LOGGER_HPP

#include "Logger_Export.hpp"
#include <QString>
#include <type_traits>

namespace Com
{
#ifndef __FUNC__
#ifdef _WINDOWS   // WINDOWS
#define __FUNC__ __FUNCTION__
#else   //*NIX
#define __FUNC__ __func__
#endif
#endif

/*!
 * @brief 日志类
 */
class COM_LOGGER_API Logger
{
public:
    Q_DISABLE_COPY(Logger)

    /*!
     * @brief 日志内容
     */
    enum LogContent
    {
        lc_date = 0x01,     ///< 日期
        lc_time = 0x02,     ///< 时间
        lc_thread = 0x04,   ///< 线程ID
        lc_level = 0x08,    ///< 日志等级
        lc_func = 0x10,     ///< 函数名
        lc_file = 0x20,     ///< 文件名
        lc_line = 0x40,     ///< 行号
    };
    Q_DECLARE_FLAGS(LogContents, LogContent)

    /*!
     * @brief 日志等级
     */
    enum LogLevel
    {
        llOther,          ///< 其他
        llInfo,           ///< 信息
        llWarning,        ///< 警告
        llCritical,       ///< 严重
        llFatal,          ///< 致命
        llDebug,          ///< 调试
    };
    static void setLogContents(LogContents contents);
    static void setLogContent(LogContent content, bool on);
    static void setSaveWithDate(bool flag);
    static void installMessageHandler();
    static void startRecord(bool writeFile = true);
    static void endRecord();
    static void sendLog(LogLevel level, const QString &func,
                        const QString &file, int line, const QString &msg);

    template<typename T>
    inline static void sendLogAny(LogLevel level, const QString &func,
                                  const QString &file, int line, T v)
    {
        sendLog(level, func, file, line, toString(v));
    }

    template<typename... Args>
    static void sendLogMulti(LogLevel level, const QString &func,
                             const QString &file, int line, Args...args)
    {
        QString text;
        unpack(text, args...);
        sendLog(level, func, file, line, text);
    }

    static void info(const QString &info);
    static QString levelStr(LogLevel level);
    static void setAppName(const QString &name);
    static void setAppVersion(const QString &version);
    static void setAppDataPath(const QString &path);

private:
    template<typename T, typename... Args>
    static void unpack(QString &result, T v, Args...args)
    {
        if (!result.isEmpty()) {
            result += QString(",");
        }
        result += toString(v);
        if constexpr(sizeof...(args) > 0) {
            unpack(result, args...);
        }
    }

    template<typename T>
    static QString toString(T v)
    {
        static_assert(std::is_same_v<QString, T> ||
                      std::is_same_v<std::string, T> ||
                      std::is_same_v<const char *, T> ||
                      std::is_arithmetic_v<T>);
        if constexpr(std::is_same_v<T, QString> ||
                     std::is_same_v<const char *, T>) {
            return v;
        }
        if constexpr(std::is_same_v<T, std::string>) {
            return QString::fromStdString(v);
        }
        if constexpr(std::is_arithmetic_v<T>) {
            return QString::number(v);
        }
    }
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::LogContents)

// 宏定义方便使用
#define LOG(level, msg)  Logger::sendLogAny  (level, __FUNC__, __FILE__, __LINE__, msg)
#define LOGM(level, ...) Logger::sendLogMulti(level, __FUNC__, __FILE__, __LINE__, __VA_ARGS__)
// 信息
#define LOGI(msg)        Logger::sendLogAny  (Logger::llInfo, __FUNC__, __FILE__, __LINE__, msg)
#define LOGIM(...)       Logger::sendLogMulti(Logger::llInfo, __FUNC__, __FILE__, __LINE__, __VA_ARGS__)
// 警告
#define LOGW(msg)        Logger::sendLogAny  (Logger::llWarning, __FUNC__, __FILE__, __LINE__, msg)
#define LOGWM(...)       Logger::sendLogMulti(Logger::llWarning, __FUNC__, __FILE__, __LINE__, __VA_ARGS__)
// 严重
#define LOGC(msg)        Logger::sendLogAny  (Logger::llCritical, __FUNC__, __FILE__, __LINE__, msg)
#define LOGCM(...)       Logger::sendLogMulti(Logger::llCritical, __FUNC__, __FILE__, __LINE__, __VA_ARGS__)
// 致命
#define LOGF(msg)        Logger::sendLogAny  (Logger::llFatal, __FUNC__, __FILE__, __LINE__, msg)
#define LOGFM(...)       Logger::sendLogMulti(Logger::llFatal, __FUNC__, __FILE__, __LINE__, __VA_ARGS__)
#if defined(DEBUG) || defined(_DEBUG)
#define LOGD(msg)        Logger::sendLogAny  (Logger::llDebug, __FUNC__, __FILE__, __LINE__, msg)
#define LOGDM(...)       Logger::sendLogMulti(Logger::llDebug, __FUNC__, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOGD(msg)
#endif
#ifdef _WINDOWS
#define INFO(msg) Logger::info(msg)
#else
#define INFO(msg)
#endif

}   // namespace SOLAR

#endif   // SOLAR_PRO_LOGGER_HPP
