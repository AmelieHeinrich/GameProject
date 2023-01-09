/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/01/2023 19:47
 */

#pragma once

#include "dx11_common.hpp"

#include <cstdint>

struct dx_render_context
{
    HWND RenderWindow;
    uint32_t Width;
    uint32_t Height;
    uint32_t RefreshRate;

    IDXGIAdapter* Adapter;
    IDXGIFactory* Factory;
    IDXGIDevice* DXGI;
    ID3D11Device* Device;
    ID3D11DeviceContext* DeviceContext;
    D3D_FEATURE_LEVEL FeatureLevel;

    IDXGISwapChain* SwapChain;
    ID3D11Texture2D* SwapChainBuffer;
    ID3D11RenderTargetView* SwapChainRTV;
};

extern dx_render_context DxRenderContext;

void DxRenderContextInit(HWND Window);
void DxRenderContextFree();
void DxRenderContextResize(uint32_t Width, uint32_t Height);
