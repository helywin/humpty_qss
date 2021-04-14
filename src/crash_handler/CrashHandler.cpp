//
// Created by MSI1 on 2020/2/19.
//

#include "CrashHandler.hpp"
#include <windows.h>
#include <windef.h>
#include <dbghelp.h>
#include <QLibrary>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>
#include <QProcess>
#include <io.h>
//#include <minidumpapiset.h>

#include "Logger.hpp"

/*!
 * @brief DLL MAIN函数,参考Windows api
 * @param hModule 句柄
 * @param ul_reason_for_call 调用原因
 * @param lpReserved 保留字
 * @return 返回值
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

namespace Com
{
LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS lpExceptionInfo);   //前置声明

/*!
 * @brief 崩溃处理私有类
 */
class CrashHandlerPrivate
{
Q_DECLARE_TR_FUNCTIONS(CrashHandlerPrivate)
private:
    friend class CrashHandler;

    friend LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS lpExceptionInfo);
    QString mAppName;   ///< 名称
    QString mAppVersion;   ///< 版本
    QString mFormat = "yyyyMMdd-hhmmss";   ///< 时间格式
    QString mMiniDumpDir = ".";   ///< 转储文件目录
    QString mFileName;   ///< 文件名
    ExceptionFilter mCallback = nullptr;   ///< 回调函数

    int generateMiniDump(LPEXCEPTION_POINTERS pExceptionPointers);
};

/*!
 * @brief 异常码转字符串
 * @param code 异常码
 * @return 字符串
 */
QString exceptionCodeToString(DWORD code)
{
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION:
            return "ACCESS_VIOLATION";
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            return "DATATYPE_MISALIGNMENT";
        case EXCEPTION_BREAKPOINT:
            return "BREAKPOINT";
        case EXCEPTION_SINGLE_STEP:
            return "SINGLE_STEP";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            return "ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            return "FLT_DENORMAL_OPERAND";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            return "FLT_DIVIDE_BY_ZERO";
        case EXCEPTION_FLT_INEXACT_RESULT:
            return "FLT_INEXACT_RESULT";
        case EXCEPTION_FLT_INVALID_OPERATION:
            return "FLT_INVALID_OPERATION";
        case EXCEPTION_FLT_OVERFLOW:
            return "FLT_OVERFLOW";
        case EXCEPTION_FLT_STACK_CHECK:
            return "FLT_STACK_CHECK";
        case EXCEPTION_FLT_UNDERFLOW:
            return "FLT_UNDERFLOW";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            return "INT_DIVIDE_BY_ZERO";
        case EXCEPTION_INT_OVERFLOW:
            return "INT_OVERFLOW";
        case EXCEPTION_PRIV_INSTRUCTION:
            return "PRIV_INSTRUCTION";
        case EXCEPTION_IN_PAGE_ERROR:
            return "IN_PAGE_ERROR";
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            return "ILLEGAL_INSTRUCTION";
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            return "NONCONTINUABLE_EXCEPTION";
        case EXCEPTION_STACK_OVERFLOW:
            return "STACK_OVERFLOW";
        case EXCEPTION_INVALID_DISPOSITION:
            return "INVALID_DISPOSITION";
        case EXCEPTION_GUARD_PAGE:
            return "GUARD_PAGE";
        case EXCEPTION_INVALID_HANDLE:
            return "INVALID_HANDLE";
        default:
            return "UNKNOWN_EXCEPTION";
    }
}

/*!
 * @brief 构造函数
 */
CrashHandler::CrashHandler() : d(new CrashHandlerPrivate)
{
    // d->mCallback = defaultExceptionFilter;
}

CrashHandler::~CrashHandler()
{
    delete d;
}

CrashHandler *gHandlerPtr = nullptr;   //! \brief 唯一实例

/*!
 * @brief 创建实例,初始化
 * @return 崩溃处理类指针
 */
CrashHandler *CrashHandler::initHandler()
{
    if (gHandlerPtr) {
        return gHandlerPtr;
    } else {
        gHandlerPtr = new CrashHandler;
        SetUnhandledExceptionFilter(CrashHandler::exceptionHandler);
        return gHandlerPtr;
    }
}

/*!
 * @brief 释放实例
 */
void CrashHandler::freeHandler()
{
    delete gHandlerPtr;
}

/*!
 * @brief 异常处理函数
 * @param lpExceptionInfo 异常处理信息
 * @return 返回值
 */
LONG WINAPI CrashHandler::exceptionHandler(LPEXCEPTION_POINTERS lpExceptionInfo)
{
    if (IsDebuggerPresent()) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // 生产dump文件
    int ret = gHandlerPtr->d->generateMiniDump(lpExceptionInfo);
    QString message =
            tr("    Oops! It seems that an unexpected crash happened.\n"
               "    Exception Info: %1\n"
               "    Exception Address: 0x%2\n"
               "    Please report the dump file.")
                    .arg(exceptionCodeToString(lpExceptionInfo->ExceptionRecord->ExceptionCode))
                    .arg(QString::number(
                            (unsigned long long) lpExceptionInfo->ExceptionRecord->ExceptionAddress,
                            16).toUpper());
    int msgboxId = QMessageBox::critical(nullptr, tr("Exception"),
                                         message, tr("See Dump"), tr("Ignore"), tr("Restart"));
    if (msgboxId == 0) {
        // 资源管理器打开文件
        QFileInfo info(gHandlerPtr->d->mFileName);
        QString cmd = QString("/Select, %1").arg(info.absoluteFilePath().replace("/", "\\"));
#ifdef UNICODE
        ShellExecute(nullptr, TEXT("open"), TEXT("Explorer"),
                     reinterpret_cast<LPCTSTR>(cmd.utf16()), nullptr, SW_SHOWNORMAL);   //打开转储文件位置
#else
        ShellExecute(nullptr, "open", "Explorer", cmd.toStdString().data(), nullptr, SW_SHOWNORMAL);   //打开转储文件位置
#endif
    } else if (msgboxId == 1) {
        exit(-1);
    } else if (msgboxId == 2) {
        // 重启程序
        // todo 解决有时候不能重启的问题
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList{});
    }
    return ret;
}

