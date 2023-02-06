/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 10:44
 */

#pragma once

#include "gpu/gpu_image.hpp"
#include "dx12_descriptor_heap.hpp"
#include <d3d12.h>

struct dx12_image
{
    ID3D12Resource* Resource;
    uint32_t Descriptor;
    D3D12_RESOURCE_STATES State;
};
