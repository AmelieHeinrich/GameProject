/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 13:02
 */

#pragma once

#include <d3d12.h>
#include <cstdint>

#include "gpu/gpu_shader.hpp"

struct dx12_shader_module
{
    void *Data;
    uint64_t Size;
};

struct dx12_shader
{
    dx12_shader_module VertexBlob;
    dx12_shader_module PixelBlob;
    dx12_shader_module ComputeBlob;
};
