#pragma once

#include <Windows.h>

/**
    Overlay window life cycle: window creation, ImGui setup and main loop.
**/
namespace Overlay
{
    void SetModule(HMODULE hModule);
    bool IsRunning();

    void Run();
    DWORD WINAPI ThreadMain(LPVOID lpParameter);
}
