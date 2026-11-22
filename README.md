<div align="center">

# D3D11 Overlay ImGui

**A DirectX 11 overlay built with ImGui, as an EXE or a DLL (x86 / x64).**

![Language](https://img.shields.io/badge/C%2B%2B-16171C?logo=cplusplus)
![Platform](https://img.shields.io/badge/Windows-16171C)
![Arch](https://img.shields.io/badge/x86%20%7C%20x64-16171C)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-16171C)](LICENSE)

</div>

---

D3D11 Overlay ImGui creates its own transparent, topmost window and renders an [ImGui](https://github.com/ocornut/imgui) interface on top of a game or an application. The overlay follows the target window (position, size and focus), so it works even if the target does not use DirectX.

The same code base builds as an EXE for an external overlay or as a DLL for an internal one, in x86 and x64. The overlay window is hidden from OBS and other window capture applications.

### OVERLAY WINDOW PICKER (EXE)

[Demo](https://github.com/adamhlt/D3D11-Overlay-ImGui/assets/48086737/5cb30859-7157-44ea-ba5c-9dad5ec66980)

### OBS DEMO (DLL)

[Demo](https://github.com/adamhlt/D3D11-Overlay-ImGui/assets/48086737/1afe86bb-d1e7-4b09-a6dc-4c153770bfe5)

## Features

**Overlay:**
- Transparent and topmost window that follows the target window.
- Click-through when the menu is hidden or when the target is not focused.
- Hidden from OBS and window capture applications.

**EXE:**
- Window picker to choose the application to draw on.
- Exits when the target window is closed.

**DLL:**
- Targets the main window of the host process automatically.
- The host process does not need to use DirectX.
- Unloads itself when the overlay is closed.

## Usage

Write your menu in `DrawMenu()` (`src/Drawing.cpp`), then build as an EXE or a DLL.

| Key      | Action                   |
|----------|--------------------------|
| `INSERT` | Show / hide the menu     |
| `END`    | Close the overlay        |

- **EXE**: run it, pick the target window in the list and click **Start Overlay**.
- **DLL**: inject it into the target process, the overlay starts on the main window of the process.

> **Note**
> The ImGui demo window source (`imgui_demo.cpp`) is only compiled in Debug.

## Build

> **Note**
> Make sure that `DXSDK_DIR` is declared in your environment variables, it is set by the [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812) installer.

1. Open `D3D11 Overlay ImGui.sln` in Visual Studio (v143 toolset).
2. Build in Debug or Release, for x86 or x64.

To build a DLL, set **Configuration Type** to **Dynamic Library (.dll)** in the project properties (**General**).

- **Output:** `D3D11 Overlay ImGui.exe` or `D3D11 Overlay ImGui.dll` in the `Debug` / `Release` (x86) or `x64\Debug` / `x64\Release` folder.
- **Architecture:** the DLL or EXE must match the architecture of the target process.

> **Warning**
> If you get a linking error, make sure that the DirectX SDK is correctly installed.

## Libraries

- [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
- [ImGui](https://github.com/ocornut/imgui)

## License

Released under the [GNU GPLv3](LICENSE).
