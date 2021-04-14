//
// Created by MSI1 on 2020/2/19.
//

#ifndef SOLAR_PRO_CRASHHANDLER_HPP
#define SOLAR_PRO_CRASHHANDLER_HPP

#ifndef _WINDOWS
#error only windows allowed
#endif

#include "CrashHandler_Export.hpp"
#include <QString>
#include <QCoreApplication>
#include <windows.h>
#include <windef.h>

namespace Com
{
class CrashHandlerPrivate;

// typedef LONG (*ExceptionFilter)(LPEXCEPTION_POINTERS lpExceptionInfo);

// 定义函数指针
using ExceptionFilter = LONG (*)(LPEXCEPTION_POINTERS lpExceptionInfo);

/*!
 * @brief 崩溃处理类
 * 捕捉程序致命错误, 报错部分程序堆栈信息供调试分析造成崩溃的值
 *
 * 利用Windows自带的dbghelp.dll来写崩溃转储文件, 生产dump文件, 可以用Visual Studio打开调试。
 * @remark 调试时一定要设置好pdb文件以及代码位置, 确保能够正确定位程序出现崩溃时对应代码的行号
 *
 * @par
 * 使用示例:
 * @code{.cpp}
 * auto handler = CrashHandler::initHandler();
 * handler->setApplicationName("app_name");
 * handler->setApplicationVersion("v1.0.0");
 * handler->setMiniDumpDir("crash");
 * // 你的代码
 * // ......
 * CrashHandler::freeHandler();
 * @endcode
 * @remark 以上代码最好放在程序的最前面, 最后要释放
 */
class COM_CRASHHANDLER_API CrashHandler
{
Q_DECLARE_TR_FUNCTIONS(CrashHandler)
public:
    CrashHandler();
    ~CrashHandler();
    CrashHandler(const CrashHandler &) = delete;
    CrashHandler &operator=(const CrashHandler &) = delete;
    CrashHandler &operator=(CrashHandler &&) = delete;
    void setApplicationName(const QString &name);
    void setApplicationVersion(const QString &version);
    void setFileNameFormatString(const QString &format);
    void setMiniDumpDir(const QString &dir);
    static CrashHandler *initHandler();
    static LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS lpExceptionInfo);
    static void freeHandler();
    void setExceptionFilterCallback(ExceptionFilter callback);

private:
    CrashHandlerPrivate *d;      ///< 数据封装
};

}   // namespace SOLAR

#endif   // SOLAR_PRO_CRASHHANDLER_HPP
