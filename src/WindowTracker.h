#pragma once

#include <Windows.h>
#include <string>
#include <vector>

/**
    Target window handling: window enumeration, validation, overlay positioning.
**/
namespace WindowTracker
{
    struct WindowItem
    {
        HWND hWindow = nullptr;
        std::string sTitle;
        std::string sProcessName;
    };

    bool IsTargeted();
    void SetTarget(HWND hWindow);
    bool IsTargetAlive();

    bool FindCurrentProcessWindow();
    void GetAllWindows(std::vector<WindowItem>& vWindowList);

    void MoveOverlayOnTarget(HWND hOverlay);
    bool IsTargetOrOverlayFocused(HWND hOverlay);
}