void CrashHandler::setExceptionFilterCallback(ExceptionFilter callback)
{
    // d->mCallback = callback;
}

/*!
 * @brief 生成转储文件
 * @param pExceptionPointers 异常信息
 * @return 返回值
 */
int CrashHandlerPrivate::generateMiniDump(LPEXCEPTION_POINTERS pExceptionPointers)
#if 0
{
printf("minidump");
BOOL bOwnDumpFile = FALSE;
HANDLE hDumpFile = NULL;
MINIDUMP_EXCEPTION_INFORMATION ExpParam;

typedef BOOL(WINAPI *MiniDumpWriteDumpT)(
        HANDLE,
        DWORD,
        HANDLE,
        MINIDUMP_TYPE,
        PMINIDUMP_EXCEPTION_INFORMATION,
        PMINIDUMP_USER_STREAM_INFORMATION,
        PMINIDUMP_CALLBACK_INFORMATION
);

MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
if (hDbgHelp)
    pfnMiniDumpWriteDump = (MiniDumpWriteDumpT) GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

if (pfnMiniDumpWriteDump) {
    TCHAR zFileName[MAX_PATH] = {0};
    LPSTR szFileName = zFileName;
    const char *szVersion = "SOLAR";
    TCHAR dwBufferSize = MAX_PATH;
    SYSTEMTIME stLocalTime;
    GetLocalTime(&stLocalTime);
    CreateDirectory(szFileName, NULL);
    wsprintf(szFileName, "%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
             szVersion,
             stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
             stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
             GetCurrentProcessId(), GetCurrentThreadId());
    // szFileName是全路径文件名，这样可以指定dump文件的保存位置
    hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    bOwnDumpFile = TRUE;
    OutputDebugString(szFileName);


    if (hDumpFile != INVALID_HANDLE_VALUE) {
        ExpParam.ThreadId = GetCurrentThreadId();
        ExpParam.ExceptionPointers = pExceptionPointers;
        ExpParam.ClientPointers = FALSE;

        pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                             hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);
        if (bOwnDumpFile)
            CloseHandle(hDumpFile);
    }
}

if (hDbgHelp != NULL)
    FreeLibrary(hDbgHelp);

return EXCEPTION_EXECUTE_HANDLER;
}
#else
{
    // 读取dbghelp.dll
    QLibrary dbgHelpDll("dbghelp.dll");
    typedef BOOL(WINAPI *MiniDumpWriteDumpT)(
            HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
            PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
    MiniDumpWriteDumpT pfnMiniDumpWriteDump;
    if (dbgHelpDll.load()) {
        pfnMiniDumpWriteDump = (MiniDumpWriteDumpT) dbgHelpDll.resolve("MiniDumpWriteDump");
        LOGI(tr("load `dbghelp.dll` at `%1`").arg(dbgHelpDll.fileName()));
    } else {
        LOGC(tr("load `dbghelp.dll` failed - %2").arg(dbgHelpDll.errorString()));
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (!QDir::current().exists(mMiniDumpDir)) {
        if (!QDir::current().mkpath(mMiniDumpDir)) {
            mMiniDumpDir = ".";
        }
    }
    mFileName = mMiniDumpDir + "/" + mAppName + "@" + mAppVersion + "-"
                + QDateTime::currentDateTime().toString(mFormat)
                + QString("-%1-%2.dmp").arg(GetCurrentProcessId()).arg(GetCurrentThreadId());
    QFile miniDumpFile(mFileName);
    if (!miniDumpFile.open(QIODevice::WriteOnly)) {
        LOGW(tr("open file `%1` failed").arg(mFileName));
        dbgHelpDll.unload();
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    LOGI(tr("generate dump file `%1` succeed").arg(mFileName));

    auto hDumpFile = (HANDLE) _get_osfhandle(miniDumpFile.handle());
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;
    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = pExceptionPointers;
    ExpParam.ClientPointers = FALSE;
    // 写转储文件
    pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
                         MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : nullptr),
                         nullptr, nullptr);

    miniDumpFile.close();
    dbgHelpDll.unload();
    return EXCEPTION_EXECUTE_HANDLER;
}

#endif

/*!
 * @brief 设置应用名称
 * @param name 名称
 */
void CrashHandler::setApplicationName(const QString &name)
{
    d->mAppName = name;
}

/*!
 * @brief 设置应用版本号
 * @param version 版本号
 */
void CrashHandler::setApplicationVersion(const QString &version)
{
    d->mAppVersion = version;
}

/*!
 * @brief 设置文件名格式
 * @param format 格式
 */
void CrashHandler::setFileNameFormatString(const QString &format)
{
    d->mFormat = format;
}

/*!
 * @brief 设置转储文件存储目录
 * @param dir 目录名
 */
void CrashHandler::setMiniDumpDir(const QString &dir)
{
    d->mMiniDumpDir = dir;
    if (!QDir::current().exists(dir)) {
        QDir::current().mkpath(dir);
    }
}

}   // namespace SOLAR