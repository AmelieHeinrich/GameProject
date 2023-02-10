/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 21:07
 */

#pragma once

#include <cstdint>

enum class gpu_buffer_type
{
    Vertex,
    Index,
    Uniform,
    Storage
};

struct gpu_buffer
{
    gpu_buffer_type Type;
    uint64_t Size;
    uint64_t Stride;
    void *Reserved;
};

void GpuBufferInit(gpu_buffer *Buffer, uint64_t Size, uint64_t Stride, gpu_buffer_type Type);
void GpuBufferInitForCopy(gpu_buffer *Buffer, uint64_t Size);
void GpuBufferFree(gpu_buffer *Buffer);
void GpuBufferUpload(gpu_buffer *Buffer, const void *Data, uint64_t Size);
