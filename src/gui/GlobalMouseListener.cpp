//
// Created by helywin on 2021/4/26.
//

#include "GlobalMouseListener.hpp"
#include <QDebug>
#include <QTimer>

#ifdef _WINDOWS
#include <windows.h>
GlobalMouseListener *gInstance = nullptr;
HHOOK gHook = nullptr;

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    QEvent::Type type;
    Qt::MouseButton button;
    bool emitSignal = true;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    const auto midButton = Qt::MiddleButton;
#else
    const auto midButton = Qt::MidButton;
#endif
    switch (wParam) {
        case WM_LBUTTONDOWN:
            type = QEvent::MouseButtonPress;
            button = Qt::LeftButton;
            break;
        case WM_LBUTTONUP:
            type = QEvent::MouseButtonRelease;
            button = Qt::LeftButton;
            break;
        case WM_LBUTTONDBLCLK:
            type = QEvent::MouseButtonDblClick;
            button = Qt::LeftButton;
            break;
        case WM_RBUTTONDOWN:
            type = QEvent::MouseButtonPress;
            button = Qt::RightButton;
            break;
        case WM_RBUTTONUP:
            type = QEvent::MouseButtonRelease;
            button = Qt::RightButton;
            break;
        case WM_RBUTTONDBLCLK:
            type = QEvent::MouseButtonPress;
            button = Qt::RightButton;
            break;
        case WM_MBUTTONDOWN:
            type = QEvent::MouseButtonPress;
            button = midButton;
            break;
        case WM_MBUTTONUP:
            type = QEvent::MouseButtonRelease;
            button = midButton;
            break;
        case WM_MBUTTONDBLCLK:
            type = QEvent::MouseButtonDblClick;
            button = midButton;
            break;
        default:
            emitSignal = false;
            break;
    }
    if (emitSignal) {
        //qDebug() << "global mouse event " << type << " " << button;
        QTimer::singleShot(0, [=] {
            gInstance->mouseEvent(type, button);
        });
    }
    return CallNextHookEx(gHook, nCode, wParam, lParam);
}

GlobalMouseListener *GlobalMouseListener::instance()
{
    if (!gInstance) {
        gHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, nullptr, 0);
        if (gHook == nullptr) {
            qDebug() << "Hook failed";
        } else {
            qDebug() << "Hook succeeded";
            gInstance = new GlobalMouseListener;
        }
    }
    return gInstance;
}
#elif __linux
// https://www.jianshu.com/p/927fc573d081
#endif
