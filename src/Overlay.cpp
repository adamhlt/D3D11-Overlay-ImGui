#include "Overlay.h"
#include "Drawing.h"
#include "Renderer.h"
#include "WindowTracker.h"

#include <atomic>
#include <dwmapi.h>
#include <tchar.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#pragma comment(lib, "dwmapi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
    constexpr LONG kStyleInteractive = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW;
    constexpr LONG kStyleClickThrough = kStyleInteractive | WS_EX_TRANSPARENT;

    HMODULE hCurrentModule = nullptr;
    std::atomic<bool> bRunning = false;

    /**
        @brief : Window message handler (https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc).
    **/
    LRESULT WINAPI WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SIZE:
            if (Renderer::IsReady() && wParam != SIZE_MINIMIZED)
                Renderer::Resize(LOWORD(lParam), HIWORD(lParam));
            return 0;

        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)
                return 0;
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            break;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    /**
        @brief : Function that create the transparent and topmost overlay window.
    **/
    HWND CreateOverlayWindow(const WNDCLASSEX& wc)
    {
        RegisterClassEx(&wc);
        const HWND hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, _T("D3D11 Overlay ImGui"), WS_POPUP,
            0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

        SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
        const MARGINS margin = { -1, 0, 0, 0 };
        DwmExtendFrameIntoClientArea(hWnd, &margin);

        return hWnd;
    }

    void InitImGui(const HWND hWnd)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;

        ImGui::StyleColorsDark();

        // Scale the font size depending of the screen size.
        const HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info = {};
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &info);
        const int iMonitorHeight = info.rcMonitor.bottom - info.rcMonitor.top;

        if (iMonitorHeight > 1080)
        {
            constexpr float fScale = 2.0f;
            ImFontConfig cfg;
            cfg.SizePixels = 13 * fScale;
            io.Fonts->AddFontDefault(&cfg);
        }

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
    }

    void ShutdownImGui()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    /**
        @brief : Function that dispatch the pending window messages.
        @retval : false when the window has been closed.
    **/
    bool PumpMessages()
    {
        bool bContinue = true;
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                bContinue = false;
        }
        return bContinue;
    }

    void RenderFrame(const HWND hWnd)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        Drawing::Draw();
        ImGui::EndFrame();

        // Overlay handle inputs when menu is showed.
        SetWindowLong(hWnd, GWL_EXSTYLE, Drawing::IsActive() ? kStyleInteractive : kStyleClickThrough);

        ImGui::Render();
        Renderer::Clear();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        Renderer::Present();
    }
}

void Overlay::SetModule(const HMODULE hModule)
{
    hCurrentModule = hModule;
}

bool Overlay::IsRunning()
{
    return bRunning;
}

/**
    @brief : Function that create the overlay window and run the main loop until the overlay is closed.
**/
void Overlay::Run()
{
    bRunning = true;
    ImGui_ImplWin32_EnableDpiAwareness();

    // Get the main window of the process when overlay as DLL.
#ifdef _WINDLL
    if (!WindowTracker::IsTargeted())
        WindowTracker::FindCurrentProcessWindow();
#endif

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = _T("D3D11 Overlay ImGui");
    wc.style = CS_VREDRAW | CS_HREDRAW;

    const HWND hWnd = CreateOverlayWindow(wc);

    if (!Renderer::Init(hWnd))
    {
        Renderer::Shutdown();
        DestroyWindow(hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        DeleteObject(wc.hbrBackground);
        bRunning = false;
        return;
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    InitImGui(hWnd);

    while (PumpMessages())
    {
        if (GetAsyncKeyState(VK_END) & 1)
            break;

        // Check if the targeted window is still up.
        if (WindowTracker::IsTargeted() && !WindowTracker::IsTargetAlive())
            break;

        if (WindowTracker::IsTargeted())
        {
            // Move the window on top of the targeted window and handle resize.
            WindowTracker::MoveOverlayOnTarget(hWnd);

            // Clear overlay when the targeted window is not focus.
            if (!WindowTracker::IsTargetOrOverlayFocused(hWnd))
            {
                SetWindowLong(hWnd, GWL_EXSTYLE, kStyleClickThrough);
                Renderer::Clear();
                Renderer::Present();
                continue;
            }
        }
#ifdef _WINDLL
        else
        {
            // Nothing to draw on until a target window exists.
            Sleep(10);
            continue;
        }
#endif

        RenderFrame(hWnd);
    }

    ShutdownImGui();
    Renderer::Shutdown();
    DestroyWindow(hWnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    DeleteObject(wc.hbrBackground);

    bRunning = false;
}

/**
    @brief : Thread entry point used when the overlay is build as a DLL.
             The DLL unloads itself once the overlay is closed.
**/
DWORD WINAPI Overlay::ThreadMain(LPVOID)
{
    Run();

    if (hCurrentModule != nullptr)
        FreeLibraryAndExitThread(hCurrentModule, 0);

    return 0;
}
