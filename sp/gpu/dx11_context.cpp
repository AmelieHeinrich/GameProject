/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/01/2023 19:47
 */

#include "dx11_context.hpp"

#include "systems/log_system.hpp"
#include "systems/event_system.hpp"
#include "game_data.hpp"

#include <cmath>
#include <string>

extern "C" 
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

const D3D_DRIVER_TYPE DriverTypes[] =
{
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
};

dx_render_context DxRenderContext;

bool DxRenderResizeCallback(event_type Type, void *Sender, void *ListenerInstance, event_data Data)
{
    if (Type == event_type::Resize)
    {
        uint32_t Width = Data.data.u32[0];
        uint32_t Height = Data.data.u32[1];
        DxRenderContextResize(Width, Height);
    }
    return true;
}

void DxRenderContextInit(HWND Window)
{
    DxRenderContext.RenderWindow = Window;

    D3D_FEATURE_LEVEL Levels[] = { D3D_FEATURE_LEVEL_11_0 };
    HRESULT Result = 0;
    for (int DriverIndex = 0; DriverIndex < 3; DriverIndex++)
    {
        Result = D3D11CreateDevice(nullptr,
                                   DriverTypes[DriverIndex],
                                   nullptr,
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

    DXGI_ADAPTER_DESC AdapterDesc = {0};
    DxRenderContext.Adapter->GetDesc(&AdapterDesc);
    std::wstring AdapterName = std::wstring(AdapterDesc.Description);
    std::string AnsiAdapterName = std::string(AdapterName.begin(), AdapterName.end());
    LogInfo("Created D3D11 context using GPU: %s", AnsiAdapterName.c_str());

    RECT Rectangle;
    GetClientRect(Window, &Rectangle);
    DxRenderContext.Width = Rectangle.right - Rectangle.left;
    DxRenderContext.Height = Rectangle.bottom - Rectangle.top;

    DxRenderContextResize(DxRenderContext.Width, DxRenderContext.Height);
    EventSystemRegister(event_type::Resize, nullptr, DxRenderResizeCallback);
}

void DxRenderContextResize(uint32_t Width, uint32_t Height)
{
    DxRenderContext.BufferCount = EgcU32(EgcFile, "buffer_count");
    if (DxRenderContext.BufferCount < 1 && DxRenderContext.BufferCount > 3) 
    {
        LogError("SwapChain buffer count must be in range [1, 3]!");
        exit(-1);
    }

    DxRenderContext.Width = Width;
    DxRenderContext.Height = Height;
    if (DxRenderContext.Buffers.empty())
        DxRenderContext.Buffers.resize(DxRenderContext.BufferCount);

    DEVMODE DevMode = {0};
    DevMode.dmSize = sizeof(DevMode);
    DevMode.dmDriverExtra = 0;
    if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &DevMode) == 0) 
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
        Desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        Desc.SampleDesc.Count = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        Desc.BufferCount = DxRenderContext.BufferCount;
        Desc.OutputWindow = DxRenderContext.RenderWindow;
        Desc.Windowed = true;
        Desc.SwapEffect = DxRenderContext.BufferCount > 1 ? DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL : DXGI_SWAP_EFFECT_DISCARD;
        Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        IDXGISwapChain* Temp = nullptr;
        HRESULT Result = DxRenderContext.Factory->CreateSwapChain(DxRenderContext.Device, &Desc, &Temp);
        if (FAILED(Result))
        {
            LogError("Failed to create/resize D3D11 swapchain!");
            exit(-1);
        }
        Temp->QueryInterface(IID_PPV_ARGS(&DxRenderContext.SwapChain));
        SafeRelease(Temp);
    }

    for (int BufferIndex = 0; BufferIndex < DxRenderContext.BufferCount; BufferIndex++)
        SafeRelease(DxRenderContext.Buffers[BufferIndex].Buffer);
    SafeRelease(DxRenderContext.RenderTarget);

    DxRenderContext.SwapChain->ResizeBuffers(DxRenderContext.BufferCount, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    for (int BufferIndex = 0; BufferIndex < DxRenderContext.BufferCount; BufferIndex++)
        DxRenderContext.SwapChain->GetBuffer(BufferIndex, IID_PPV_ARGS(&DxRenderContext.Buffers[BufferIndex].Buffer));

    DxRenderContext.Device->CreateRenderTargetView(DxRenderContext.Buffers[0].Buffer, nullptr, &DxRenderContext.RenderTarget);
}

void DxRenderContextFree()
{
    EventSystemUnregister(event_type::Resize, nullptr, DxRenderResizeCallback);

    for (int BufferIndex = 0; BufferIndex < DxRenderContext.BufferCount; BufferIndex++)
    {
        SafeRelease(DxRenderContext.Buffers[BufferIndex].Buffer);
    }
    SafeRelease(DxRenderContext.RenderTarget);
    SafeRelease(DxRenderContext.SwapChain);
    SafeRelease(DxRenderContext.Factory);
    SafeRelease(DxRenderContext.Adapter);
    SafeRelease(DxRenderContext.DXGI);
    SafeRelease(DxRenderContext.Device);
}

void DxRenderContextBegin()
{
    D3D11_VIEWPORT Viewport = {};
    Viewport.TopLeftX = 0;
    Viewport.TopLeftY = 0;
    Viewport.Width = DxRenderContext.Width;
    Viewport.Height = DxRenderContext.Height;
    Viewport.MinDepth = 0.0f;
    Viewport.MaxDepth = 1.0f;
    
    D3D11_RECT Rect;
    Rect.top = 0;
    Rect.left = 0;
    Rect.right = Viewport.Width;
    Rect.bottom = Viewport.Height;

    float Clear[] = {0.0f, 0.0f, 0.0f, 1.0f};
    DxRenderContext.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DxRenderContext.DeviceContext->OMSetRenderTargets(1, &DxRenderContext.RenderTarget, nullptr);
    DxRenderContext.DeviceContext->ClearRenderTargetView(DxRenderContext.RenderTarget, Clear);
    DxRenderContext.DeviceContext->RSSetScissorRects(1, &Rect);
    DxRenderContext.DeviceContext->RSSetViewports(1, &Viewport);
}

void DxRenderContextPresent()
{
    bool VerticalSync = EgcB32(EgcFile, "vsync");
    DxRenderContext.SwapChain->Present(VerticalSync, 0);
}

void DxRenderContextDraw(uint32_t Count)
{
    DxRenderContext.DeviceContext->Draw(Count, 0);
}

void DxRenderContextDrawIndexed(uint32_t Count)
{
    DxRenderContext.DeviceContext->DrawIndexed(Count, 0, 0);
}
