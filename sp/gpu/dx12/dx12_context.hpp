/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 11:33
 */

#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>

struct dx12_context
{
    ID3D12Device* Device;
    ID3D12Debug1* Debug;
    ID3D12DebugDevice* DebugDevice;
    IDXGIDevice* DXGI;
    IDXGIFactory3* Factory;
    IDXGIAdapter1* Adapter;
};

extern dx12_context DX12;
