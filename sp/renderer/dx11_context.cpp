/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/01/2023 19:47
 */

#include "dx11_context.hpp"

#include "log_system.hpp"

#include <cmath>
#include <string>

const D3D_DRIVER_TYPE DriverTypes[] =
{
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
};

dx_render_context DxRenderContext;

void DxRenderContextInit(HWND Window)
{
    DxRenderContext.RenderWindow = Window;

    D3D_FEATURE_LEVEL Levels[] = { D3D_FEATURE_LEVEL_11_0 };
    HRESULT Result = 0;
    for (int DriverIndex = 0; DriverIndex < 3; DriverIndex++)
    {
        Result = D3D11CreateDevice(NULL,
                                   DriverTypes[DriverIndex],
                                   NULL,
                                   D3D11_CREATE_DEVICE_DEBUG,
                                   Levels,
                                   1,
                                   D3D11_SDK_VERSION,
                                   &DxRenderContext.Device,
                                   &DxRenderContext.FeatureLevel,
                                   &DxRenderContext.DeviceContext);
        if (SUCCEEDED(Result))
            break;
    }

    if (FAILED(Result))
    {
        LogError("Failed to create D3D11 device!");
        exit(-1);
    }

    DxRenderContext.Device->QueryInterface(IID_PPV_ARGS(&DxRenderContext.DXGI));
    DxRenderContext.DXGI->GetParent(IID_PPV_ARGS(&DxRenderContext.Adapter));
    DxRenderContext.Adapter->GetParent(IID_PPV_ARGS(&DxRenderContext.Factory));

    RECT Rectangle;
    GetClientRect(Window, &Rectangle);
    DxRenderContext.Width = Rectangle.right - Rectangle.left;
    DxRenderContext.Height = Rectangle.bottom - Rectangle.top;

    DxRenderContextResize(DxRenderContext.Width, DxRenderContext.Height);

    DXGI_ADAPTER_DESC AdapterDesc = {0};
    DxRenderContext.Adapter->GetDesc(&AdapterDesc);
    std::wstring AdapterName = std::wstring(AdapterDesc.Description);
    std::string AnsiAdapterName = std::string(AdapterName.begin(), AdapterName.end());
    LogInfo("Created D3D11 context using GPU: %s", AnsiAdapterName.c_str());
}

void DxRenderContextResize(uint32_t Width, uint32_t Height)
{
    DxRenderContext.Width = Width;
    DxRenderContext.Height = Height;

    DEVMODE DevMode = {0};
    DevMode.dmSize = sizeof(DevMode);
    DevMode.dmDriverExtra = 0;
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode) == 0) 
    {
        DxRenderContext.RefreshRate = round(DevMode.dmDisplayFrequency);
    } else
    {
        DxRenderContext.RefreshRate = 60;
    }

    if (!DxRenderContext.SwapChain)
    {
        DXGI_SWAP_CHAIN_DESC Desc = {};
        Desc.BufferDesc.Width = Width;
        Desc.BufferDesc.Height = Height;
        Desc.BufferDesc.RefreshRate.Numerator = DxRenderContext.RefreshRate;
        Desc.BufferDesc.RefreshRate.Denominator = 1;
        Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        Desc.SampleDesc.Count = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        Desc.BufferCount = 1;
        Desc.OutputWindow = DxRenderContext.RenderWindow;
        // TODO(amelie.h): Add support for fullscreen
        Desc.Windowed = true;

        HRESULT Result = DxRenderContext.Factory->CreateSwapChain(DxRenderContext.Device, &Desc, &DxRenderContext.SwapChain);
        if (FAILED(Result))
        {
            LogError("Failed to create/resize D3D11 swapchain!");
            exit(-1);
        }
    }

    SafeRelease(DxRenderContext.SwapChainBuffer);
    SafeRelease(DxRenderContext.SwapChainRTV);

    DxRenderContext.SwapChain->ResizeBuffers(1, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    DxRenderContext.SwapChain->GetBuffer(0, IID_PPV_ARGS(&DxRenderContext.SwapChainBuffer));
    DxRenderContext.Device->CreateRenderTargetView(DxRenderContext.SwapChainBuffer, NULL, &DxRenderContext.SwapChainRTV);
}

void DxRenderContextFree()
{
    SafeRelease(DxRenderContext.SwapChainBuffer);
    SafeRelease(DxRenderContext.SwapChainRTV);
    SafeRelease(DxRenderContext.SwapChain);
    SafeRelease(DxRenderContext.Factory);
    SafeRelease(DxRenderContext.Adapter);
    SafeRelease(DxRenderContext.DXGI);
    SafeRelease(DxRenderContext.Device);
}
