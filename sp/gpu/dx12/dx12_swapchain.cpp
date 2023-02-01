/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 14:09
 */

#include "dx12_swapchain.hpp"

#include "dx12_context.hpp"
#include "game_data.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

void Dx12SwapchainInit(dx12_swapchain *SwapChain)
{
    RECT Rectangle;
    GetClientRect(Win32.Window, &Rectangle);
    DX12.Width = Rectangle.right - Rectangle.left;
    DX12.Height = Rectangle.bottom - Rectangle.top;

    DXGI_SWAP_CHAIN_DESC1 Desc = {};
    Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Desc.SampleDesc.Count = 1;
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    Desc.BufferCount = EgcI32(EgcFile, "buffer_count");
    Desc.Scaling = DXGI_SCALING_NONE;
    Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    Desc.Width = DX12.Width;
    Desc.Height = DX12.Height;

    IDXGISwapChain1 *Temp;
    HRESULT Result = DX12.Factory->CreateSwapChainForHwnd(DX12.CommandQueue, Win32.Window, &Desc, nullptr, nullptr, &Temp);
    if (FAILED(Result))
        LogError("D3D12: Failed to create swap chain!");
    Temp->QueryInterface(IID_PPV_ARGS(&SwapChain->SwapChain));
    SafeRelease(Temp);

    int BufferCount = EgcI32(EgcFile, "buffer_count");
    SwapChain->Buffers.resize(BufferCount);
    SwapChain->RenderTargets.resize(BufferCount);
    
    for (int BufferIndex = 0; BufferIndex < BufferCount; BufferIndex++)
    {
        Result = SwapChain->SwapChain->GetBuffer(BufferIndex, IID_PPV_ARGS(&SwapChain->Buffers[BufferIndex]));
        if (FAILED(Result))
            LogError("D3D12: Failed to get swap chain buffer at index %d!", BufferIndex);
        SwapChain->RenderTargets[BufferIndex] = Dx12DescriptorHeapAlloc(&DX12.RTVHeap);
        DX12.Device->CreateRenderTargetView(SwapChain->Buffers[BufferIndex], nullptr, Dx12DescriptorHeapCPU(&DX12.RTVHeap, SwapChain->RenderTargets[BufferIndex]));
    }
}

void Dx12SwapchainFree(dx12_swapchain *SwapChain)
{
    int BufferCount = EgcI32(EgcFile, "buffer_count");
    for (int BufferIndex = 0; BufferIndex < BufferCount; BufferIndex++)
    {
        SafeRelease(SwapChain->Buffers[BufferIndex]);
        Dx12DescriptorHeapFreeSpace(&DX12.RTVHeap, SwapChain->RenderTargets[BufferIndex]);
    }
    SafeRelease(SwapChain->SwapChain);
}

void Dx12SwapchainPresent(dx12_swapchain *SwapChain)
{
    bool VerticalSync = EgcB32(EgcFile, "vsync");
    SwapChain->SwapChain->Present(VerticalSync, 0);
}

int Dx12SwapchainImageIndex(dx12_swapchain *SwapChain)
{
    return SwapChain->SwapChain->GetCurrentBackBufferIndex();
}

void Dx12SwapchainResize(dx12_swapchain *SwapChain, uint32_t Width, uint32_t Height)
{
    if (SwapChain->SwapChain)
    {
        int BufferCount = EgcI32(EgcFile, "buffer_count");
        for (int BufferIndex = 0; BufferIndex < BufferCount; BufferIndex++)
        {
            SafeRelease(SwapChain->Buffers[BufferIndex]);
            Dx12DescriptorHeapFreeSpace(&DX12.RTVHeap, SwapChain->RenderTargets[BufferIndex]);
        }
        HRESULT Result = SwapChain->SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
        if (FAILED(Result))
            LogError("D3D12: Failed to resize swap chain buffers!");
        for (int BufferIndex = 0; BufferIndex < BufferCount; BufferIndex++)
        {
            Result = SwapChain->SwapChain->GetBuffer(BufferIndex, IID_PPV_ARGS(&SwapChain->Buffers[BufferIndex]));
            if (FAILED(Result))
                LogError("D3D12: Failed to get swap chain buffer at index %d!", BufferIndex);
            SwapChain->RenderTargets[BufferIndex] = Dx12DescriptorHeapAlloc(&DX12.RTVHeap);
            DX12.Device->CreateRenderTargetView(SwapChain->Buffers[BufferIndex], nullptr, Dx12DescriptorHeapCPU(&DX12.RTVHeap, SwapChain->RenderTargets[BufferIndex]));
        }
    }
}
