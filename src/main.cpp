#include "Overlay.h"

#ifdef _WINDLL

namespace
{
    HANDLE hOverlayThread = nullptr;
}

BOOL WINAPI DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, const LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        Overlay::SetModule(hinstDLL);
        hOverlayThread = CreateThread(nullptr, 0, Overlay::ThreadMain, nullptr, 0, nullptr);
    }

    if (fdwReason == DLL_PROCESS_DETACH)
    {
        // The overlay unloads itself when it ends. Only stop the thread if the DLL
        // is unloaded from the outside while the overlay is still running.
        if (lpReserved == nullptr && hOverlayThread != nullptr && Overlay::IsRunning())
            TerminateThread(hOverlayThread, 0);

        if (hOverlayThread != nullptr)
            CloseHandle(hOverlayThread);
    }

    return TRUE;
}

#else

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    Overlay::Run();

    return 0;
}

#endif
