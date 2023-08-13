```
      ____ _____ ____ ______   ____                  __               ____          ______      _ 
     / __ \__  // __ <  <  /  / __ \_   _____  _____/ /___ ___  __   /  _/___ ___  / ____/_  __(_)
    / / / //_ </ / / / // /  / / / / | / / _ \/ ___/ / __ `/ / / /   / // __ `__ \/ / __/ / / / /
   / /_/ /__/ / /_/ / // /  / /_/ /| |/ /  __/ /  / / /_/ / /_/ /  _/ // / / / / / /_/ / /_/ / /
  /_____/____/_____/_//_/   \____/ |___/\___/_/  /_/\__,_/\__, /  /___/_/ /_/ /_/\____/\__,_/_/
                                                         /____/                             

                              
                                D3D11 Overlay with ImGui (x86 / x64)
                                   EXE / DLL DirectX 11 Overlay
```
<p align="center">
    <img src="https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=for-the-badge&logo=appveyor" alt="C++">
    <img src="https://img.shields.io/badge/platform-Windows-0078d7.svg?style=for-the-badge&logo=appveyor" alt="Windows">
    <img src="https://img.shields.io/badge/arch-x86-red.svg?style=for-the-badge&logo=appveyor" alt="x86">
    <img src="https://img.shields.io/badge/arch-x64-green.svg?style=for-the-badge&logo=appveyor" alt="x64">
</p>

## :open_book: Project Overview :

D3D11 Overlay ImGui use D3D11 to create a DirectX window with ImGui, and this allow you to draw on top of games / application.

You can easily use it as a DLL for internal cheat and EXE for external. Everything is setup you just need to choose between DLL or EXE. D3D11 Overlay ImGui create his own window, if you use it as a DLL you main process don't need to use DirectX.

This project works in x86 and x64, DLL and EXE.

#### Features :

- Hide your cheats from OBS or any window capture app.
- Create cheat with ImGui on games that don't use DirectX.
- Allow creation of internal / external cheats.
- Easy to build, everything is setup.

#### Used librairies :

- [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
- [ImGui](https://github.com/ocornut/imgui)

## :rocket: Getting Started

> **Note** <br>
> Make sure that **DXSDK_DIR** is declared in your environment variables.

To see your environment variables :

> **Settings --> System --> About --> System Advanced Settings --> Environment Variables**

### Visual Studio :

1. Open the solution file (.sln).
2. Build the project in Release (x86 or x64)

Every configuration in x86 / x64 (Debug and Realese) are already configured with librairies and includes.

Everything is setup, you just need to choose between DLL or EXE.

> **Warning** <br>
> If you have any linking error when compiling make sure that you have correctly install DirectX SDK.

## 🧪 Demonstration :

[Demo](https://github.com/adamhlt/D3D11-Overlay-ImGui/assets/48086737/1afe86bb-d1e7-4b09-a6dc-4c153770bfe5)
