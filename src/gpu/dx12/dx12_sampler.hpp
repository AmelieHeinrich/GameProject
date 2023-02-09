/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 00:32
 */

#pragma once

#include "gpu/gpu_sampler.hpp"

#include <d3d12.h>
#include <cstdint>

struct dx12_sampler
{
    D3D12_SAMPLER_DESC Desc;
    uint32_t Descriptor;
};
