/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 10:16
 */

#pragma once

#include "gpu/gpu_pipeline_profiler.hpp"
#include "gpu/gpu_buffer.hpp"

#include <d3d12.h>

struct dx12_pipeline_profiler
{
    ID3D12QueryHeap *Heap;
    gpu_buffer Buffer;
};
