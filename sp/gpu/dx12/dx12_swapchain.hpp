/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 14:06
 */

#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdint>
#include <vector>

struct dx12_swapchain
{
    IDXGISwapChain3 *SwapChain;
    std::vector<ID3D12Resource*> Buffers;
    std::vector<int> RenderTargets;
};

void Dx12SwapchainInit(dx12_swapchain *SwapChain);
void Dx12SwapchainFree(dx12_swapchain *SwapChain);
void Dx12SwapchainPresent(dx12_swapchain *SwapChain);
int Dx12SwapchainImageIndex(dx12_swapchain *SwapChain);
void Dx12SwapchainResize(dx12_swapchain *SwapChain, uint32_t Width, uint32_t Height);
