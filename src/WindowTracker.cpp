#include "WindowTracker.h"
#include "ProcessUtils.h"

#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

namespace
{
    HWND hTargetWindow = nullptr;
    DWORD dTargetPID = 0;

    /**
        @brief : Function that check if a window is cloaked.
    **/
    bool IsWindowCloaked(const HWND hWindow)
    {
        DWORD dCloaked = 0;
        const HRESULT hr = DwmGetWindowAttribute(hWindow, DWMWA_CLOAKED, &dCloaked, sizeof(dCloaked));
        return SUCCEEDED(hr) && dCloaked;
    }

    /**
        @brief : Function that check if a window is a valid overlay target.
    **/
    bool IsWindowValid(const HWND hWindow)
    {
        if (!IsWindowVisible(hWindow) || IsIconic(hWindow) || IsWindowCloaked(hWindow))
            return false;

        RECT rect;
        GetClientRect(hWindow, &rect);
        const DWORD dStyles = static_cast<DWORD>(GetWindowLongPtr(hWindow, GWL_STYLE));
        const DWORD dExStyles = static_cast<DWORD>(GetWindowLongPtr(hWindow, GWL_EXSTYLE));

        if (dExStyles & WS_EX_TOOLWINDOW)
            return false;
        if (dStyles & WS_CHILD)
            return false;
        if (rect.bottom == 0 || rect.right == 0)
            return false;

        return true;
    }

    /**
        @brief : Callback that retrieve the first valid window of the current process.
    **/
    BOOL CALLBACK EnumProcessWindow(const HWND hWindow, LPARAM)
    {
        DWORD dPID;
        GetWindowThreadProcessId(hWindow, &dPID);
        if (GetCurrentProcessId() != dPID)
            return TRUE;

        if (!IsWindowValid(hWindow))
            return TRUE;

        WindowTracker::SetTarget(hWindow);
        return FALSE;
    }

    /**
        @brief : Callback that retrieve all the valid windows with their title and process name.
    **/
    BOOL CALLBACK EnumAllWindows(const HWND hWindow, const LPARAM lParam)
    {
        if (!IsWindowValid(hWindow))
            return TRUE;

        char szTitle[256] = {};
        if (GetWindowTextA(hWindow, szTitle, sizeof(szTitle)) == 0)
            return TRUE;

        DWORD dPID;
        GetWindowThreadProcessId(hWindow, &dPID);

        auto* pWindowList = reinterpret_cast<std::vector<WindowTracker::WindowItem>*>(lParam);
        pWindowList->push_back({ hWindow, szTitle, ProcessUtils::GetProcessName(dPID) });
        return TRUE;
    }
}

/**
    @brief : Function that check if a target window is set.
**/
bool WindowTracker::IsTargeted()
{
    return hTargetWindow != nullptr;
}

/**
    @brief : Function that define the target window and bring it to the foreground.
    @param hWindow : target window.
**/
void WindowTracker::SetTarget(const HWND hWindow)
{
    hTargetWindow = hWindow;
    SetForegroundWindow(hTargetWindow);
    GetWindowThreadProcessId(hTargetWindow, &dTargetPID);
}

/**
    @brief : Function that look if the targeted window is still up.
**/
bool WindowTracker::IsTargetAlive()
{
    if (hTargetWindow == nullptr || !IsWindow(hTargetWindow))
        return false;

    DWORD dCurrentPID;
    GetWindowThreadProcessId(hTargetWindow, &dCurrentPID);
    return dCurrentPID == dTargetPID;
}

/**
    @brief : Function that retrieve the main window of the current process and target it.
             Used when the overlay is build as a DLL.
    @retval : true if a window has been found.
**/
bool WindowTracker::FindCurrentProcessWindow()
{
    EnumWindows(EnumProcessWindow, 0);
    return IsTargeted();
}

/**
    @brief : Function that clear the window list and fill it with all the valid windows.
**/
void WindowTracker::GetAllWindows(std::vector<WindowItem>& vWindowList)
{
    vWindowList.clear();
    EnumWindows(EnumAllWindows, reinterpret_cast<LPARAM>(&vWindowList));
}

/**
    @brief : Function that move the overlay on top of the targeted window.
    @param hOverlay : Window of the overlay.
**/
void WindowTracker::MoveOverlayOnTarget(const HWND hOverlay)
{
    if (hTargetWindow == nullptr)
        return;

    RECT rect;
    GetWindowRect(hTargetWindow, &rect);

    const int iWidth = rect.right - rect.left - 5;
    const int iHeight = rect.bottom - rect.top - 29;

    SetWindowPos(hOverlay, nullptr, rect.left, rect.top, iWidth, iHeight, SWP_SHOWWINDOW);
}

/**
    @brief : Function that check if the overlay window or the targeted window is focus.
    @param hOverlay : Window of the overlay.
**/
bool WindowTracker::IsTargetOrOverlayFocused(const HWND hOverlay)
{
    char szForegroundClass[125];
    char szTargetClass[125];
    char szOverlayClass[125];

    if (GetClassNameA(GetForegroundWindow(), szForegroundClass, sizeof(szForegroundClass)) == 0)
        return false;

    if (GetClassNameA(hTargetWindow, szTargetClass, sizeof(szTargetClass)) == 0)
        return false;

    if (GetClassNameA(hOverlay, szOverlayClass, sizeof(szOverlayClass)) == 0)
        return false;

    return strcmp(szForegroundClass, szTargetClass) == 0 || strcmp(szForegroundClass, szOverlayClass) == 0;
}
