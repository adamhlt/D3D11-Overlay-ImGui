#include "Renderer.h"

#include <d3d11.h>

namespace
{
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pDeviceContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11RenderTargetView* pRenderTargetView = nullptr;

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (pBackBuffer != nullptr)
        {
            pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
            pBackBuffer->Release();
        }
    }

    void CleanupRenderTarget()
    {
        if (pRenderTargetView)
        {
            pRenderTargetView->Release();
            pRenderTargetView = nullptr;
        }
    }

    template <typename T>
    void SafeRelease(T*& pObject)
    {
        if (pObject)
        {
            pObject->Release();
            pObject = nullptr;
        }
    }
}

/**
    @brief : Function that create the D3D11 device from the overlay window.
    @param  hWnd : window created for the overlay.
    @retval : true if the device has been created else false.
**/
bool Renderer::Init(const HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    constexpr UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

/**
    @brief : Function that release the render target, the swap chain and the D3D11 device.
**/
void Renderer::Shutdown()
{
    CleanupRenderTarget();
    SafeRelease(pSwapChain);
    SafeRelease(pDeviceContext);
    SafeRelease(pDevice);
}

bool Renderer::IsReady()
{
    return pDevice != nullptr;
}

ID3D11Device* Renderer::GetDevice()
{
    return pDevice;
}

ID3D11DeviceContext* Renderer::GetDeviceContext()
{
    return pDeviceContext;
}

/**
    @brief : Function that resize the swap chain buffers and recreate the render target.
**/
void Renderer::Resize(const UINT uWidth, const UINT uHeight)
{
    if (!IsReady())
        return;

    CleanupRenderTarget();
    pSwapChain->ResizeBuffers(0, uWidth, uHeight, DXGI_FORMAT_UNKNOWN, 0);
    CreateRenderTarget();
}

/**
    @brief : Function that bind the render target and clear it with a fully transparent color.
**/
void Renderer::Clear()
{
    constexpr float fClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, fClearColor);
}

void Renderer::Present()
{
    pSwapChain->Present(1, 0);
}
