/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 13:36
 */

#pragma once

#include "gpu/gpu_command_buffer.hpp"

#include <d3d12.h>

struct dx12_command_buffer
{
    ID3D12CommandAllocator *Allocator;
    ID3D12GraphicsCommandList *List;
};
