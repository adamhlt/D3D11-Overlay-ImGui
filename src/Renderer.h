#pragma once

#include <Windows.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

/**
    D3D11 device, swap chain and render target management.
**/
namespace Renderer
{
    bool Init(HWND hWnd);
    void Shutdown();
    bool IsReady();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

    void Resize(UINT uWidth, UINT uHeight);

    void Clear();
    void Present();
}
